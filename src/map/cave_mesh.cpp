#include "cave_mesh.hpp"
#include "../utils/math.hpp"


cave_mesh::cave_mesh()
{

}

bool cave_mesh::initialized_textures = false;
opengl_texture_image_structure cave_mesh::texture;
opengl_texture_image_structure cave_mesh::normal_map_texture;
opengl_shader_structure cave_mesh::shader;

void cave_mesh::initialize(){
    cmesh_ground = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},terrain_sample,terrain_sample);
    cmesh = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},arch_sample,arch_sample);
    cmesh_wall1 = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},wall_sample,wall_sample);
    cmesh_wall2 = mesh_primitive_grid({-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0},wall_sample,wall_sample);
    cmeshd.initialize_data_on_gpu(cmesh);
    cmeshd_ground.initialize_data_on_gpu(cmesh_ground);
    cmeshd_wall1.initialize_data_on_gpu(cmesh_wall1);
    cmeshd_wall2.initialize_data_on_gpu(cmesh_wall2);
    cmeshd.model.scaling = 10 * scaling;
    cmeshd.model.translation = {0,0,scaling * 4.5/2};
    cmeshd.model.scaling_xyz = {1,length,1};
    cmeshd_ground.model.scaling = 10 * scaling;
    cmeshd_ground.model.translation = {0,0,scaling * (-11-r)/2};
    cmeshd_ground.model.scaling_xyz = {1,length,1};
    
    cmeshd_wall1.model.scaling = 10 * scaling;
    cmeshd_wall1.model.translation = {0,0,scaling * 4.5/2};
    cmeshd_wall1.model.scaling_xyz = {1,length,1};
    cmeshd_wall2.model.scaling = 10 * scaling;
    cmeshd_wall2.model.translation = {0,0,scaling * 4.5/2};
    cmeshd_wall2.model.scaling_xyz = {1,length,1};


    if(partition==NULL){
        partition = new collision_partition({1.2,1.2,1.3});   
    }
    collision_handler::initialize(partition);


    //cmeshd_ground.model.rotation = rotation_axis_angle({1,0,0},-Pi/2);
    update_terrain();

    if(!initialized_textures){
        texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/rock_face_comp.png",
            GL_REPEAT,
            GL_REPEAT);
        normal_map_texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/rock_face_normal_comp.png");
        shader.load(project::path + "shaders/normal_map/mesh_normal.vert.glsl", project::path + "shaders/normal_map/mesh_normal.frag.glsl");
        initialized_textures = true;
    }


    cmeshd.texture = texture;
    cmeshd.supplementary_texture["image_texture_2"] = normal_map_texture;
    cmeshd.shader = shader;
    cmeshd_wall1.texture = texture;
    cmeshd_wall1.supplementary_texture["image_texture_2"] = normal_map_texture;
    cmeshd_wall1.shader = shader;
    cmeshd_wall2.texture = texture;
    cmeshd_wall2.supplementary_texture["image_texture_2"] = normal_map_texture;
    cmeshd_wall2.shader = shader;
    //glEnableVertexAttribArray(2); // Tangent
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    //glEnableVertexAttribArray(3); // Bitangent
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    cmeshd_ground.shader = shader;
    cmeshd_ground.texture = texture;
    cmeshd_ground.supplementary_texture["image_texture_2"] = normal_map_texture;
    

    cmeshd.material.phong.specular *= 0.2;
    cmeshd.material.phong.specular_exponent = 10;
    cmeshd.material.phong.diffuse *= 0.6;

    cmeshd_ground.material.phong.specular *= 0.2;
    cmeshd_ground.material.phong.specular_exponent = 10;
    cmeshd_ground.material.phong.diffuse *= 0.6;

    cmeshd_wall1.material = cmeshd_ground.material;
    cmeshd_wall2.material = cmeshd_ground.material;
}

void cave_mesh::initialize(collision_partition *_partition){
    partition = _partition;
    initialize();
}

cave_mesh::~cave_mesh(){
    delete partition;
}


void cave_mesh::draw(environment_structure environment){
    cgp::draw(cmeshd,environment);
    cgp::draw(cmeshd_ground,environment);
    cgp::draw(cmeshd_wall1,environment);
    cgp::draw(cmeshd_wall2,environment);
}

