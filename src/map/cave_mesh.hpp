#ifndef CAVE_MESH_H
#define CAVE_MESH_H

#include "cgp/cgp.hpp"
#include "../environment.hpp"
#include "../utils/collision_handler.hpp"
#include "../utils/collision_object.hpp"
#include "../utils/touchable_object.hpp"

class cave_mesh: public collision_handler
{
private:
    cgp::mesh cmesh;
    cgp::mesh_drawable cmeshd;
    cgp::mesh cmesh_ground;
    cgp::mesh_drawable cmeshd_ground;

    cgp::mesh cmesh_wall1;
    cgp::mesh_drawable cmeshd_wall1;
    cgp::mesh cmesh_wall2;
    cgp::mesh_drawable cmeshd_wall2;

    bool createdPartition = true;

    std::vector<collision_object*> toDelete;


public:
    cave_mesh();
    ~cave_mesh();

    static int const terrain_sample = 300;
    static int const arch_sample = 200;
    static int const wall_sample = 150;

    static bool initialized_textures;
    static opengl_texture_image_structure texture;
    static opengl_texture_image_structure normal_map_texture;
    static opengl_shader_structure shader;

    int octave = 8;
    float persistency = 0.7;
    float frequency_gain = 1.5;
    float terrain_height = 0.45f;
    int octave_ground = 8;
    float persistency_ground = 0.55;
    float frequency_gain_ground = 1.5;
    float terrain_height_ground = 0.25f;

    float quadraticWeight = 1.0;

    timer_basic timer;


    float r=0.4f; // Cave radius horizontal
    float def_r=1.3f; // Cave deformation

    float scaling = 1;

    float length = 2;

    void initialize();
    void initialize(collision_partition *_partition);
    void draw(environment_structure environment);
    void update_terrain();

    static opengl_shader_structure getShader();
};

#endif // CAVE_MESH_H
