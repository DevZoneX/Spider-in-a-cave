#include "spider_controller.hpp"




void SpiderController::initialize(spider* _ControlledSpider,timer_basic* _timer,input_devices& _inputs, window_structure& window){
    ControlledSpider = _ControlledSpider;
    camera_control.initialize(_inputs, window);
    inputs = &_inputs;
    timer = _timer;
    //camera_control.set_trapped_cursor(true);
    debug.sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.1f));
    old_t = timer->t;
    position = ControlledSpider->translation;
}
bool SpiderController::stick_to_ground(collision_object* col, bool reset){
    ControlledSpider->updateGlobal();
    debug.reset_stick();
    bool allGood = true;
    for(int i=0;i<NUM_LEGS;i++){
        vec3 pos = ControlledSpider->getLegJoint(params.legs[i]);
        pos = pos + params.RestPositionDistance*(pos - ControlledSpider->translation);
        collision_ray ray(pos,-5*ControlledSpider->getUpVector());
        if(!col->does_collide(&ray,pos)){
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
    smoothHeight(reset);
    position = ControlledSpider->translation;
    return allGood;
}

void SpiderController::smoothHeight(bool average){
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
        if(average){
            ControlledSpider->translation = total_average + upVector*params.BodyHeight;
        }
        else{
            vec3 target_pos = total_average + upVector*params.BodyHeight;
            vec3 diff = target_pos - ControlledSpider->translation;
            float value = dot(diff,ControlledSpider->getUpVector());
            ControlledSpider->translation += value * ControlledSpider->getUpVector();
        }
        
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
    timer->update();
    float dt = timer->t-old_t;

    int min_iter = dt/params.maxDt;
    float leftDt = dt - min_iter*params.maxDt;
    for(int i=0;i<min_iter;i++){
        handlePosition(params.maxDt);
        handleVelocity(params.maxDt);
    }
    handlePosition(leftDt);
    handleVelocity(leftDt);


    old_t = timer->t;
    ControlledSpider->translation = position;
    ControlledSpider->updateTranslation();
    ControlledSpider->updateRotation();
    ControlledSpider->updateGlobal();
    for(int i=0;i<NUM_LEGS;i++){
        ControlledSpider->setLegPosition(params.legs[i],legPositions[params.legs[i]]);
    }
    
}




// Controls handlers
void SpiderController::handleVelocity(float dt){
    vec3 diff = target_velocity - velocity;
    float value = norm(diff);
    if(value>0.00001){diff = normalize(diff);}
    if(value<params.acceleration*dt){
        velocity = target_velocity;
        return;
    }
    velocity = velocity + params.acceleration*diff*dt;
}
void SpiderController::handlePosition(float dt){
    position = position + dt*velocity;
}

// Controls input
void SpiderController::mouse_move_event(environment_structure &environment,input_devices& _inputs){
    if (!_inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);

    
}
void SpiderController::mouse_click_event(environment_structure &environment){
    camera_control.action_mouse_move(environment.camera_view);
}
void SpiderController::action_keyboard(environment_structure &environment){
    camera_control.action_keyboard(environment.camera_view);
}
void SpiderController::idle_frame(environment_structure &environment, collision_object* col){
    bool forward=false;
    bool backward=false;
    bool left=false;
    bool right=false;
    vec3 direction = {0,0,0};
    if(params.selected_keyboard==0){
        forward = inputs->keyboard.is_pressed('w');
        backward = inputs->keyboard.is_pressed('s');
        left = inputs->keyboard.is_pressed('a');
        right = inputs->keyboard.is_pressed('d');
    }
    else if(params.selected_keyboard==1){
        forward = inputs->keyboard.is_pressed('z');
        backward = inputs->keyboard.is_pressed('s');
        left = inputs->keyboard.is_pressed('q');
        right = inputs->keyboard.is_pressed('d');
    }
    else if(params.selected_keyboard==2){
        forward = inputs->keyboard.up;
        backward = inputs->keyboard.down;
        left = inputs->keyboard.left;
        right = inputs->keyboard.right;
    }

    if(forward){direction+=ControlledSpider->getFrontVector();}
    if(backward){direction-=ControlledSpider->getFrontVector();}
    if(right){direction+=ControlledSpider->getRightVector();}
    if(left){direction-=ControlledSpider->getRightVector();}

    if(norm(direction)!=0){direction = normalize(direction);}
    target_velocity = params.maxSpeed * direction;
    

    vec3 center = ControlledSpider->translation;
    camera_control.camera_model.distance_to_center = params.camera_max_distance;
    vec3 director = camera_control.camera_model.position()-center;
    if(norm(director)>0.00001){
        director = normalize(director); 
    }
    else{
        director = {0,1,0};
    }
    collision_ray ray(center,params.camera_max_distance*director);
    vec3 temp;
    if(col->does_collide(&ray,temp)){
        float dist = norm(temp-center);
        camera_control.camera_model.distance_to_center = dist*0.95;
    }
    camera_control.camera_model.look_at(camera_control.camera_model.position(), center, ControlledSpider->getUpVector());
    camera_control.idle_frame(environment.camera_view);
}










// ___________________________________________________________  //
// ___________________________________________________________  //
// ___________________________________________________________  //
// ___________________________________________________________  //



void SpiderController::debug::reset_stick(){
    rays_to_draw.clear();
    rays_collision_pos.clear();
}