#include "collision_object.hpp"

#include "math.hpp"


using namespace cgp;



numarray<partition_coordinates> get_segment_boxes(math::segment segment, collision_partition *partition){
    partition_coordinates C1;
    partition_coordinates C2;
    if(!partition->which_partition(segment.start,C1)){
        return { partition->get_out_coordinates() };
    }
    if(!partition->which_partition(segment.start+segment.director,C2)){
        return { partition->get_out_coordinates() };
    }
    partition_coordinates C = C1;
    partition_coordinates newCValue;
    numarray<partition_coordinates> toReturn = { C };
    while(C!=C2){
        vec3 temp1, temp2, temp3;
        if(C2.x-C.x!=0){
            if(math::parallelogram_segment_intersection(partition->get_partition_face(C, (C2.x-C.x>0) ? math::FRONT_FACE : math::BACK_FACE),segment,temp1)){
                newCValue = C;
                newCValue.x = newCValue.x + ((C2.x-C.x>0) ? 1 : -1);
                toReturn.push_back(newCValue);
            }
        }
        if(C2.y-C.y!=0){
            if(math::parallelogram_segment_intersection(partition->get_partition_face(C, (C2.y-C.y>0) ? math::LEFT_FACE : math::RIGHT_FACE),segment,temp2)){
                newCValue = C;
                newCValue.y = newCValue.y + ((C2.y-C.y>0) ? 1 : -1);
                toReturn.push_back(newCValue);
            }
        }
        if(C2.z-C.z!=0){
            if(math::parallelogram_segment_intersection(partition->get_partition_face(C, (C2.z-C.z>0) ? math::UP_FACE : math::DOWN_FACE),segment,temp3)){
                newCValue = C;
                newCValue.z = newCValue.z + ((C2.z-C.z>0) ? 1 : -1);
                toReturn.push_back(newCValue);
            }
        }
        if(C==newCValue){
            std::cout << "C ERROR" << C << " " << C2 << std::endl;
            toReturn.push_back(C2);
            return toReturn;
        }
        C = newCValue;
    }
    return toReturn;
}



std::ostream& operator<<(std::ostream &out, partition_coordinates C){
        out << "[" << C.x << "," << C.y << "," << C.z << "]";
        return out;
    }

collision_object::collision_object(){}
bool collision_object::does_collide(collision_object* col2, vec3 &collision_point){if(col2!=NULL && collision_point.x==0){return false;}return false;}
bool collision_object::does_collide(collision_object* col2){if(col2!=NULL){return false;}return false;}
void collision_object::draw(environment_structure environment){cgp::draw(mesh_drawable(),environment);}




bool collision_sphere::sphere_initialized = false;
cgp::mesh_drawable collision_sphere::sphere;
collision_sphere::collision_sphere(cgp::vec3 _center, float _r, int _N_sub){
    N_sub = _N_sub;
    if(!sphere_initialized){
        sphere.initialize_data_on_gpu(mesh_primitive_sphere(1));
        sphere.material.phong.specular = 0;
        sphere_initialized=true;
    }
    numarray<vec3> positions;
    positions.resize(N_sub);
    sphere_positions.initialize_data_on_gpu(positions);
    translation = _center;
    r = _r;
}
bool collision_sphere::is_in_box(cgp::vec3 start, cgp::vec3 end){if(start.x==end.x){return false;}return false;}
bool collision_sphere::does_collide(collision_object* col2, vec3 &collision_point){
    if (dynamic_cast<collision_sphere*>(col2) != nullptr){
        collision_sphere* sphere2 = dynamic_cast<collision_sphere*>(col2);
        if(norm(translation-sphere2->translation)<r*scaling+sphere2->r*sphere2->scaling){
            collision_point = (translation*sphere2->scaling*sphere2->r+sphere2->translation*r*scaling)/(sphere2->scaling*sphere2->r+scaling*r);
            return true;
        }
    }
    else if (dynamic_cast<collision_ray*>(col2) != nullptr){
        collision_ray* ray = dynamic_cast<collision_ray*>(col2);
        return ray->does_collide(this,collision_point);
    }
    return false;
}
bool collision_sphere::does_collide(collision_object* col2){
    vec3 vector;
    return does_collide(col2, vector);
}
void collision_sphere::draw(environment_structure environment){
    if(draw_full){
        sphere.model.scaling = r*scaling;
        sphere.model.translation = translation;
        sphere.material.color = color;
        cgp::draw(sphere,environment);
    }
    else{
        numarray<vec3> positions;
        positions.resize(N_sub);
        sphere_positions.color = color;
        float angleZ = 0;
        float anglePhi = 0;
        vec3 up = {0,0,1};
        for(int i=0;i<N_theta;i++){
            angleZ = (double)i/(double)N_theta*2*Pi;
            vec3 rotated = {cos(angleZ),sin(angleZ),0};
            for(int j=0;j<N_sub;j++){
                anglePhi = (double)j/(double)(N_sub-1)*Pi;
                positions[j] = sin(anglePhi)*rotated+cos(anglePhi)*up;
                positions[j] = translation + r*scaling*positions[j];
            }
            sphere_positions.vbo_position.update(positions);
            cgp::draw(sphere_positions,environment);
        }
    }
}


