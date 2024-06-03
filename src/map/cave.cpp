#include "cave.hpp"

cave::cave()
{

}

void cave::initialize(){
    if(partition==NULL){
        partition = new collision_partition({1.2,1.19,1.3});   
    }

    CaveMesh.initialize(partition);
    cristal1.initialize();
    cristal1.scaling = 0.7;
    cristal1.translation = {-5.373450,-4.367498,-1.732364};
    cristal1.rotation = rotation_transform::from_quaternion({0.069329,0.216544,0.324752,0.918062}),
    cristal1.distance = 10;
    cristal1.addCollisions(partition);
    cristal1.update();


    cristal2.initialize();
    cristal2.scaling = 0.7;
    cristal2.translation = {1.731301,19.054394,-1.596459};
    cristal2.rotation = rotation_transform::from_quaternion({0.042079,-0.198239,-0.094838,0.974647}),
    cristal2.distance = 12;
    cristal2.intensity = 3;
    cristal2.addCollisions(partition);
    cristal2.update();


    collision_handler::initialize(partition);
}

void cave::draw(environment_structure &environment){
    environment.multiLight = true;
    environment.lights.push_back(cristal1.getLightParams());
    environment.lights.push_back(cristal2.getLightParams());
    CaveMesh.draw(environment);
    cristal1.draw(environment);
    cristal2.draw(environment);
}
