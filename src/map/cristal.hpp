#pragma once

#include "cgp/cgp.hpp"
#include "../environment.hpp"

using namespace cgp;

class cristal{
public:
    static bool texturesInitialized;
    static opengl_texture_image_structure texture_purple;
    static opengl_texture_image_structure texture_orange;
    vec3 translation={0,0,0};
    rotation_transform rotation;
    float scaling = 1;
    vec3 scaling_xyz = {1,1,1};

    mesh_drawable toDraw;

    float intensity = 1.5;
    float distance = 7;

    vec3 color = {1,1,1};


    vec3 lightColor={1,1,1};
    float lightDistance;
    float lightIntensity;
    cristal(){}
    virtual void initialize(){}
    void checkTextures();
    void update();
    virtual void draw(environment_structure environment){if(environment.light.x==0){}}
    virtual vec3 getLightPosition(){return translation;}
    virtual light_params getLightParams();
};

class cristal_ram: public cristal{
protected:
    virtual void chooseTexture();
public:
    static mesh_drawable cristald;
    static bool initialized;

    cristal_ram();


    void initialize() override;
    void draw(environment_structure environment) override;
    vec3 getLightPosition() override;
};
