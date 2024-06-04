#ifndef CAVE_H
#define CAVE_H

#include "../environment.hpp"
#include "cave_mesh.hpp"
#include "cristal.hpp"

class cave: public collision_handler
{
private:
    cave_mesh CaveMesh;

    cristal_rock cristal1;
    cristal_ram cristal2;
    cristal_large cristal3;
    cristal_large_gold cristal4;
    cristal_ram_gold cristal5;
    cristal_rock_gold cristal6;
public:
    cave();

    void initialize();

    void draw(environment_structure &environment);
};

#endif // CAVE_H
