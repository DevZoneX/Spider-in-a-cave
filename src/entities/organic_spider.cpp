#include "organic_spider.hpp"



bool organic_spider::textureInitialized = false;
opengl_texture_image_structure organic_spider::texture;


mesh organic_spider::getLegMesh(leg whichLeg, bone whichBone, float &scaling){
    float length = getBoneLength(whichLeg,whichBone);
    mesh mesh = mesh_primitive_cylinder(0.03f,{0,0,0},{0,length,0});
    if(whichLeg==FrontLeft || whichLeg==FrontRight){
        if(whichBone==BaseBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_front_1.obj");
            scaling = length/0.308f;
        }
        else if(whichBone==MiddleBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_front_2_bis.obj");
            scaling = length/0.347f;
        }
        else if(whichBone==FootBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_front_3.obj");
            scaling = length/0.45f;
        }
    }
    else if(whichLeg==MiddleLeft || whichLeg==MiddleRight){
        if(whichBone==BaseBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_middle_1.obj");
            scaling = length/0.40f;
        }
        else if(whichBone==MiddleBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_middle_2.obj");
            scaling = length/0.38f;
        }
        else if(whichBone==FootBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_middle_3.obj");
            scaling = length/0.45f;
        }
    }
    else if(whichLeg==Middle2Left || whichLeg==Middle2Right){
        if(whichBone==BaseBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_middle2_1.obj");
            scaling = length/0.37f;
        }
        else if(whichBone==MiddleBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_middle2_2.obj");
            scaling = length/0.37f;
        }
        else if(whichBone==FootBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_middle2_3.obj");
            scaling = length/0.45f;
        }
    }
    else if(whichLeg==BackLeft || whichLeg==BackRight){
        if(whichBone==BaseBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_back_1.obj");
            scaling = length/0.36f;
        }
        else if(whichBone==MiddleBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_back_2.obj");
            scaling = length/0.33f;
        }
        else if(whichBone==FootBone){
            mesh = mesh_load_file_obj(project::path+"assets/spider/legs/spider_back_3.obj");
            scaling = length/0.50f;
        }
    }
    return mesh;
}


void organic_spider::initialize(){
    mesh_drawable body;

    if(!textureInitialized){
        texture.load_and_initialize_texture_2d_on_gpu(getTexturePath(),GL_REPEAT,GL_REPEAT);
        textureInitialized = true;
    }

    body.initialize_data_on_gpu(mesh_load_file_obj(project::path+"assets/spider/spider_body.obj"));
    body.model.scaling = 1.5;
    body.texture = texture;
    spider_hierarchy.add(body,"body");

    spider_hierarchy.add(mesh_drawable(),"leftc1","body");
    spider_hierarchy.add(mesh_drawable(),"rightc1","body");
    spider_hierarchy.add(mesh_drawable(),"leftc2","body");
    spider_hierarchy.add(mesh_drawable(),"rightc2","body");



    initializeLegHierarchy(FrontLeft, {0.27,0.24,-0.015});
    initializeLegHierarchy(FrontRight, {0.27,0.24,-0.015});
    initializeLegHierarchy(BackLeft, {-0.16,+0.26,-0.02});
    initializeLegHierarchy(BackRight, {-0.16,+0.26,-0.02});
    initializeLegHierarchy(MiddleLeft, {0.13,+0.32,-0.02});
    initializeLegHierarchy(MiddleRight, {0.14,+0.27,-0.02});
    initializeLegHierarchy(Middle2Left, {-0.02,+0.29,-0.02});
    initializeLegHierarchy(Middle2Right, {-0.02,+0.29,-0.02});


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

void organic_spider::initializeLegHierarchy(leg whichLeg, vec3 bindPosition)
{
    mesh_drawable articulation;
    mesh_drawable bone1;
    mesh_drawable bone2;
    mesh_drawable bone3;

    std::string baseName = getLegPrefix(whichLeg);
    float scaling = 1;

    articulation.initialize_data_on_gpu(mesh_primitive_sphere(0.025f));
    bone1.initialize_data_on_gpu(getLegMesh(whichLeg,BaseBone,scaling));
    bone1.model.scaling = scaling; scaling = 1;
    bone2.initialize_data_on_gpu(getLegMesh(whichLeg,MiddleBone,scaling));
    bone2.model.scaling = scaling; scaling = 1;
    bone3.initialize_data_on_gpu(getLegMesh(whichLeg,FootBone,scaling));
    bone3.model.scaling = scaling; scaling = 1;


    
    bone1.texture = texture;
    bone2.texture = texture;
    bone3.texture = texture;

    spider_hierarchy.add(mesh_drawable(),baseName+"_rest","body");
    spider_hierarchy.add(mesh_drawable(),baseName,"body");
    spider_hierarchy.add(bone1,baseName+"_articulation",baseName);
    
    spider_hierarchy.add(mesh_drawable(),baseName+"2_articulation",baseName+"_articulation");
    
    spider_hierarchy.add(bone2,baseName+"2",baseName+"2_articulation");

    spider_hierarchy.add(mesh_drawable(),baseName+"3_articulation",baseName+"2");

    spider_hierarchy.add(bone3,baseName+"3",baseName+"3_articulation");


    spider_hierarchy[baseName].transform_local.translation = {bindPosition.x,0,0};
    spider_hierarchy[baseName+"_articulation"].transform_local.translation = {0,bindPosition.y,bindPosition.z};
    spider_hierarchy[baseName+"2_articulation"].transform_local.translation = {0,getBoneLength(whichLeg,BaseBone)+0.01,0};
    spider_hierarchy[baseName+"2"].transform_local.translation = {0,0.01,0};
    spider_hierarchy[baseName+"3_articulation"].transform_local.translation = {0,getBoneLength(whichLeg,MiddleBone)+0.01,0};
    spider_hierarchy[baseName+"3"].transform_local.translation = {0,0.01,0};

    spider_hierarchy[baseName+"_rest"].transform_local.translation = getRestPositionLocal(whichLeg);
}

std::string organic_spider::getTexturePath()
{
    return project::path+"assets/spider/textures/spider3_color.jpg";
}

float organic_spider::getBoneLength(leg whichLeg, bone whichBone)
{
     if(whichLeg==FrontLeft || whichLeg==FrontRight){
        if(whichBone==BaseBone){
            return 0.40f;
        }
        else if(whichBone==MiddleBone){
            return 0.45f;
        }
        else if(whichBone==FootBone){
            return 0.65f;
        }
        return 0.0;
    }
    else if(whichLeg==BackLeft || whichLeg==BackRight){
        if(whichBone==BaseBone){
            return 0.40f;
        }
        else if(whichBone==MiddleBone){
            return 0.38f;
        }
        else if(whichBone==FootBone){
            return 0.65f;
        }
        return 0.0;
    }
    else if(whichLeg==MiddleLeft || whichLeg==MiddleRight){
        if(whichBone==BaseBone){
            return 0.40f;
        }
        else if(whichBone==MiddleBone){
            return 0.37f;
        }
        else if(whichBone==FootBone){
            return 0.60f;
        }
        return 0.0;
    }
    else if(whichLeg==Middle2Left || whichLeg==Middle2Right){
        if(whichBone==BaseBone){
            return 0.45f;
        }
        else if(whichBone==MiddleBone){
            return 0.37f;
        }
        else if(whichBone==FootBone){
            return 0.55f;
        }
        return 0.0;
    }
    return 0.0;
}