void cave_mesh::update_terrain()
{
    // Number of samples in each direction (assuming a square grid)
    int const N = std::sqrt(cmesh.position.size());
    int const N_ground = std::sqrt(cmesh_ground.position.size());
    int const N_wall = std::sqrt(cmesh_wall1.position.size());
    numarray<vec3> tangents;
    numarray<vec3> bitangents;


    numarray<vec3> tangents_ground;
    numarray<vec3> bitangents_ground;

    numarray<vec3> tangents_wall1;
    numarray<vec3> bitangents_wall1;
    numarray<vec3> tangents_wall2;
    numarray<vec3> bitangents_wall2;

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
    // Do the same for the walls
    for (int ku = 0; ku < N_wall; ++ku) {
        for (int kv = 0; kv < N_wall; ++kv) {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N_wall-1.0f);
            const float v = kv/(N_wall-1.0f);

            int const idx = ku*N_wall+kv;

            // Compute the Perlin noise
            float const noise = noise_perlin({u, length*v/2}, octave, persistency, frequency_gain);
            float const noise2 = noise_perlin({u+1, length*v/2+1}, octave, persistency, frequency_gain);

            // use the noise as height value
            float dr = 0.3*terrain_height*noise;
            float dr2 = 0.3*terrain_height*noise2;

            float x=u,y=v,offset,offset2,offsetz;
            float multx=1,multz=1;
            if(u<0.5){multx = -1;}
            if(v<0.5){multz = -1;}
            multx *= 1.05;
            multz *= 1.07;

            offset = +1.22;
            offset2 = 1.22;
            offsetz = 0.17;

            x = x-0.5;
            y = y-0.5;

            float norm = sqrt(x*x+y*y);

            float size_mult = 0.70;

            cmesh_wall1.position[idx].x = multx*size_mult*(log(fabs(u-0.5)+0.3)+1.2039);
            cmesh_wall1.position[idx].y = offset+dr-1.35*norm;
            cmesh_wall1.position[idx].z = offsetz + multz*size_mult*(log(fabs(v-0.5)+0.3)+1.2039);
            
            cmesh_wall2.position[idx].x = -multx*size_mult*(log(fabs(u-0.5)+0.3)+1.2039);
            cmesh_wall2.position[idx].y = -(offset2+dr2-1.35*norm);
            cmesh_wall2.position[idx].z = offsetz -0.03 + multz*size_mult*(log(fabs(v-0.5)+0.3)+1.2039);

            tangents_wall1.push_back({0,1,0});
            bitangents_wall1.push_back({1,0,0});
            tangents_wall2.push_back({0,1,0});
            bitangents_wall2.push_back({1,0,0});
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
    for (int ku = 0; ku < N_wall-1; ++ku) {
        for (int kv = 0; kv < N_wall-1; ++kv) {
            int const idx = ku*N_wall+kv;
            int const idx2 = ku*N_wall+kv+1;
            int const idx3 = (ku+1)*N_wall+kv;
            int const idx4 = (ku+1)*N_wall+kv+1;

            vec3 pos1 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx]+cmeshd_wall1.model.translation;
            vec3 pos2 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx2]+cmeshd_wall1.model.translation;
            vec3 pos3 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx3]+cmeshd_wall1.model.translation;
            vec3 pos4 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx4]+cmeshd_wall1.model.translation;

            partition->add_collision(new collision_triangle(pos1,pos2-pos1,pos4-pos1));
            partition->add_collision(new collision_triangle(pos1,pos3-pos1,pos4-pos1));

            pos1 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx]+cmeshd_wall1.model.translation;
            pos2 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx2]+cmeshd_wall1.model.translation;
            pos3 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx3]+cmeshd_wall1.model.translation;
            pos4 = cmeshd_wall1.model.scaling*cmeshd_wall1.model.scaling_xyz*cmesh.position[idx4]+cmeshd_wall1.model.translation;

            partition->add_collision(new collision_triangle(pos1,pos2-pos1,pos4-pos1));
            partition->add_collision(new collision_triangle(pos1,pos3-pos1,pos4-pos1));
        }
    }

    for (int ku = 0; ku < N-1; ++ku) {
        for (int kv = 0; kv < N-1; ++kv) {
            int const idx = ku*N+kv;
            int const idx2 = ku*N+kv+1;
            int const idx3 = (ku+1)*N+kv;
            int const idx4 = (ku+1)*N+kv+1;

            vec3 pos1 = cmeshd.model.scaling*cmeshd.model.scaling_xyz*cmesh.position[idx]+cmeshd.model.translation;
            vec3 pos2 = cmeshd.model.scaling*cmeshd.model.scaling_xyz*cmesh.position[idx2]+cmeshd.model.translation;
            vec3 pos3 = cmeshd.model.scaling*cmeshd.model.scaling_xyz*cmesh.position[idx3]+cmeshd.model.translation;
            vec3 pos4 = cmeshd.model.scaling*cmeshd.model.scaling_xyz*cmesh.position[idx4]+cmeshd.model.translation;

            partition->add_collision(new collision_triangle(pos1,pos2-pos1,pos4-pos1));
            partition->add_collision(new collision_triangle(pos1,pos3-pos1,pos4-pos1));
        }
    }

    for (int ku = 0; ku < N_ground-1; ++ku) {
        for (int kv = 0; kv < N_ground-1; ++kv) {
            int const idx = ku*N_ground+kv;
            int const idx2 = ku*N_ground+kv+1;
            int const idx3 = (ku+1)*N_ground+kv;
            int const idx4 = (ku+1)*N_ground+kv+1;

            vec3 pos1 = cmeshd_ground.model.scaling*cmeshd_ground.model.scaling_xyz*cmesh_ground.position[idx]+cmeshd_ground.model.translation;
            vec3 pos2 = cmeshd_ground.model.scaling*cmeshd_ground.model.scaling_xyz*cmesh_ground.position[idx2]+cmeshd_ground.model.translation;
            vec3 pos3 = cmeshd_ground.model.scaling*cmeshd_ground.model.scaling_xyz*cmesh_ground.position[idx3]+cmeshd_ground.model.translation;
            vec3 pos4 = cmeshd_ground.model.scaling*cmeshd_ground.model.scaling_xyz*cmesh_ground.position[idx4]+cmeshd_ground.model.translation;

            partition->add_collision(new collision_triangle(pos1,pos2-pos1,pos4-pos1));
            partition->add_collision(new collision_triangle(pos1,pos3-pos1,pos4-pos1));
        }
    }

    // Update the normal of the mesh structure
    cmesh.normal_update();
    cmesh_ground.normal_update();
    cmesh_wall1.normal_update();
    cmesh_wall2.normal_update();


    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {
            int const idx = ku*N+kv;
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

    for (int ku = 0; ku < N_wall; ++ku) {
        for (int kv = 0; kv < N_wall; ++kv) {
            int const idx = ku*N_wall+kv;
            int idx2;
            int idx3;

            if(ku==N_wall-1 && kv==N_wall-1){
                idx2 = (ku-1)*N_wall+kv-1;
                idx3 = (ku-1)*N_wall+kv;
            }
            else if(ku==N_wall-1){
                idx2 = ku*N_wall+kv+1;
                idx3 = (ku-1)*N_wall+kv;
            }
            else if(kv==N_wall-1){
                idx2 = ku*N_wall+kv-1;
                idx3 = (ku+1)*N_wall+kv;
            }
            else{
                idx2 = (ku+1)*N_wall+kv;
                idx3 = (ku+1)*N_wall+kv+1;
            }

            tangents_wall1[idx] = math::calculate_tangent(cmesh_wall1.position[idx],cmesh_wall1.position[idx2],cmesh_wall1.position[idx3],cmesh_wall1.uv[idx],cmesh_wall1.uv[idx2],cmesh_wall1.uv[idx3],cmesh_wall1.normal[idx]);
            bitangents_wall1[idx] = math::calculate_bitangent(tangents_wall1[idx],cmesh_wall1.normal[idx]);
            
            tangents_wall2[idx] = math::calculate_tangent(cmesh_wall2.position[idx],cmesh_wall2.position[idx2],cmesh_wall2.position[idx3],cmesh_wall2.uv[idx],cmesh_wall2.uv[idx2],cmesh_wall2.uv[idx3],cmesh_wall2.normal[idx]);
            bitangents_wall2[idx] = math::calculate_bitangent(tangents_wall1[idx],cmesh_wall2.normal[idx]);
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
    //cmeshd.supplementary_vbo.push_back(tangents_vbo);
    //cmeshd.supplementary_vbo.push_back(bitangents_vbo);


    cmeshd_ground.vbo_position.update(cmesh_ground.position);
    cmeshd_ground.vbo_normal.update(cmesh_ground.normal);
    cmeshd_ground.vbo_color.update(cmesh_ground.color);
    cmeshd_ground.initialize_supplementary_data_on_gpu(tangents_ground,4);
    cmeshd_ground.initialize_supplementary_data_on_gpu(bitangents_ground,5);


    cmeshd_wall1.vbo_position.update(cmesh_wall1.position);
    cmeshd_wall1.vbo_normal.update(cmesh_wall1.normal);
    cmeshd_wall1.vbo_color.update(cmesh_wall1.color);
    cmeshd_wall1.initialize_supplementary_data_on_gpu(tangents_wall1,4);
    cmeshd_wall1.initialize_supplementary_data_on_gpu(bitangents_wall1,5);

    cmeshd_wall2.vbo_position.update(cmesh_wall2.position);
    cmeshd_wall2.vbo_normal.update(cmesh_wall2.normal);
    cmeshd_wall2.vbo_color.update(cmesh_wall2.color);
    cmeshd_wall2.initialize_supplementary_data_on_gpu(tangents_wall2,4);
    cmeshd_wall2.initialize_supplementary_data_on_gpu(bitangents_wall2,5);
}
