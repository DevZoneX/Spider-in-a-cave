#pragma once

#include "cgp/cgp.hpp"
#include "../environment.hpp"
#include "../utils/collision_handler.hpp"
#include "../utils/collision_object.hpp"

using namespace cgp;

class cristal{
public:
    static bool texturesInitialized;
    static opengl_texture_image_structure texture_purple;
    static opengl_texture_image_structure texture_orange;
    static opengl_shader_structure cristal_shader;
    vec3 translation={0,0,0};
    rotation_transform rotation;
    float scaling = 1;
    vec3 scaling_xyz = {1,1,1};

    mesh_drawable toDraw;

    float intensity = 1.5;
    float distance = 7;

    vec3 color = {1,1,1};


    float lightDistance;
    float lightIntensity;
    cristal();
    virtual void initialize(){}
    virtual void initialize(collision_partition *partition){initialize();addCollisions(partition);}
    virtual void addCollisions(collision_partition *partition){if(partition==NULL){}}
    void checkTextures();
    void update();
    virtual void draw(environment_structure environment);
    virtual vec3 getLightPosition(){return translation;}
    virtual light_params getLightParams();
};

class cristal_ram: public cristal{
protected:
    virtual void chooseTexture();
public:
    static mesh cristal;
    static mesh_drawable cristald;
    static bool initialized;

    cristal_ram();


    void initialize() override;
    void addCollisions(collision_partition *partition) override;
    vec3 getLightPosition() override;
};

class cristal_rock: public cristal{
protected:
    virtual void chooseTexture();
public:
    static mesh cristal;
    static mesh_drawable cristald;
    static bool initialized;

    cristal_rock();


    void initialize() override;
    void addCollisions(collision_partition *partition) override;
    vec3 getLightPosition() override;
};
class cristal_large: public cristal{
protected:
    virtual void chooseTexture();
public:
    static mesh cristal;
    static mesh_drawable cristald;
    static bool initialized;

    cristal_large();


    void initialize() override;
    void addCollisions(collision_partition *partition) override;
    vec3 getLightPosition() override;
};


class cristal_rock_gold: public cristal_rock{
protected:
    void chooseTexture();
public:
    cristal_rock_gold();
};

class cristal_large_gold: public cristal_large{
protected:
    void chooseTexture();
public:
    cristal_large_gold();
};

class cristal_ram_gold: public cristal_ram{
protected:
    void chooseTexture();
public:
    cristal_ram_gold();
};



