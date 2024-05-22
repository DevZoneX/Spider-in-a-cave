#include "test_scene.hpp"


// Your function

test_scene::~test_scene() {
    if(colsbox!=nullptr){
        delete colsbox;
    }
    if(colsbox2!=nullptr){
        delete colsbox2;
    }
    if(colsphere!=nullptr){
        delete colsphere;
    }
    if(colsphere2!=nullptr){
        delete colsphere2;
    }
    if(colray!=nullptr){
        delete colray;
    }
    if(coltri!=nullptr){
        delete coltri;
    }
    if(cpart!=nullptr){
        delete cpart;
    }
}

void test_scene::initialize(input_devices& _inputs, window_structure& window){

	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(_inputs, window); 
    inputs = &_inputs;
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{ 5.0f, -4.0f, 3.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);



    
    numarray<vec3> key_positions =
    { {0.647256,0.900676,-0.441647}, {0.652574,-0.942182,-0.469014}, {-0.263303,1,-0.516560}, {-0.263303,-1,-0.516560}, {0.3,1,-0.432094}, {0.3,-1,-0.432094}, {0.089932,1,-0.432094}, {0.089932,-1,-0.436595}};
    legs_positions.initialize(key_positions);

    numarray<vec3> key_positions_col =
    { {0,3,0}, {0,4,0}, {0,0.5,0}, {0,-0.5,0}, {1,1,1}, {0,0.5,2}, {-1,1,1}, {1,2,0}, {2,0,1} };
    col_positions.initialize(key_positions_col);
    numarray<vec3> key_positions_col_part =
    { {0,3,0}, {3,3,0}, {0,0,0}, {1,1,1}, {0,1,0}, {0.5,0,0.5} };
    col_positions_partition.initialize(key_positions_col_part);

    numarray<vec3> key_positions_ray =
    { {0,3,0}, {3,3,0} };
    col_positions_scene3.initialize(key_positions_ray);

    Spider.initialize();
    Spider2.initialize();

    debug_timer.t_periodic = 1;

    colsphere = new collision_sphere({0,0,0}, 1.0f);
    colsphere->translation = {0,3,0};
    colsphere2 = new collision_sphere({1,0,0}, 1.0f);
    colsphere2->translation = {0,4,0};

    colsbox = new collision_box({0,0,0},{1,0,0},{0,1,0},{0,0,1});
    colsbox->translation = {0,0.5,0};
    colsbox->rotation = rotation_axis_angle({0,1,1},0);

    colsbox2 = new collision_box({0,0,0},{1,0,0},{0,1,0},{0,0,1});
    //colsbox2->rotation = rotation_axis_angle({0,1,1},10);
    colsbox2->translation = {0,-0.5,0};

    colray = new collision_ray({1,1,1},{0,0.5,2});

    coltri = new collision_triangle({-1,1,1},{1,2,0},{2,0,1});

    sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.05f));
    sphere.material.color={0.2,0,0.1};


    cpart = new collision_partition({2,2,2},{0,0,0});



    cave.initialize();
    SpiderCtrl.initialize(&Spider2);
    SpiderCtrl.stick_to_ground(&cave);
}

