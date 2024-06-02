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
    cristal1.distance = 10;
    cristal1.addCollisions(partition);
    cristal2.initialize();
    cristal2.translation = {1,6,-3.1};
    cristal2.rotation = rotation_transform::from_axis_angle({1,0,0},0.3);
    cristal2.addCollisions(partition);
    cristal1.update();
    cristal2.update();
    cristal3.initialize();
    cristal3.scaling = 0.7;
    cristal3.translation = {0,3,0};
    cristal3.distance = 10;
    cristal3.addCollisions(partition);
    cristal3.update();

    collision_handler::initialize(partition);
}

void cave::draw(environment_structure &environment){
    environment.multiLight = true;
    environment.lights.push_back(cristal1.getLightParams());
    environment.lights.push_back(cristal2.getLightParams());
    CaveMesh.draw(environment);
    cristal1.draw(environment);
    cristal2.draw(environment);
    cristal3.draw(environment);
}
