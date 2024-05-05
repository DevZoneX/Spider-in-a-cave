#ifndef CAVE_H
#define CAVE_H

#include "../environment.hpp"
#include "cave_mesh.hpp"

class cave
{
private:
    cave_mesh CaveMesh;
public:
    cave();

    void initialize();

    void draw(environment_structure environment);
};

#endif // CAVE_H
