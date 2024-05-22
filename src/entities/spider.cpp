#include "spider.hpp"

using namespace cgp;

spider::spider(){
}
spider::~spider(){
}


void spider::initialize(){
    mesh_drawable body;
    mesh_drawable tail_body;
    mesh_drawable cube;
    mesh_drawable articulation;


    mesh_drawable front_leg_c1;
    mesh_drawable front_leg_c2;
    mesh_drawable front_leg_c3;


    body.initialize_data_on_gpu(mesh_primitive_sphere(0.45f));
    tail_body.initialize_data_on_gpu(mesh_primitive_sphere(0.6f));
    cube.initialize_data_on_gpu(mesh_primitive_cube({0,0,0},0.2f));
    articulation.initialize_data_on_gpu(mesh_primitive_sphere(0.035f));
    body.model.scaling_xyz = {1.3,0.7f,0.5f};
    tail_body.model.scaling_xyz = {1.5,1,1};
    tail_body.model.translation = {-0.8 ,0 ,0};
    cube.model.translation = {0.15f,0,0};
    cube.model.scaling_xyz = {1.5,0.25f,0.25f};


    front_leg_c1.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(FrontLeft,BaseBone),0}));
    front_leg_c2.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(FrontLeft,MiddleBone),0}));
    front_leg_c3.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(FrontLeft,FootBone),0}));

    spider_hierarchy.add(body,"body");
    spider_hierarchy.add(tail_body,"tail_body","body");
    spider_hierarchy.add(mesh_drawable(),"crochet","body");
    spider_hierarchy.add(cube,"leftc1","crochet");
    spider_hierarchy.add(cube,"rightc1","crochet");
    spider_hierarchy.add(cube,"leftc2","leftc1");
    spider_hierarchy.add(cube,"rightc2","rightc1");


    spider_hierarchy["tail_body"].transform_local.translation = {-0.57,0,0};
    spider_hierarchy["tail_body"].transform_local.rotation = rotation_transform::from_axis_angle({0,1,0},Pi/6);
    spider_hierarchy["leftc1"].transform_local.translation = {0.4,-0.15f,0.04};
    spider_hierarchy["rightc1"].transform_local.translation = {0.4,0.15f,0.04};
    spider_hierarchy["crochet"].transform_local.rotation = rotation_transform::from_axis_angle({0,1,0},0.2);
    spider_hierarchy["leftc2"].transform_local.translation = {0.29,0,0};
    spider_hierarchy["rightc2"].transform_local.translation = {0.29,0,0};



    initializeLegHierarchy(FrontLeft, "front_arm_left", {0.35,0.22,0});
    initializeLegHierarchy(FrontRight, "front_arm_right", {0.35,0.22,0});
    initializeLegHierarchy(BackLeft, "back_arm_left", {-0.18,+0.25,0});
    initializeLegHierarchy(BackRight, "back_arm_right", {-0.18,+0.25,0});
    initializeLegHierarchy(MiddleLeft, "middle_arm_left", {0.15,+0.3,0});
    initializeLegHierarchy(MiddleRight, "middle_arm_right", {0.15,+0.3,0});
    initializeLegHierarchy(Middle2Left, "middle_arm2_left", {-0.05,+0.27,0});
    initializeLegHierarchy(Middle2Right, "middle_arm2_right", {-0.05,+0.27,0});

    initializeLegFabric(FrontRight);
    initializeLegFabric(FrontLeft);
    initializeLegFabric(BackLeft);
    initializeLegFabric(BackRight);
    initializeLegFabric(MiddleLeft);
    initializeLegFabric(MiddleRight);
    initializeLegFabric(Middle2Left);
    initializeLegFabric(Middle2Right);

    updateLegHierarchy(FrontLeft,"front_arm_left");
    updateLegHierarchy(FrontRight,"front_arm_right");
    updateLegHierarchy(BackLeft,"back_arm_left");
    updateLegHierarchy(BackRight,"back_arm_right");
    updateLegHierarchy(MiddleLeft,"middle_arm_left");
    updateLegHierarchy(MiddleRight,"middle_arm_right");
    updateLegHierarchy(Middle2Left,"middle_arm2_left");
    updateLegHierarchy(Middle2Right,"middle_arm2_right");
}

