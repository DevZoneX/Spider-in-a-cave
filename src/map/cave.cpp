#include "cave.hpp"

cave::cave()
{

}

void cave::initialize(){
    if(partition==NULL){
        partition = new collision_partition({1.2,1.2,1.3});   
    }

    CaveMesh.initialize(partition);
    cristal1.initialize();
    cristal1.scaling = 0.7;
    cristal1.translation = {2,0,0};
    cristal1.update();

    collision_handler::initialize(partition);
}

void cave::draw(environment_structure &environment){
    environment.multiLight = true;
    environment.lights.push_back(cristal1.getLightParams());
    CaveMesh.draw(environment);
    cristal1.draw(environment);
}
