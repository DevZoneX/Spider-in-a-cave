#include "collision_handler.hpp"


using namespace cgp;



collision_handler::~collision_handler(){
    /*if(partition!=NULL){
        delete partition;
    }*/
}


void collision_handler::initialize(collision_partition *_partition){
    if(partition==NULL){return;}
    initialized=true;
    partition=_partition;
}



bool collision_handler::does_collide(collision_object* col2, vec3 &collision_point){
    cgp::numarray<partition_coordinates> coords = col2->get_boxes(partition);
    std::vector<collision_object*> near_objects;

    
    for(auto coord : coords){
        std::vector<collision_object*> temp_objects = partition->get_partition(coord);
        std::vector<collision_object*> extend;
        for(auto object : temp_objects){
            bool already = false;
            for(auto already_objects:near_objects){
                if(already_objects==object){
                    already=true;
                    break;
                }
            }
            if(!already){
                extend.push_back(object);
            }
        }
        near_objects.insert(near_objects.end(),extend.begin(),extend.end());
    }


    if (dynamic_cast<collision_ray*>(col2) != nullptr){
        collision_ray* ray = dynamic_cast<collision_ray*>(col2);

        float min_dist = -1;
        vec3 result;
        vec3 temp;
        for(collision_object* col: near_objects){
            if(ray->does_collide(col, temp)){
                float dist = norm(ray->translation-temp);
                if(min_dist==-1 || min_dist>dist){
                    min_dist=dist;
                    result=temp;
                }
            }
        }
        if(min_dist>=0){
            collision_point=result;
            return true;
        }
        return false;
    }

    return false;
}

bool collision_handler::does_collide(collision_object* col2){
    vec3 temp;
    return does_collide(col2, temp);
}