cgp::mesh_drawable collision_box::cube;
bool collision_box::cube_initialized=false;
cgp::curve_drawable collision_box::cube_curve;
collision_box::collision_box(vec3 _start,vec3 _axis1, vec3 _axis2, vec3 _axis3){
    translation = _start;
    axis1 = _axis1;
    axis2 = _axis2;
    axis3 = _axis3;
    if(!cube_initialized){
        cube.initialize_data_on_gpu(mesh_primitive_cubic_grid({0,0,0},axis1,axis1+axis2,axis2,axis3,axis1+axis3,axis1+axis2+axis3,axis2+axis3));
        cube.material.phong.specular = 0;
        numarray<vec3> positions;
        positions.resize(2);
        cube_curve.initialize_data_on_gpu(positions);
        cube_initialized=true;
    }
}
numarray<partition_coordinates> collision_box::get_boxes(collision_partition* partition){
    numarray<partition_coordinates> numarrarr[12];
    vec3 axis1b = scaling_xyz.x * scaling * axis1;
    vec3 axis2b = scaling_xyz.y * scaling * axis2;
    vec3 axis3b = scaling_xyz.z * scaling * axis3;
    numarrarr[0] = get_segment_boxes(math::segment(translation,axis1b), partition);
    numarrarr[1] = get_segment_boxes(math::segment(translation,axis2b), partition);
    numarrarr[2] = get_segment_boxes(math::segment(translation,axis3b), partition);
    numarrarr[3] = get_segment_boxes(math::segment(translation+axis1b,axis2b), partition);
    numarrarr[4] = get_segment_boxes(math::segment(translation+axis1b,axis3b), partition);
    numarrarr[5] = get_segment_boxes(math::segment(translation+axis2b,axis1b), partition);
    numarrarr[6] = get_segment_boxes(math::segment(translation+axis2b,axis3b), partition);
    numarrarr[7] = get_segment_boxes(math::segment(translation+axis3b,axis2b), partition);
    numarrarr[8] = get_segment_boxes(math::segment(translation+axis3b,axis1b), partition);
    numarrarr[9] = get_segment_boxes(math::segment(translation+axis1b+axis3b,axis2b), partition);
    numarrarr[10] = get_segment_boxes(math::segment(translation+axis1b+axis2b,axis3b), partition);
    numarrarr[11] = get_segment_boxes(math::segment(translation+axis3b+axis2b,axis1b), partition);

    numarray<partition_coordinates> FinalVec;

    
    for(int i=0;i<12;i++){
        std::vector<partition_coordinates>::iterator iter;
        for(iter=numarrarr[i].begin(); iter < numarrarr[i].end(); iter++){
            std::vector<partition_coordinates>::iterator iterFinal;
            partition_coordinates C = *iter;
            bool present = false;
            for(iterFinal=FinalVec.begin(); iterFinal < FinalVec.end(); iterFinal++){
                if(*iterFinal==C){
                    present=true;
                    break;
                }
            }
            if(!present){
                FinalVec.push_back(C);
            }

        }
    }
    return FinalVec;
}
bool collision_box::does_collide(collision_object* col2, vec3 &collision_point){
    if (dynamic_cast<collision_box*>(col2) != nullptr){
        collision_box* box_other = dynamic_cast<collision_box*>(col2);
        collision_box* box1 = box_other;
        collision_box* box2 = this;

        

        int pointCounter = 0;
        vec3 meanVect = {0,0,0};
        vec3 tempVect = {0,0,0};

        bool collision = false;

        for(int i=0;i<2;i++){
            rotation_transform rot1 = box1->rotation;
            vec3 scale1 = box1->scaling_xyz*box1->scaling;
            rotation_transform rot2 = box2->rotation;
            vec3 scale2 = box2->scaling_xyz*box1->scaling;
            vec3 axis_1[3] = {rot1*scale1.x*box1->axis1,rot1*scale1.y*box1->axis2,rot1*scale1.z*box1->axis3};
            vec3 axis_2[3] = {rot2*scale2.x*box2->axis1,rot2*scale2.y*box2->axis2,rot2*scale2.z*box2->axis3};
            vec3 start1 = box1->translation;
            vec3 start2 = box2->translation;

            // All thee first box segments
            math::segment segments[12] = {math::segment(start1,axis_1[0]),
                math::segment(start1,axis_1[1]),math::segment(start1,axis_1[2]),
                math::segment(start1+axis_1[0],axis_1[1]),math::segment(start1+axis_1[0],axis_1[2]),
                math::segment(start1+axis_1[1],axis_1[0]),math::segment(start1+axis_1[1],axis_1[2]),
                math::segment(start1+axis_1[2],axis_1[0]),math::segment(start1+axis_1[2],axis_1[1]),
                math::segment(start1+axis_1[0]+axis_1[1],axis_1[2]),
                math::segment(start1+axis_1[0]+axis_1[2],axis_1[1]),
                math::segment(start1+axis_1[1]+axis_1[2],axis_1[0])
            };
            // All the second box parallelograms
            math::parallelogram parals[6] = {
                math::parallelogram(start2,axis_2[0],axis_2[1]),
                math::parallelogram(start2,axis_2[0],axis_2[2]),
                math::parallelogram(start2,axis_2[1],axis_2[2]),
                math::parallelogram(start2+axis_2[2],axis_2[0],axis_2[1]),
                math::parallelogram(start2+axis_2[1],axis_2[0],axis_2[2]),
                math::parallelogram(start2+axis_2[0],axis_2[1],axis_2[2]),
            };

            for(int s=0;s<12;s++){
                for(int p=0;p<6;p++){
                    if(math::parallelogram_segment_intersection(parals[p],segments[s],tempVect)){
                        pointCounter++;
                        //math::draw(parals[p],environment);
                        //math::draw(segments[s],environment);
                        cube.model.scaling = 0.1f;
                        cube.model.translation = tempVect;
                        meanVect += tempVect;
                        collision = true;
                    }
                }
            }
            box1 = this;
            box2 = box_other;
        }

        if(collision){
            collision_point = meanVect/(double)pointCounter;
        }
        return collision;
    }
    else if (dynamic_cast<collision_ray*>(col2) != nullptr){
        collision_ray* ray = dynamic_cast<collision_ray*>(col2);
        return ray->does_collide(this,collision_point);
    }
    return false;
}
bool collision_box::does_collide(collision_object* col2){
    vec3 vector;
    return does_collide(col2, vector);
}
void collision_box::draw(environment_structure environment){
    if(draw_full){
        cube.model.translation = translation;
        cube.model.scaling_xyz = scaling_xyz*scaling;
        cube.model.rotation = rotation;
        cgp::draw(cube,environment);
    }
    else{
        vec3 _axis1 = axis1*scaling_xyz*scaling;
        vec3 _axis2 = axis2*scaling_xyz*scaling;
        vec3 _axis3 = axis3*scaling_xyz*scaling;
        cube_curve.color = color;
        numarray<vec3> positions;
        positions.resize(2);
        positions[0]=translation;
        positions[1]=translation+rotation*_axis1;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*_axis2;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*_axis3;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[0]=translation+rotation*(_axis1+_axis2);
        positions[1]=translation+rotation*_axis1;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*_axis2;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*(_axis1+_axis2+_axis3);
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[0]=translation+rotation*(_axis1+_axis3);
        positions[1]=translation+rotation*_axis1;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*_axis3;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*(_axis1+_axis2+_axis3);
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[0]=translation+rotation*(_axis2+_axis3);
        positions[1]=translation+rotation*_axis2;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*_axis3;
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
        positions[1]=translation+rotation*(_axis1+_axis2+_axis3);
        cube_curve.vbo_position.update(positions);
        cgp::draw(cube_curve,environment);
    }
}


