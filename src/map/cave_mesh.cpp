#include "cave_mesh.hpp"
#include "../utils/math.hpp"


cave_mesh::cave_mesh()
{

}



void cave_mesh::initialize(){
    cmesh_ground = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},terrain_sample,terrain_sample);
    cmesh = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},arch_sample,arch_sample);
    cmeshd.initialize_data_on_gpu(cmesh);
    cmeshd_ground.initialize_data_on_gpu(cmesh_ground);
    cmeshd.model.scaling = 10 * scaling;
    cmeshd.model.translation = {0,0,scaling * 4.5/2};
    cmeshd.model.scaling_xyz = {1,length,1};
    cmeshd_ground.model.scaling = 10 * scaling;
    cmeshd_ground.model.translation = {0,0,scaling * (-11-r)/2};
    cmeshd_ground.model.scaling_xyz = {1,length,1};


    //cmeshd_ground.model.rotation = rotation_axis_angle({1,0,0},-Pi/2);
    update_terrain();


    cmeshd.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/rock_face_comp.png",
            GL_REPEAT,
            GL_REPEAT);
    //glEnableVertexAttribArray(2); // Tangent
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    //glEnableVertexAttribArray(3); // Bitangent
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    cmeshd.shader.load(project::path + "shaders/normal_map/mesh_normal.vert.glsl", project::path + "shaders/normal_map/mesh_normal.frag.glsl");
    cmeshd_ground.shader.load(project::path + "shaders/normal_map/mesh_normal.vert.glsl", project::path + "shaders/normal_map/mesh_normal.frag.glsl");
    cmeshd_ground.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/rock_face_comp.png",
            GL_REPEAT,
            GL_REPEAT);
    cmeshd_ground.supplementary_texture["image_texture_2"].load_and_initialize_texture_2d_on_gpu(project::path + "assets/rock_face_normal_comp.png");

    cmeshd.material.phong.specular *= 0.2;
    cmeshd.material.phong.specular_exponent = 10;
    cmeshd.material.phong.diffuse *= 0.6;

    cmeshd_ground.material.phong.specular *= 0.2;
    cmeshd_ground.material.phong.specular_exponent = 10;
    cmeshd_ground.material.phong.diffuse *= 0.6;
}

void cave_mesh::draw(environment_structure environment){
    cgp::draw(cmeshd,environment);
    cgp::draw(cmeshd_ground,environment);

}

void cave_mesh::update_terrain()
{
    // Number of samples in each direction (assuming a square grid)
    int const N = std::sqrt(cmesh.position.size());
    int const N_ground = std::sqrt(cmesh_ground.position.size());
    numarray<vec3> tangents;
    numarray<vec3> bitangents;


    numarray<vec3> tangents_ground;
    numarray<vec3> bitangents_ground;

    // Recompute the new vertices
    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {

            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            int const idx = ku*N+kv;

            // Compute the Perlin noise
            float const noise = noise_perlin({u, length*v/2}, octave, persistency, frequency_gain);

            // use the noise as height value
            float dr = terrain_height*noise;

            cmesh.position[idx].z = scaling*def_r*r*(1+dr)*sin(1.4*Pi*u-0.2*Pi);
            cmesh.position[idx].x = scaling*r*(1+dr)*cos(1.4*Pi*u-0.2*Pi);

            tangents.push_back({0,1,0});
            bitangents.push_back({1,0,0});

            // use also the noise as color value
            //cmesh.color[idx] = vec3(0,0.5f,0)*0.3f+0.7f*noise*vec3(1,1,1);
        }
    }

    // Do the same for ground
    for (int ku = 0; ku < N_ground; ++ku) {
        for (int kv = 0; kv < N_ground; ++kv) {
            const float u = ku/(N_ground-1.0f);
            const float v = kv/(N_ground-1.0f);

            int const idx = ku*N_ground+kv;

            float const noise = noise_perlin({u, length*v}, octave_ground, persistency_ground, frequency_gain_ground);

            // use the noise as height value
            float dr = terrain_height_ground*noise;
            cmesh_ground.position[idx].z = dr+0*pow(u-0.5,2) + pow(u-0.5,2)/r;
            //cmesh_ground.position[idx].z = 0;
            tangents_ground.push_back({0,1,0});
            bitangents_ground.push_back({1,0,0});
        }
    }

    // Update the normal of the mesh structure
    cmesh.normal_update();
    cmesh_ground.normal_update();


    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {
            int const idx = ku*N+kv;
            cmesh.normal[idx] *= -1;
            int idx2;
            int idx3;

            if(ku==N-1 && kv==N-1){
                idx2 = (ku-1)*N+kv-1;
                idx3 = (ku-1)*N+kv;
            }
            else if(ku==N-1){
                idx2 = ku*N+kv+1;
                idx3 = (ku-1)*N+kv;
            }
            else if(kv==N-1){
                idx2 = ku*N+kv-1;
                idx3 = (ku+1)*N+kv;
            }
            else{
                idx2 = (ku+1)*N+kv;
                idx3 = (ku+1)*N+kv+1;
            }

            tangents[idx] = math::calculate_tangent(cmesh.position[idx],cmesh.position[idx2],cmesh.position[idx3],cmesh.uv[idx],cmesh.uv[idx2],cmesh.uv[idx3],cmesh.normal[idx]);
            bitangents[idx] = math::calculate_bitangent(tangents[idx],cmesh.normal[idx]);
        }
    }

    for (int ku = 0; ku < N_ground; ++ku) {
        for (int kv = 0; kv < N_ground; ++kv) {
            int const idx = ku*N_ground+kv;

            int idx2;
            int idx3;

            if(ku==N_ground-1 && kv==N_ground-1){
                idx2 = (ku-1)*N_ground+kv-1;
                idx3 = (ku-1)*N_ground+kv;
            }
            else if(ku==N_ground-1){
                idx2 = ku*N_ground+kv+1;
                idx3 = (ku-1)*N_ground+kv;
            }
            else if(kv==N_ground-1){
                idx2 = ku*N_ground+kv-1;
                idx3 = (ku+1)*N_ground+kv;
            }
            else{
                idx2 = (ku+1)*N_ground+kv;
                idx3 = (ku+1)*N_ground+kv+1;
            }

            tangents_ground[idx] = math::calculate_tangent(cmesh_ground.position[idx],cmesh_ground.position[idx2],cmesh_ground.position[idx3],cmesh_ground.uv[idx],cmesh_ground.uv[idx2],cmesh_ground.uv[idx3],cmesh_ground.normal[idx]);
            bitangents_ground[idx] = math::calculate_bitangent(tangents_ground[idx],cmesh_ground.normal[idx]);
        }
    }



    // Update step: Allows to update a mesh_drawable without creating a new one
    cmeshd.vbo_position.update(cmesh.position);
    cmeshd.vbo_normal.update(cmesh.normal);
    cmeshd.vbo_color.update(cmesh.color);
    cmeshd.initialize_supplementary_data_on_gpu(tangents,4);
    cmeshd.initialize_supplementary_data_on_gpu(bitangents,5);
    cmeshd_ground.initialize_supplementary_data_on_gpu(tangents_ground,4);
    cmeshd_ground.initialize_supplementary_data_on_gpu(bitangents_ground,5);
    //cmeshd.supplementary_vbo.push_back(tangents_vbo);
    //cmeshd.supplementary_vbo.push_back(bitangents_vbo);


    cmeshd_ground.vbo_position.update(cmesh_ground.position);
    cmeshd_ground.vbo_normal.update(cmesh_ground.normal);
    cmeshd_ground.vbo_color.update(cmesh_ground.color);

}