void test_scene::display_frame(environment_structure &environment) {
    if(gui.selected_scene==0){
        Spider.set_rotation(rotation_transform::from_axis_angle({1,0,0},gui.spider_rotation_around_x) * rotation_transform::from_axis_angle({0,1,0},gui.spider_rotation_around_y) * rotation_transform::from_axis_angle({0,0,1},gui.spider_rotation_around_z));
        Spider.setLegPosition(spider::FrontRight,legs_positions.key_positions[0],false);
        Spider.setLegPosition(spider::FrontLeft,legs_positions.key_positions[1],false);
        Spider.setLegPosition(spider::BackRight,legs_positions.key_positions[2],false);
        Spider.setLegPosition(spider::BackLeft,legs_positions.key_positions[3],false);
        Spider.setLegPosition(spider::MiddleRight,legs_positions.key_positions[4],false);
        Spider.setLegPosition(spider::MiddleLeft,legs_positions.key_positions[5],false);
        Spider.setLegPosition(spider::Middle2Right,legs_positions.key_positions[6],false);
        Spider.setLegPosition(spider::Middle2Left,legs_positions.key_positions[7],false);
        Spider.draw(environment);
        legs_positions.display_key_positions(environment);
    }
    else if(gui.selected_scene==1){

        colsphere->translation = col_positions.key_positions[0];
        colsphere2->translation = col_positions.key_positions[1];
        colsbox->translation = col_positions.key_positions[2];
        colsbox2->translation = col_positions.key_positions[3];
        colray->translation = col_positions.key_positions[4];
        colray->director = col_positions.key_positions[5]-col_positions.key_positions[4];
        coltri->translation = col_positions.key_positions[6];
        coltri->axis1 = col_positions.key_positions[7]-col_positions.key_positions[6];
        coltri->axis2 = col_positions.key_positions[8]-col_positions.key_positions[6];
        vec3 temp = {0,0,0};
        colsphere->color = {0,1,0};
        colsphere2->color = {0,0,1};
        if(colsphere->does_collide(colsphere2)){
            colsphere->color = {1,0,0};
            colsphere2->color = {1,0,0};
        }

        colsbox->rotation = rotation_transform::from_axis_angle({0,0.1,1},40);

        colsbox->draw(environment);
        colsbox2->draw(environment);
        if(colsbox->does_collide(colsbox2,temp)){
            colsbox->color = {1,0,0};
            colsbox2->color = {1,0,0};
            sphere.model.translation = temp;
            draw(sphere,environment);
        }
        
        if(colray->does_collide(colsbox,temp)){
            sphere.model.translation = temp;
            draw(sphere,environment);
        }
        if(colray->does_collide(colsbox2,temp)){
            sphere.model.translation = temp;
            draw(sphere,environment);
        }

        if(colray->does_collide(colsphere,temp)){
            colsphere->color = {0,0.2,0};
            sphere.model.translation = temp;
            draw(sphere,environment);
        }
        if(colray->does_collide(colsphere2,temp)){
            colsphere2->color = {0,0.2,0};
            sphere.model.translation = temp;
            draw(sphere,environment);
        }

        colray->draw(environment);
        colsphere->draw(environment);
        colsphere2->draw(environment);

        coltri->color = {1,1,0};
        if(colray->does_collide(coltri,temp)){
            coltri->color = {0,0.2,0};
            sphere.model.translation = temp;
            draw(sphere,environment);
        }


        
        
        coltri->draw(environment);
        col_positions.display_key_positions(environment);
    }
    else if(gui.selected_scene==2){
        
        colray->translation = col_positions_partition.key_positions[0];
        colray->director = col_positions_partition.key_positions[1]-col_positions_partition.key_positions[0];
        colsbox->translation = col_positions_partition.key_positions[2];
        coltri->translation = col_positions_partition.key_positions[3];
        coltri->axis1 = col_positions_partition.key_positions[4]-col_positions_partition.key_positions[3];
        coltri->axis2 = col_positions_partition.key_positions[5]-col_positions_partition.key_positions[3];
        numarray<partition_coordinates> Cs = colray->get_boxes(cpart);
        numarray<partition_coordinates> Cs_box = colsbox->get_boxes(cpart);
        numarray<partition_coordinates> Cs_tri = coltri->get_boxes(cpart);

        if(gui.show_partition){
            for(int i=0;i<Cs.size();i++){
                cpart->draw(Cs[i], environment);
            }
        }
        if(gui.show_box_partition){
            for(int i=0;i<Cs_box.size();i++){
                cpart->draw(Cs_box[i], environment);
            }
        }
        if(gui.show_triangle_partition){
            for(int i=0;i<Cs_tri.size();i++){
                cpart->draw(Cs_tri[i], environment);
            }
        }
        colray->draw(environment);
        colsbox->draw(environment);
        coltri->draw(environment);
        col_positions_partition.display_key_positions(environment);
    }
    else if(gui.selected_scene==3){
        if(gui.show_cave){
            cave.draw(environment);
        }
        colray->translation = col_positions_scene3.key_positions[0];
        colray->director = col_positions_scene3.key_positions[1] - col_positions_scene3.key_positions[0];
        colray->draw(environment);
        col_positions_scene3.display_key_positions(environment);

        vec3 temp;
        if(cave.does_collide(colray,temp)){
            sphere.model.translation = temp;
            draw(sphere,environment);
            std::cout << "Collision";
        }
    }
    else if(gui.selected_scene==4){
        SpiderCtrl.update();
        cave.draw(environment);
        Spider2.draw(environment);
        SpiderCtrl.debug_draw(environment);
    }
}