void spider::draw(environment_structure environment){
    spider_hierarchy["body"].transform_local.translation = translation;
    spider_hierarchy["body"].transform_local.rotation = rotation;

    spider_hierarchy["leftc1"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},-c_angle);
    spider_hierarchy["rightc1"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},c_angle);
    spider_hierarchy["leftc2"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},c_angle_2);
    spider_hierarchy["rightc2"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},-c_angle_2);

    //float front_rotation_basis_x = Pi/6;
    //float front_rotation_middle = -Pi/3;
    //float front_rotation_last = -Pi/3;

    fabric* legF = getLegFabric(FrontRight);
    spider_hierarchy["front_arm_right_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},legF->getBoneAngle(-1)) * rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(0));
    spider_hierarchy["front_arm_right2_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(1));
    spider_hierarchy["front_arm_right3_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(2));

    legF = getLegFabric(FrontLeft);
    spider_hierarchy["front_arm_left"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},Pi);
    spider_hierarchy["front_arm_left_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},legF->getBoneAngle(-1)) * rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(0));
    spider_hierarchy["front_arm_left2_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(1));
    spider_hierarchy["front_arm_left3_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(2));

    updateLegHierarchy(BackLeft,"back_arm_left");
    updateLegHierarchy(BackRight,"back_arm_right");
    updateLegHierarchy(MiddleLeft,"middle_arm_left");
    updateLegHierarchy(MiddleRight,"middle_arm_right");
    updateLegHierarchy(Middle2Left,"middle_arm2_left");
    updateLegHierarchy(Middle2Right,"middle_arm2_right");

    spider_hierarchy.update_local_to_global_coordinates();
    cgp::draw(spider_hierarchy,environment);
}
void spider::updateGlobal(){
    spider_hierarchy.update_local_to_global_coordinates();
}

void spider::setLegPosition(leg whichLeg, vec3 target, bool debug){
    hierarchy_mesh_drawable_node spider_node;
    hierarchy_mesh_drawable_node parent_node;
    if(whichLeg==FrontRight){
        spider_node = spider_hierarchy["front_arm_right_articulation"];
        parent_node = spider_hierarchy["front_arm_right"];
    }
    else if(whichLeg==FrontLeft){
        spider_node = spider_hierarchy["front_arm_left_articulation"];
        parent_node = spider_hierarchy["front_arm_left"];
    }
    else if(whichLeg==BackRight){
        spider_node = spider_hierarchy["back_arm_right_articulation"];
        parent_node = spider_hierarchy["back_arm_right"];
    }
    else if(whichLeg==BackLeft){
        spider_node = spider_hierarchy["back_arm_left_articulation"];
        parent_node = spider_hierarchy["back_arm_left"];
    }
    else if(whichLeg==MiddleRight){
        spider_node = spider_hierarchy["middle_arm_right_articulation"];
        parent_node = spider_hierarchy["middle_arm_right"];
    }
    else if(whichLeg==MiddleLeft){
        spider_node = spider_hierarchy["middle_arm_left_articulation"];
        parent_node = spider_hierarchy["middle_arm_left"];
    }
    else if(whichLeg==Middle2Right){
        spider_node = spider_hierarchy["middle_arm2_right_articulation"];
        parent_node = spider_hierarchy["middle_arm2_right"];
    }
    else if(whichLeg==Middle2Left){
        spider_node = spider_hierarchy["middle_arm2_left_articulation"];
        parent_node = spider_hierarchy["middle_arm2_left"];
    }
    getLegFabric(whichLeg)->calculate(spider_node.drawable.hierarchy_transform_model.translation,target,parent_node.drawable.hierarchy_transform_model.rotation,debug);


}

float spider::getBoneLength(leg whichLeg, bone whichBone){
    if(whichLeg==FrontLeft || whichLeg==FrontRight){
        if(whichBone==BaseBone || whichBone==FootBone){
            return 0.45f;
        }
        else if(whichBone==MiddleBone){
            return 0.35f;
        }
        return 0.0;
    }
    else if(whichLeg==BackLeft || whichLeg==BackRight){
        if(whichBone==BaseBone || whichBone==FootBone){
            return 0.45f;
        }
        else if(whichBone==MiddleBone){
            return 0.35f;
        }
        return 0.0;
    }
    else if(whichLeg==MiddleLeft || whichLeg==MiddleRight || whichLeg==Middle2Left || whichLeg==Middle2Right){
        if(whichBone==BaseBone || whichBone==FootBone){
            return 0.35f;
        }
        else if(whichBone==MiddleBone){
            return 0.30f;
        }
        return 0.0;
    }
    return 0.0;
}

fabric* spider::getLegFabric(leg whichLeg){
    return &legFabric[whichLeg];
}

void spider::initializeLegFabric(leg whichLeg){
    float *lengths = new float[3];
    lengths[0] = getBoneLength(whichLeg,BaseBone);
    lengths[1] = getBoneLength(whichLeg,MiddleBone);
    lengths[2] = getBoneLength(whichLeg,FootBone);
    getLegFabric(whichLeg)->initialize(lengths,3);
}

vec3 spider::getLegPosition(leg whichLeg){
    return getLegFabric(whichLeg)->getLast();
}

vec3 spider::getLegJoint(leg whichLeg){
    return spider_hierarchy[getLegPrefix(whichLeg)+"_articulation"].drawable.hierarchy_transform_model.translation;
}
std::string spider::getLegPrefix(leg whichLeg){
    switch (whichLeg)
    {
        case BackLeft:
            return "back_arm_left";
            break;
        case BackRight:
            return "back_arm_right";
            break;
        case MiddleLeft:
            return "middle_arm_left";
            break;
        case MiddleRight:
            return "middle_arm_right";
            break;
        case Middle2Left:
            return "middle_arm2_left";
            break;
        case Middle2Right:
            return "middle_arm2_right";
            break;
        case FrontLeft:
            return "front_arm_left";
            break;
        case FrontRight:
            return "front_arm_right";
            break;
        default:
            return "front_arm_right";
            break;
    }
}
vec3 spider::getUpVector(){
    vec3 up = {0,0,1};
    return rotation*up;
}
vec3 spider::getRightVector(){
    vec3 right = {0,-1,0};
    return rotation*right;
}
vec3 spider::getFrontVector(){
    vec3 front = {1,0,0};
    return rotation*front;
}


void spider::initializeLegHierarchy(leg whichLeg, std::string baseName, vec3 bindPosition){
    mesh_drawable articulation;
    mesh_drawable bone1;
    mesh_drawable bone2;
    mesh_drawable bone3;

    articulation.initialize_data_on_gpu(mesh_primitive_sphere(0.035f));
    bone1.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(whichLeg,BaseBone),0}));
    bone2.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(whichLeg,MiddleBone),0}));
    bone3.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(whichLeg,FootBone),0}));

    spider_hierarchy.add(mesh_drawable(),baseName,"body");
    spider_hierarchy.add(bone1,baseName+"_articulation",baseName);
    spider_hierarchy.add(articulation,baseName+"2_articulation",baseName+"_articulation");
    spider_hierarchy.add(bone2,baseName+"2",baseName+"2_articulation");
    spider_hierarchy.add(articulation,baseName+"3_articulation",baseName+"2");
    spider_hierarchy.add(bone3,baseName+"3",baseName+"3_articulation");


    spider_hierarchy[baseName].transform_local.translation = {bindPosition.x,0,0};
    spider_hierarchy[baseName+"_articulation"].transform_local.translation = {0,bindPosition.y,bindPosition.z};
    spider_hierarchy[baseName+"2_articulation"].transform_local.translation = {0,getBoneLength(whichLeg,BaseBone)+0.01,0};
    spider_hierarchy[baseName+"2"].transform_local.translation = {0,0.01,0};
    spider_hierarchy[baseName+"3_articulation"].transform_local.translation = {0,getBoneLength(whichLeg,MiddleBone)+0.01,0};
    spider_hierarchy[baseName+"3"].transform_local.translation = {0,0.01,0};
}

void spider::updateLegHierarchy(leg whichLeg, std::string baseName){
    fabric* legF = getLegFabric(whichLeg);

    if(whichLeg==FrontLeft || whichLeg==MiddleLeft || whichLeg==BackLeft || whichLeg==Middle2Left){
        spider_hierarchy[baseName].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},Pi);
    }
    spider_hierarchy[baseName+"_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1},legF->getBoneAngle(-1)) * rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(0));
    spider_hierarchy[baseName+"2_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(1));
    spider_hierarchy[baseName+"3_articulation"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0},legF->getBoneAngle(2));
}


void spider::set_rotation(rotation_transform _rotation){
    rotation = _rotation;
    spider_hierarchy["body"].transform_local.rotation = rotation;
    spider_hierarchy.update_local_to_global_coordinates();
}
rotation_transform spider::get_rotation(){
    return rotation;
}
