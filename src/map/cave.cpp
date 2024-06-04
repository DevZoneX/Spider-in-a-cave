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
    cristal1.update();
    cristal1.addCollisions(partition);


    cristal2.initialize();
    cristal2.scaling = 0.7;
    cristal2.translation = {1.731301,19.054394,-1.596459};
    cristal2.rotation = rotation_transform::from_quaternion({0.042079,-0.198239,-0.094838,0.974647}),
    cristal2.distance = 12;
    cristal2.intensity = 3;
    cristal2.update();
    cristal2.addCollisions(partition);

    cristal3.initialize();
    cristal3.translation = {6.399254,-1.817590,2.995139};
    cristal3.rotation = rotation_transform::from_quaternion({-0.016534,-0.087254,0.223681,0.970608}),
    cristal3.distance = 12;
    cristal3.intensity = 3;
    cristal3.update();
    cristal3.addCollisions(partition);

    cristal4.initialize();
    cristal4.scaling = 1.4;
    cristal4.translation = {5.926202,-17.780735,1.404731};
    cristal4.rotation = rotation_transform::from_quaternion({-0.408144,-0.132138,0.601376,0.674022}),
    cristal4.distance = 12;
    cristal4.intensity = 3.5;
    cristal4.update();
    cristal4.addCollisions(partition);


    collision_handler::initialize(partition);
}

void cave::draw(environment_structure &environment){
    environment.multiLight = true;
    environment.lights.push_back(cristal1.getLightParams());
    environment.lights.push_back(cristal2.getLightParams());
    light_params cristal3_light = cristal3.getLightParams();
    cristal3_light.position += {-1,0,0};
    environment.lights.push_back(cristal3_light);
    environment.lights.push_back(cristal4.getLightParams());
    CaveMesh.draw(environment);
    cristal1.draw(environment);
    cristal2.draw(environment);
    cristal3.draw(environment);
    cristal4.draw(environment);
}
