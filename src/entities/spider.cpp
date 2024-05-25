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



    initializeLegHierarchy(FrontLeft, {0.35,0.22,0});
    initializeLegHierarchy(FrontRight, {0.35,0.22,0});
    initializeLegHierarchy(BackLeft, {-0.23,+0.25,0});
    initializeLegHierarchy(BackRight, {-0.23,+0.25,0});
    initializeLegHierarchy(MiddleLeft, {0.15,+0.3,0});
    initializeLegHierarchy(MiddleRight, {0.15,+0.3,0});
    initializeLegHierarchy(Middle2Left, {-0.05,+0.27,0});
    initializeLegHierarchy(Middle2Right, {-0.05,+0.27,0});

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
vec3 spider::getRestPositionLocal(leg whichLeg){
    vec3 position = {0,0,0};
    position.y = 0.8;
    if(whichLeg==FrontLeft || whichLeg==FrontRight){
        position.x = 0.8;
        position.y *= 0.95;
    }
    else if(whichLeg==MiddleLeft || whichLeg==MiddleRight){
        position.x = 0.4;
    }
    else if(whichLeg==Middle2Left || whichLeg==Middle2Right){
        position.x = 0;
    }
    else if(whichLeg==BackLeft || whichLeg==BackRight){
        position.x = -0.5;
        position.y *= 0.95;
    }

    if(whichLeg==FrontLeft || whichLeg==MiddleLeft || whichLeg==Middle2Left || whichLeg==BackLeft){
        position.y *= -1;
    }
    position.z = -0.5;
    return position;
}
vec3 spider::getRestPosition(leg whichLeg){
    return spider_hierarchy[getLegPrefix(whichLeg)+"_rest"].drawable.hierarchy_transform_model.translation;
}

vec3 spider::getRestPosition(leg whichLeg, float vx, float vy){
    return getRestPosition(whichLeg) + 0.4*vx*getFrontVector()+ 0.4*vy*getRightVector();
}

vec3 spider::getRestPosition(leg whichLeg, float vx, float vy, float angle)
{
    vec3 position = getRestPosition(whichLeg,vx,vy);
    float multiplier = 0.5f;
    float middleMultiplier = 0.3f;
    float frontMultiplier = 0.7;
    
    if(whichLeg==FrontLeft || whichLeg==FrontRight){
        position -= multiplier*getRightVector()*angle;
    }
    else if(whichLeg==BackLeft || whichLeg==BackRight){
        position += multiplier*getRightVector()*angle;
    }
    else if(whichLeg==MiddleLeft || whichLeg==MiddleRight){
        position -= middleMultiplier*multiplier*getRightVector()*angle;
    }
    else if(whichLeg==Middle2Left || whichLeg==Middle2Right){
        position += middleMultiplier*multiplier*getRightVector()*angle;
    }

    if(whichLeg==FrontRight || whichLeg==BackRight || whichLeg==MiddleRight || whichLeg==Middle2Right){
        frontMultiplier *= -1;
    }
    position += multiplier*frontMultiplier*getFrontVector()*angle;
    
    
    return position;
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
spider::LegPartitions spider::getLegPartitions()
{
    LegPartitions partition;
    partition.push_back({FrontLeft,MiddleRight,Middle2Left,BackRight});
    partition.push_back({FrontRight,MiddleLeft,Middle2Right,BackLeft});
    return partition;
}
vec3 spider::getUpVector()
{
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


void spider::initializeLegHierarchy(leg whichLeg, vec3 bindPosition){
    mesh_drawable articulation;
    mesh_drawable bone1;
    mesh_drawable bone2;
    mesh_drawable bone3;

    std::string baseName = getLegPrefix(whichLeg);

    articulation.initialize_data_on_gpu(mesh_primitive_sphere(0.035f));
    bone1.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(whichLeg,BaseBone),0}));
    bone2.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(whichLeg,MiddleBone),0}));
    bone3.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f,{0,0,0},{0,getBoneLength(whichLeg,FootBone),0}));

    spider_hierarchy.add(mesh_drawable(),baseName+"_rest","body");
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

    spider_hierarchy[baseName+"_rest"].transform_local.translation = getRestPositionLocal(whichLeg);
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
