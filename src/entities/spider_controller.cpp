#include "spider_controller.hpp"




void SpiderController::initialize(spider* _ControlledSpider){
    ControlledSpider = _ControlledSpider;
    debug.sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.1f));
}
bool SpiderController::stick_to_ground(collision_object* colh){
    ControlledSpider->updateGlobal();
    bool allGood = true;
    for(int i=0;i<NUM_LEGS;i++){
        vec3 pos = ControlledSpider->getLegJoint(params.legs[i]);
        pos = pos + params.RestPositionDistance*(pos - ControlledSpider->translation);
        collision_ray ray(pos,-5*ControlledSpider->getUpVector());
        if(!colh->does_collide(&ray,pos)){
            allGood = false;
            legPositions[params.legs[i]] = ControlledSpider->getLegPosition(params.legs[i]);
        }
        else{
            legPositions[params.legs[i]] = pos;
            ControlledSpider->setLegPosition(params.legs[i],pos);
            debug.rays_collision_pos.push_back(pos);
        }
        debug.rays_to_draw.push_back(ray);
    }
    smoothHeight();
    return allGood;
}

void SpiderController::smoothHeight(){
    vec3 total_average={0,0,0};
    vec3 left_average={0,0,0};
    vec3 right_average={0,0,0};
    vec3 front_average={0,0,0};
    vec3 back_average={0,0,0};
    int leftC=0,rightC=0,frontC=0,backC=0;
    int count = 0;
    for(int i=0;i<NUM_LEGS;i++){
        count++;
        total_average += legPositions[params.legs[i]];
        if(i%2==0){
            left_average += legPositions[params.legs[i]];
            leftC++;
        }else{
            right_average += legPositions[params.legs[i]];
            rightC++;
        }

        if(i<=1){
            front_average += legPositions[params.legs[i]];
            frontC++;
        }
        else if(i>=NUM_LEGS-2){
            back_average += legPositions[params.legs[i]];
            backC++;
        }
    }
    back_average /= backC;
    front_average /= frontC;
    left_average /= leftC;
    right_average /= rightC;


    vec3 initialUp = {0,0,1};
    vec3 upVector = cross(front_average-back_average,right_average-left_average);
    std::cout << upVector << std::endl;
    if(norm(upVector)<=0.00001){
        std::cout << "ERROR" << std::endl;
        return;
    }
    upVector = normalize(upVector);
    quaternion q; // Quaternion of rotation
    vec3 temp = cross(initialUp, upVector);
    //q.xyz() = temp;
    q.x = temp.x;
    q.y = temp.y;
    q.z = temp.z;
    q.w = sqrt(pow(norm(initialUp),2) * pow(norm(upVector),2)) + dot(initialUp, upVector);
    q = normalize(q);

    ControlledSpider->set_rotation(q);

    if(count>0){
        total_average /= count;
        ControlledSpider->translation = total_average + upVector*params.BodyHeight;
        
    }
    collision_ray ray(ControlledSpider->translation,ControlledSpider->getUpVector());
    ray.color = {0.7,0,1};
    debug.rays_to_draw.push_back(ray);
}

void SpiderController::debug_draw(environment_structure environment){
    if(debug.debug_stick_to_ground){
        for(auto ray : debug.rays_to_draw){
        ray.draw(environment);
        }
        for(auto pos : debug.rays_collision_pos){
            debug.sphere.model.translation = pos;
            cgp::draw(debug.sphere,environment);
        }
    }
}

void SpiderController::update(){
    ControlledSpider->updateGlobal();
    for(int i=0;i<NUM_LEGS;i++){
        ControlledSpider->setLegPosition(params.legs[i],legPositions[params.legs[i]]);
    }
}