bool collision_ray::ray_initialized;
cgp::curve_drawable collision_ray::ray_curve;
cgp::mesh_drawable collision_ray::ray_sphere;
collision_ray::collision_ray(vec3 _start, vec3 _director){
    translation=_start;
    director=_director;
    if(!ray_initialized){
        numarray<vec3> positions;
        positions.resize(2);
        ray_curve.initialize_data_on_gpu(positions);
        ray_sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.05f));
        ray_initialized=true;
    }
}
numarray<partition_coordinates> collision_ray::get_boxes(collision_partition* partition){
    math::segment ray_segment = math::segment(translation,director);
    return get_segment_boxes(ray_segment, partition);
}
bool collision_ray::does_collide(collision_object* col2, vec3 &collision_point){
    if (dynamic_cast<collision_box*>(col2) != nullptr){
        collision_box* box_other = dynamic_cast<collision_box*>(col2);

        math::segment seg(translation,director);
        math::parallelogram paral({0,0,0},{1,0,0},{0,1,0});

        rotation_transform rot1 = box_other->rotation;
        vec3 scale1 = box_other->scaling_xyz;
        vec3 axis_1[3] = {rot1*scale1.x*box_other->axis1,rot1*scale1.y*box_other->axis2,rot1*scale1.z*box_other->axis3};
        vec3 start1 = box_other->translation;
        math::parallelogram parals[6] = {
                math::parallelogram(start1,axis_1[0],axis_1[1]),
                math::parallelogram(start1,axis_1[0],axis_1[2]),
                math::parallelogram(start1,axis_1[1],axis_1[2]),
                math::parallelogram(start1+axis_1[2],axis_1[0],axis_1[1]),
                math::parallelogram(start1+axis_1[1],axis_1[0],axis_1[2]),
                math::parallelogram(start1+axis_1[0],axis_1[1],axis_1[2]),
            };

        vec3 tempVect = {0,0,0};
        vec3 minVect = {0,0,0};
        float min_offset = -1;
        float offset;

        bool collision = false;

        for(int p=0;p<6;p++){
            if(math::parallelogram_segment_intersection(parals[p],seg,tempVect)){
                offset = dot(tempVect-translation,director);
                collision = true;
                if(min_offset==-1 || min_offset>offset){
                    minVect = tempVect;
                    min_offset = offset;
                }
            }
        }

        if(collision){
            collision_point = minVect;
        }
        return collision;

    }
    else if (dynamic_cast<collision_sphere*>(col2) != nullptr){
        collision_sphere* sphere = dynamic_cast<collision_sphere*>(col2);

        vec3 nonorth_diff = sphere->translation-translation;
        vec3 normalized_director = director/norm(director);

        float coeff = dot(nonorth_diff,normalized_director);

        vec3 orth_diff = nonorth_diff - coeff*normalized_director;
        vec3 projection = sphere->translation - orth_diff;
        if(norm(orth_diff)>sphere->r*sphere->scaling){return false;}
        math::segment seg = math::segment(translation,scaling*director);
        vec3 candidate = projection-sqrt(pow(sphere->r*sphere->scaling,2)-pow(norm(orth_diff),2))*normalized_director;
        if(seg.evaluate(candidate)){
            collision_point = candidate;
            return true;
        }
        candidate = projection+sqrt(pow(sphere->r*sphere->scaling,2)-pow(norm(orth_diff),2))*normalized_director;
        if(seg.evaluate(candidate)){
            collision_point = candidate;
            return true;
        }
        return false;
    }
    else if (dynamic_cast<collision_triangle*>(col2) != nullptr){
        collision_triangle* triangle = dynamic_cast<collision_triangle*>(col2);
        math::plane triangle_plane = math::plane(triangle->translation,triangle->axis1,triangle->axis2);
        math::line segment_line = math::line(translation,director);
        math::segment segment = math::segment(translation,director);
        vec3 temp;
        if(!math::plane_line_intersection(triangle_plane,segment_line,temp)){return false;}
        if(!segment.evaluate(temp)){return false;}
        collision_point = temp;
        float c1 = (temp-triangle->translation).x;
        float c2 = (temp-triangle->translation).y;
        vec2 vector1 = {triangle->axis1.x, triangle->axis1.y};
        vec2 vector2 = {triangle->axis2.x, triangle->axis2.y};
        if(triangle->axis1.x==triangle->axis2.x){
            vector1.x = triangle->axis1.z;
            vector2.x = triangle->axis2.z;
            c1 = (temp-triangle->translation).z;
        }
        else if(triangle->axis1.y==triangle->axis2.y){
            vector1.y = triangle->axis1.z;
            vector2.y = triangle->axis2.z;
            c2 = (temp-triangle->translation).z;
        }
        float D = math::determinant2D(vector1,vector2);
        if( cgp::abs(D) <= 1e-06 ){return false;}
        float D1 = math::determinant2D({c1 ,c2},vector2);
        float D2 = math::determinant2D(vector1,{c1 ,c2});
        float l1 = D1/D;
        float l2 = D2/D;
        if(l1>=0 && l2>=0 && l1+l2<=1){
            collision_point = temp;
            return true;
        }
        return false;
    }
    return false;
}
bool collision_ray::does_collide(collision_object* col2){
    vec3 temp;
    return does_collide(col2, temp);
}
void collision_ray::draw(environment_structure environment){
    numarray<vec3> positions;
    positions.resize(2);
    positions[0] = translation;
    positions[1] = translation+scaling*director;
    ray_curve.vbo_position.update(positions);
    ray_curve.color = color;
    cgp::draw(ray_curve,environment);
    ray_sphere.model.translation = translation;
    ray_sphere.material.color = color;
    cgp::draw(ray_sphere,environment);
}



