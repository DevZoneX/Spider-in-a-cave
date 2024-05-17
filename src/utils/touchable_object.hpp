#ifndef TOUCHABLE_OBJECT_HPP
#define TOUCHABLE_OBJECT_HPP


#include "../environment.hpp"
#include "collision_object.hpp"
#include "math.hpp"
#include "cgp/cgp.hpp"

class collision_object;
class partition_coordinates;

class collision_partition{
private:
    float x_length=2;
    float y_length=2;
    float z_length=2;

    vec3 center = {0,0,0};

    int N_x=10;
    int N_y=10;
    int N_z=10;

    std::vector<collision_object*> *collision_list_partition = NULL;
    std::vector<collision_object*> out_collisions;

    cgp::mesh_drawable partition_cube;
public:
    collision_partition(vec3 partition_length = {2,2,2}, vec3 _center={0,0,0},vec3 terrain_length = {-1,-1,-1});
    ~collision_partition();

    vec3 color = {0.5,0.4,0.3};

    vec3 get_center(){return center;}
    float get_x_length(){return x_length;}
    float get_y_length(){return y_length;}
    float get_z_length(){return z_length;}
    int get_N_x(){return N_x;}
    int get_N_y(){return N_y;}
    int get_N_z(){return N_z;}

    bool which_partition(cgp::vec3 coords, partition_coordinates &C); // returns true if the coordinate is inside the terrain length
    std::vector<collision_object*> get_partition(partition_coordinates C);
    std::vector<collision_object*> get_partition(int idx){
        if(idx<0){return out_collisions;}
        return collision_list_partition[idx];
    }

    void add_collision(collision_object* col);
    vec3 get_partition_coordinates(partition_coordinates C);
    partition_coordinates get_out_coordinates();
    math::parallelogram get_partition_face(partition_coordinates C, math::cube_face face);

   

    
    void draw(partition_coordinates C, environment_structure environment);
};

std::ostream& operator<<(std::ostream &strm,collision_partition &colpar);



class touchable_object
{
private:
    collision_object* c_boxes = NULL;
public:
    touchable_object();
    ~touchable_object();

    void initialize(){};
    void draw(environment_structure environment){if(environment.background_color.x){return;}};
    virtual bool goes_collide(collision_object *col){if(col==NULL){return false;}return false;};
};

#endif // TOUCHABLE_OBJECT_HPP
