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

