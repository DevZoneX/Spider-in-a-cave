#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP


#include "cgp/cgp.hpp"
#include "../environment.hpp"
#include "touchable_object.hpp"

struct partition_coordinates{
public:
    int x=0;
    int y=0;
    int z=0;

    bool operator==(partition_coordinates C){return (x==C.x && y==C.y && z==C.z);}
    bool operator!=(partition_coordinates C){return !operator==(C);}
    

};

std::ostream& operator<<(std::ostream &out, partition_coordinates C);

class collision_partition;

class collision_object
{
protected:
public:
    cgp::vec3 translation = {0,0,0};
    vec3 color = {0,1,0};
    cgp::rotation_transform rotation = cgp::rotation_axis_angle({0,0,1},0);
    float scaling = 1;

    bool changed = true;

    collision_object();
    virtual ~collision_object(){}

    virtual numarray<partition_coordinates> get_boxes(collision_partition* partition){if(partition==NULL){return {};}return {};};
    virtual bool is_partitionned(){return true;};
    virtual bool does_collide(collision_object* col2, vec3 &collision_point);
    virtual bool does_collide(collision_object* col2);

    virtual void draw(environment_structure environment);
};


class collision_sphere: public collision_object
{
private:
    int N_sub;
protected:
    int N_theta = 12;
    static cgp::mesh_drawable sphere;
    static bool sphere_initialized;
    cgp::curve_drawable sphere_positions;
public:
    ~collision_sphere(){};

    float r;

    collision_sphere(cgp::vec3 _center, float _r, int _N_sub=13);
    bool draw_full = false;

    bool is_in_box(cgp::vec3 start, cgp::vec3 end);
    bool does_collide(collision_object* col2, vec3 &collision_point);
    bool does_collide(collision_object* col2);
    void draw(environment_structure environment);
};

class collision_box: public collision_object
{
protected:
    static cgp::mesh_drawable cube;
    static bool cube_initialized;
    static cgp::curve_drawable cube_curve;

public:
    ~collision_box(){};
    cgp::vec3 axis1;
    cgp::vec3 axis2;
    cgp::vec3 axis3;

    /*cgp::vec3 translation;
    vec3 color = {0,0.7,0.7};
    cgp::rotation_transform rotation = cgp::rotation_axis_angle({0,0,1},0);*/
    cgp::vec3 scaling_xyz={1,1,1};

    collision_box(cgp::vec3 _start,cgp::vec3 _axis1, cgp::vec3 _axis2, cgp::vec3 _axis3);
    bool draw_full = false;

    bool is_in_box(cgp::vec3 start, cgp::vec3 end);
    numarray<partition_coordinates> get_boxes(collision_partition* partition);
    bool does_collide(collision_object* col2, vec3 &collision_point);
    bool does_collide(collision_object* col2);
    void draw(environment_structure environment);
};

class collision_ray: public collision_object{
protected:
    static bool ray_initialized;
    static cgp::curve_drawable ray_curve;
    static cgp::mesh_drawable ray_sphere;

public:
    cgp::vec3 director;
    collision_ray(cgp::vec3 _start, cgp::vec3 _director);
    bool is_in_box(cgp::vec3 start, cgp::vec3 end);
    numarray<partition_coordinates> get_boxes(collision_partition* partition);
    bool does_collide(collision_object* col2, vec3 &collision_point);
    bool does_collide(collision_object* col2);
    void draw(environment_structure environment);
};

class collision_triangle: public collision_object{
protected:
    static bool triange_initialized;
    static cgp::mesh_drawable triangle_mesh;

public:
    cgp::vec3 axis1;
    cgp::vec3 axis2;
    collision_triangle(cgp::vec3 _start, cgp::vec3 _axis1, cgp::vec3 _axis2);
    bool is_in_box(cgp::vec3 start, cgp::vec3 end);
    numarray<partition_coordinates> get_boxes(collision_partition* partition);
    bool does_collide(collision_object* col2, vec3 &collision_point);
    bool does_collide(collision_object* col2);
    void draw(environment_structure environment);
};

#endif // COLLISION_OBJECT_HPP
