#ifndef CAVE_MESH_H
#define CAVE_MESH_H

#include "cgp/cgp.hpp"
#include "../environment.hpp"

class cave_mesh
{
private:
    cgp::mesh cmesh;
    cgp::mesh_drawable cmeshd;
    cgp::mesh cmesh_ground;
    cgp::mesh_drawable cmeshd_ground;
public:
    cave_mesh();

    static int const terrain_sample = 300;
    static int const arch_sample = 200;

    int octave = 8;
    float persistency = 0.7;
    float frequency_gain = 1.5;
    float terrain_height = 0.45f;
    int octave_ground = 8;
    float persistency_ground = 0.55;
    float frequency_gain_ground = 1.5;
    float terrain_height_ground = 0.25f;

    float quadraticWeight = 1.0;


    float r=0.4f; // Cave radius horizontal
    float def_r=1.3f; // Cave deformation

    float scaling = 1;

    float length = 2;

    void initialize();
    void draw(environment_structure environment);
    void update_terrain();
};

#endif // CAVE_MESH_H
