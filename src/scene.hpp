#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"

#include "map/cave.hpp"

#include "subscene/test_scene.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;

// Variables associated to the GUI (buttons, etc)
struct gui_parameters {
	
	bool display_wireframe = false;

	bool has_initialized_test_scene = false;

    int selected_scene = 0;
    int num_scenes = 1;
    const char* const listc[2] = {"Main Game","Test scene"};

    float spider_rotation_around_x = 0;
    float spider_rotation_around_y = 0;
    float spider_rotation_around_z = 0;


	bool is_cartoon = false;
	int cartoon_levels = 7;

};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
    //camera_controller_orbit_euler camera_control;
    camera_controller_first_person_euler camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	timer_basic timer;

    timer_event_periodic debug_timer;

    cave Cave;
	organic_spider Spider;
	SpiderController SpiderCtrl;
    // A helper structure used to enable hot update for the spider debug


	test_scene testing_scene;



	// ****************************** //
	// Functions
	// ****************************** //

    

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

	void display_info();



};





