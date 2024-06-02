#ifndef CAVE_H
#define CAVE_H

#include "../environment.hpp"
#include "cave_mesh.hpp"
#include "cristal.hpp"

class cave: public collision_handler
{
private:
    cave_mesh CaveMesh;

    cristal_ram cristal1;
    cristal_rock cristal2;
public:
    cave();

    void initialize();

    void draw(environment_structure &environment);
};

#endif // CAVE_H
