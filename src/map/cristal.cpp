#include "cristal.hpp"


bool cristal::texturesInitialized = false;
opengl_texture_image_structure cristal::texture_purple;
opengl_texture_image_structure cristal::texture_orange;

void cristal::checkTextures(){
    if(!texturesInitialized){
        std::cout << "here" << std::endl;
        texture_purple.load_and_initialize_texture_2d_on_gpu(project::path+"assets/cristal/textures/Cristals.png",GL_REPEAT,GL_REPEAT);
        std::cout << "here2" << std::endl;
        texture_orange.load_and_initialize_texture_2d_on_gpu(project::path+"assets/cristal/textures/Cristals2.png",GL_REPEAT,GL_REPEAT);
        std::cout << "here3" << std::endl;
        texturesInitialized = true;
    }
}

void cristal::update(){
    toDraw.model.scaling = scaling;
    toDraw.model.scaling_xyz = scaling_xyz;
    toDraw.model.translation = translation;
    toDraw.model.rotation = rotation;

    toDraw.material.phong.ambient=1;
    toDraw.material.phong.specular = 0.2;
    toDraw.material.phong.diffuse = 0;
    toDraw.material.phong.specular_exponent = 40;
}

light_params cristal::getLightParams(){
    return light_params(getLightPosition(),color,distance,intensity);
}

bool cristal_ram::initialized = false;
mesh_drawable cristal_ram::cristald;
void cristal_ram::chooseTexture(){
    toDraw.texture = cristal::texture_purple;
}
cristal_ram::cristal_ram(){
    intensity = 1.5;
    distance = 7;

    color = {1,0.5,1};
}
void cristal_ram::initialize()
{
    if(!initialized){
        cristald.initialize_data_on_gpu(mesh_load_file_obj(project::path+"assets/cristal/cristals2.obj"));
        initialized = true;
    }
    toDraw = cristald;
    checkTextures();
    chooseTexture();
}

void cristal_ram::draw(environment_structure environment){    
    cgp::draw(toDraw,environment);
}

vec3 cristal_ram::getLightPosition()
{
    vec3 up = {0,0,1};
    up = scaling_xyz * up;
    up = rotation * up;
    up = scaling * up;
    up *= 0.9;
    return translation + up;
}

