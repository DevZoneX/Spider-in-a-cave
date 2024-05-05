#include "fabric.hpp"

fabric::fabric(){

}
fabric::~fabric(){
    delete[] positions;
    delete[] lengths;
}

void fabric::initialize(float *_lengths, cgp::vec2 *_positions,int _bone_number){
    lengths = _lengths;
    positions = _positions;
    bone_number = _bone_number;

    angles = new float[_bone_number];
    for(int i=0;i<_bone_number;i++){
        angles[i] = 0;
    }
    initialized=true;
}

void fabric::initialize(float *_lengths,int _bone_number){
    cgp::vec2 *_positions = new cgp::vec2[_bone_number+1];

    float sofar=0;
    for(int i=0;i<_bone_number+1;i++){
        _positions[i] = {sofar,0};
        sofar += _lengths[i];
    }

    initialize(_lengths,_positions,_bone_number);
}

void fabric::calculate(cgp::vec3 start, cgp::vec3 target, cgp::rotation_transform rotation, bool debug){
    float sofar=0;
    mat3 rotMat = rotation.matrix();
    for(int i=0;i<bone_number+1;i++){
        positions[i] = {0.1*i*i,sofar};
        sofar += lengths[i];
    }

    assert_cgp(initialized, "Fabric not initialized");
    if(start.x==last_first.x && start.y==last_first.y && start.z==last_first.z && target.x==last_last.x && target.y==last_last.y && target.z==last_last.z){
        return;
    }
    if(debug){
        std::cout << "Calculate : " << target << std::endl;
    }

    vec3 normal = rotMat * (vec3){0,1,0};
    vec3 up = rotMat * (vec3){0,0,1};
    vec3 front = rotMat * (vec3){1,0,0};

    vec3 directed = target-start;
    //directed = rotMatInv * directed;
    //float y = directed.z;
    float y = dot(up,directed);
    //float x = norm((vec2){directed.x,directed.y});
    float x = norm(directed-y*up);

    vec2 directionProj = {dot(directed,normal),dot(directed,front)};
    z_angle=Pi/2;
    if(directionProj.y<0){z_angle=-Pi/2;}
    if(directionProj.x!=0){
        z_angle = atan(directionProj.y/directionProj.x);
    }
    if(directionProj.x<0){z_angle+=Pi;}
    z_angle *= -1;

    /*float angleDir = Pi/2;
    if(directed.x!=0){
        angleDir = atan(directed.y/directed.x);
    }
    if(directed.x<0){
        angleDir += Pi;
    }
    if(angleDir<0){angleDir+=2*Pi;}
    float angleNormal = Pi/2;
    if(normal.x!=0){
        angleNormal = atan(normal.y/normal.x);
    }
    if(normal.x<0){
        angleNormal += Pi;
    }
    if(angleNormal<0){angleNormal+=2*Pi;}
    z_angle = -angleNormal+angleDir;*/


    //std::cout << "AngleDir:"<<((int)(angleDir*180/Pi))<<" | AngleNormal:"<<((int)(angleNormal*180/Pi))<<std::endl;



    if(directed.y<0){

    }

    vec2 current = {x,y};

    vec2 temp;

    for(int i=0;i<iterations;i++){
        current = {x,y};
        for(int j=bone_number-1;j>=0;j--){
            positions[j+1]=current;
            temp = positions[j+1]-positions[j];
            temp = normalize(temp);
            //std::cout << "bone n:" << j << " length: " << lengths[j] << std::endl;
            current = positions[j+1]-temp*lengths[j];
        }
        positions[0]=current;
        current = {0,0};
        for(int j=0;j<bone_number;j++){
            positions[j]=current;
            temp = positions[j+1]-positions[j];
            temp = normalize(temp);
            current = positions[j]+temp*lengths[j];
        }
        positions[bone_number] = current;
    }

    float total_angle = 0;
    for(int j=0;j<bone_number;j++){
        temp = positions[j+1]-positions[j];
        y = temp.y;
        x = temp.x;
        if(x==0){
            angles[j] = Pi/2-total_angle;
            total_angle = Pi/2;
        }
        else{
            if(x>=0){
                angles[j] = atan(y/x) - total_angle;
            }
            else{
                angles[j] = Pi + atan(y/x) - total_angle;
            }
            total_angle += angles[j];
        }
    }
    //angles[0] -= Pi/2;
    last_first = start;
    last_last = target;
}



vec3 fabric::getLast(){
    assert_cgp(initialized, "Fabric not initialized");
    return last_last;
}

float fabric::getBoneAngle(int bone){
    assert_cgp(initialized, "Fabric not initialized");
    assert_cgp(bone>=-1, "Bone given is lower than -1");
    assert_cgp(bone<bone_number, "Bone given is out of bound");
    if(bone==-1){
        return z_angle;
    }
    return angles[bone];
}
