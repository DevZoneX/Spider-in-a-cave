#include "scene.hpp"


using namespace cgp;

#include "utils/math.hpp"

#include "subscene/test_scene.hpp"


// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize()
{
	std::cout << "Start function scene_structure::initialize()" << std::endl;


	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window); 
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{ 5.0f, -4.0f, 3.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);

	environment.fog_color = {0,0,0};

	// Display general information
	display_info();
	// Create the global (x,y,z) frame
	


    


	// Create the shapes seen in the 3D scene
	// ********************************************** //
    Spider.initialize();
    SpiderCtrl.initialize(&Spider,&timer,inputs,window);
    Cave.initialize();
    SpiderCtrl.stick_to_ground(&Cave);
}


// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{

	// Set the light to the current position of the camera
	environment.light = {0,0,0.5};//camera_control.camera_model.position();
	environment.lights.clear();

	// Update time
	timer.update();

    environment.uniform_generic.uniform_float["time"] = timer.t;
    environment.uniform_generic.uniform_int["is_cartoon"] = gui.is_cartoon ? 1 : 0;
    environment.uniform_generic.uniform_int["cartoon_levels"] = gui.cartoon_levels;
	
    debug_timer.update();

	

    if(gui.selected_scene==0){
        environment.has_fog = true;
        environment.fog_distance = 7;
        SpiderCtrl.update(&Cave);
        Cave.draw(environment);
        Spider.draw(environment);
    }
    else if(gui.selected_scene==1){
        testing_scene.display_frame(environment);
    }


	if (gui.display_wireframe) {
        /*draw_wireframe(terrain, environment);
        draw_wireframe(water, environment);
		draw_wireframe(tree, environment);
		draw_wireframe(cube1, environment);
        draw_wireframe(cube2, environment);*/
	}
	







}

void scene_structure::display_gui()
{



    ImGui::ListBox("Scene",&gui.selected_scene,gui.listc,gui.num_scenes,gui.num_scenes);

    if(!gui.has_initialized_test_scene){
        bool initNow = ImGui::Button("Initialize test scenes", {300,30});
        if(initNow){
            testing_scene.initialize(inputs,window);    
            gui.has_initialized_test_scene = true;
            gui.num_scenes = 2;
        }
    }

	if(gui.selected_scene==0){
        /*ImGui::Checkbox("Cartoon Shadering",&gui.is_cartoon);
		if(gui.is_cartoon){
        	ImGui::SliderInt("Cartoon levels",&gui.cartoon_levels,1,20);
		}*/
        SpiderCtrl.display_gui();
    }
    else if(gui.selected_scene==1){
        testing_scene.display_gui();
    }

}

void scene_structure::mouse_move_event()
{
	
    if(gui.selected_scene==0){
        SpiderCtrl.mouse_move_event(environment, inputs);
    }
    else if(gui.selected_scene==1){
        testing_scene.mouse_move_event(environment,inputs,camera_projection);
    }
    else{
        if (!inputs.keyboard.shift)
		    camera_control.action_mouse_move(environment.camera_view);
    }

}
void scene_structure::mouse_click_event()
{
    if(gui.selected_scene==0){
        SpiderCtrl.mouse_click_event(environment);
    }
    else if(gui.selected_scene==1){
        testing_scene.mouse_click_event(environment);
    }
    else{
	    camera_control.action_mouse_click(environment.camera_view);
    }
}
void scene_structure::keyboard_event()
{
    if(gui.selected_scene==0){
        SpiderCtrl.action_keyboard(environment);
    }
    else if(gui.selected_scene==1){
        testing_scene.action_keyboard(environment);
    }
    else{
        camera_control.action_keyboard(environment.camera_view);
    }
	
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
    if(gui.selected_scene==0){
        SpiderCtrl.idle_frame(environment, &Cave);
    }
    else if(gui.selected_scene==1){
        testing_scene.idle_frame(environment);
    }
}


void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Here is the main scene, initialize the test scenes to discover mechanics in details." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}
