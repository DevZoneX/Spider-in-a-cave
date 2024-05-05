#include "touchable_object.hpp"



collision_partition::collision_partition(vec3 partition_length, vec3 _center,vec3 terrain_length){
    x_length = partition_length.x;
    y_length = partition_length.y;
    z_length = partition_length.z;
    center = _center;
    if(terrain_length.x!=-1){N_x = terrain_length.x/x_length + 1;}
    if(terrain_length.y!=-1){N_y = terrain_length.y/y_length + 1;}
    if(terrain_length.z!=-1){N_z = terrain_length.z/z_length + 1;}
    collision_list_partition = new std::vector<collision_object*>[8*N_x*N_y*N_z+1];
    partition_cube.initialize_data_on_gpu(mesh_primitive_cubic_grid({0,0,0},{x_length,0,0},{x_length,y_length,0},{0,y_length,0},{0,0,z_length},{x_length,0,z_length},{x_length,y_length,z_length},{0,y_length,z_length}));
}
collision_partition::~collision_partition(){
    delete [] collision_list_partition;
}
bool collision_partition::which_partition(cgp::vec3 coords, partition_coordinates &C){
    vec3 new_coords = coords-center;
    int X,Y,Z;
    X = floor(new_coords.x/x_length);
    Y = floor(new_coords.y/y_length);
    Z = floor(new_coords.z/z_length);
    if(X>=N_x || X<-N_x){return false;}
    if(Y>=N_y || Y<-N_y){return false;}
    if(Z>=N_z || Z<-N_z){return false;}
    C.x=X;
    C.y=Y;
    C.z=Z;
    return true;
}
std::vector<collision_object*> collision_partition::get_partition(partition_coordinates C){
    int x=C.x;
    int y=C.y;
    int z=C.z;
    if(x>=-N_x && x<N_x && y>=-N_y && y<N_y && z>=-N_z && z<N_z){
        return collision_list_partition[(x+N_x)*4*N_y*N_z+(y+N_y)*2*N_y+(z+N_z)];
    }
    return out_collisions;
}
void collision_partition::add_collision(collision_object* col){
    numarray<partition_coordinates> Cs = col->get_boxes(this);
    for(int i=0;i<Cs.size();i++){
        int x=Cs[i].x;
        int y=Cs[i].y;
        int z=Cs[i].z;
        if(x>=-N_x && x<N_x && y>=-N_y && y<N_y && z>=-N_z && z<N_z){
            collision_list_partition[(x+N_x)*4*N_y*N_z+(y+N_y)*2*N_y+(z+N_z)].push_back(col);
        }
        else{
            out_collisions.push_back(col);
        }
    }
}
partition_coordinates collision_partition::get_out_coordinates(){return (partition_coordinates){N_x,N_y,N_z};}
vec3 collision_partition::get_partition_coordinates(partition_coordinates C){
    return {x_length*C.x,y_length*C.y,z_length*C.z};
}
void collision_partition::draw(partition_coordinates C, environment_structure environment){
    partition_cube.model.translation = get_partition_coordinates(C);
    partition_cube.material.color = color;
    cgp::draw(partition_cube, environment);
}
math::parallelogram collision_partition::get_partition_face(partition_coordinates C, math::cube_face face){
    vec3 start = get_partition_coordinates(C);
    vec3 axisx = {x_length,0,0};
    vec3 axisy = {0,y_length,0};
    vec3 axisz = {0,0,z_length};
    if(face==math::BACK_FACE){
        return math::parallelogram(start,axisy,axisz);
    }
    else if(face==math::FRONT_FACE){
        return math::parallelogram(start+axisx,axisy,axisz);
    }
    else if(face==math::RIGHT_FACE){
        return math::parallelogram(start,axisx,axisz);
    }
    else if(face==math::LEFT_FACE){
        return math::parallelogram(start+axisy,axisx,axisz);
    }
    else if(face==math::UP_FACE){
        return math::parallelogram(start+axisz,axisx,axisy);
    }
    else if(face==math::DOWN_FACE){
        return math::parallelogram(start,axisx,axisy);
    }
    return math::parallelogram(start,axisx,axisy);
}


touchable_object::touchable_object()
{

}

touchable_object::~touchable_object(){
    if(c_boxes!=NULL){
        delete[] c_boxes;
    }
}


