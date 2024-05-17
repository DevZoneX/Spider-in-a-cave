#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP


#include "cgp/cgp.hpp"
//#include "../environment.hpp"
#include "touchable_object.hpp"
#include "collision_object.hpp"

class collision_handler: public collision_object{

protected:
    collision_partition *partition=NULL;
    bool initialized=false;

public:
    bool partitionned=false;

    collision_handler(){};
    ~collision_handler();

    void initialize(collision_partition *_partition);
    bool is_partitionned() override {return partitionned;};
    bool does_collide(collision_object* col2, vec3 &collision_point);
    bool does_collide(collision_object* col2);
};

#endif // COLLISION_HANDLER_HPP