void test_scene::display_gui(){

    ImGui::ListBox("Test Scenes",&gui.selected_scene,gui.listc,gui.num_scenes,gui.num_scenes);

    if(gui.selected_scene==0){
        ImGui::SliderFloat("Rotation around X",&gui.spider_rotation_around_x,-Pi/4,Pi/4);
        ImGui::SliderFloat("Rotation around Y",&gui.spider_rotation_around_y,-Pi/3,Pi/3);
        ImGui::SliderFloat("Rotation around Z",&gui.spider_rotation_around_z,-Pi,Pi);
        legs_positions.display_gui();
    }
    else if(gui.selected_scene==1){
        ImGui::SliderFloat("Sphere1 radius",&colsphere->r,0,3.0f);
        ImGui::SliderFloat("Sphere2 radius",&colsphere2->r,0,3.0f);
        float v[3] = {1,1,1};
        v[0]=colsbox->scaling_xyz.x;v[1]=colsbox->scaling_xyz.y;v[2]=colsbox->scaling_xyz.z;
        ImGui::SliderFloat3("Cube1 scale",v,0,3.0f);
        colsbox->scaling_xyz = {v[0],v[1],v[2]};

        v[0]=colsbox2->scaling_xyz.x;v[1]=colsbox2->scaling_xyz.y;v[2]=colsbox2->scaling_xyz.z;
        ImGui::SliderFloat3("Cube2 scale",v,0,3.0f);
        colsbox2->scaling_xyz = {v[0],v[1],v[2]};
        //ImGui::SliderFloat("Sphere2 radius",&colsphere2->r,0,3.0f);
    }
    else if(gui.selected_scene==2){
        ImGui::Checkbox("Show partition", &gui.show_partition);
        if(gui.show_partition){
            /*int c[3] = {C.x,C.y,C.z};
            ImGui::SliderInt3("Partition Coordinates",c,-cpart->get_N_x(),cpart->get_N_x()-1);
            C = {c[0],c[1],c[2]};*/
        }
        ImGui::Checkbox("Show Box partition", &gui.show_box_partition);
        ImGui::Checkbox("Show Triangle partition", &gui.show_triangle_partition);
    }
    else if(gui.selected_scene==3){
        ImGui::Checkbox("Show Cave", &gui.show_cave);
    }
    else if(gui.selected_scene==4){

    }
}

void test_scene::mouse_move_event(environment_structure &environment,input_devices& inputs,camera_projection_perspective &_camera_projection){
    if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);

    if(gui.selected_scene==0){
        legs_positions.update_picking(inputs, camera_control.camera_model, _camera_projection);
    }
    else if(gui.selected_scene==1){
        col_positions.update_picking(inputs, camera_control.camera_model, _camera_projection);
    }
    else if(gui.selected_scene==2){
        col_positions_partition.update_picking(inputs, camera_control.camera_model, _camera_projection);
    }
    else if(gui.selected_scene==3){
        col_positions_scene3.update_picking(inputs, camera_control.camera_model, _camera_projection);
    }
}
void test_scene::mouse_click_event(environment_structure &environment){
    camera_control.action_mouse_move(environment.camera_view);
}
void test_scene::action_keyboard(environment_structure &environment){
    camera_control.action_keyboard(environment.camera_view);
}
void test_scene::idle_frame(environment_structure &environment){
    camera_control.idle_frame(environment.camera_view);
}