bool collision_triangle::triange_initialized;
mesh_drawable collision_triangle::triangle_mesh;
collision_triangle::collision_triangle(cgp::vec3 _start, cgp::vec3 _axis1, cgp::vec3 _axis2){
    translation = _start;
    color = {0,0.7,1};
    axis1 = _axis1;
    axis2 = _axis2;
    if(!triange_initialized){
        triangle_mesh.initialize_data_on_gpu(mesh_primitive_triangle({0,0,0},axis1,axis2));
        triange_initialized=true;
    }
}
numarray<partition_coordinates> collision_triangle::get_boxes(collision_partition* partition){
    numarray<partition_coordinates> numarrarr[3];
    vec3 axis1b = scaling * axis1;
    vec3 axis2b = scaling * axis2;
    numarrarr[0] = get_segment_boxes(math::segment(translation,axis1b), partition);
    numarrarr[1] = get_segment_boxes(math::segment(translation,axis2b), partition);
    numarrarr[2] = get_segment_boxes(math::segment(translation+axis1b,axis2b-axis1b), partition);
    

    numarray<partition_coordinates> FinalVec;

    
    for(int i=0;i<3;i++){
        std::vector<partition_coordinates>::iterator iter;
        for(iter=numarrarr[i].begin(); iter < numarrarr[i].end(); iter++){
            std::vector<partition_coordinates>::iterator iterFinal;
            partition_coordinates C = *iter;
            bool present = false;
            for(iterFinal=FinalVec.begin(); iterFinal < FinalVec.end(); iterFinal++){
                if(*iterFinal==C){
                    present=true;
                    break;
                }
            }
            if(!present){
                FinalVec.push_back(C);
            }

        }
    }
    return FinalVec;
}
bool collision_triangle::does_collide(collision_object* col2, vec3 &collision_point){
    collision_point = collision_point;
    if (dynamic_cast<collision_ray*>(col2) != nullptr){
        collision_ray* ray = dynamic_cast<collision_ray*>(col2);
        return ray->does_collide(this,collision_point);
    }
    return false;
}
bool collision_triangle::does_collide(collision_object* col2){
    vec3 temp;
    return does_collide(col2, temp);
}
void collision_triangle::draw(environment_structure environment){
    numarray<vec3> pos;
    pos.resize(3);
    pos[0]={0,0,0};pos[1]=axis1;pos[2]=axis2;
    triangle_mesh.vbo_position.update(pos);
    triangle_mesh.model.translation = translation;
    triangle_mesh.material.color = color;
    
    cgp::draw(triangle_mesh,environment);
}

