#ifndef TESTSCENE
#define TESTSCENE

#include "cgp/cgp.hpp"
#include "../environment.hpp"
#include "subscene.hpp"



#include "../entities/spider_controller.hpp"
#include "../entities/spider.hpp"
#include "../entities/organic_spider.hpp"
#include "../map/cave.hpp"
#include "../utils/key_positions_structure.hpp"
#include "../utils/collision_object.hpp"
#include "../utils/collision_handler.hpp"

using namespace cgp;


struct test_gui {
    bool display_frame = true;

    int selected_scene = 5;
    int num_scenes = 6;
    const char* const listc[6] = {"Spider", "Colisions", "Partition and collisions", "Terrain and collisions", "Spider Controller", "Cave Decorator"};


    float spider_rotation_around_x = 0;
    float spider_rotation_around_y = 0;
    float spider_rotation_around_z = 0;
	vec3 director = {0,2,1};

    bool show_partition = false;
    bool show_box_partition = false;
    bool show_triangle_partition = false;
    bool show_cave = true;

    bool show_decorator = false;
};


class test_scene: public subscene {
public:

    test_gui gui;
    input_devices* inputs;


    // Props

    mesh_drawable global_frame;          // The standard global frame
    keyframe_structure legs_positions;
    keyframe_structure col_positions;
    keyframe_structure col_positions_partition;
    keyframe_structure col_positions_scene3;
    keyframe_structure col_positions_decorator;
    organic_spider Spider;
    organic_spider Spider2;
    SpiderController SpiderCtrl;
    collision_sphere* colsphere;
    collision_sphere* colsphere2;
    collision_box* colsbox;
    collision_box* colsbox2;
    collision_ray* colray;
    collision_triangle* coltri;
    mesh_drawable sphere;

    collision_partition *cpart;
    partition_coordinates C;
    cave cave_obj;

    cristal_rock_gold cristal_decorator;
    float deco_z_rot = 0;



	timer_basic timer;
    timer_event_periodic debug_timer;

    // Functions
    test_scene(){};
    ~test_scene();
    void initialize(input_devices& _inputs, window_structure& window);
    void display_frame(environment_structure &environment);
    void display_gui();
    void mouse_move_event(environment_structure &environment,input_devices& inputs,camera_projection_perspective &_camera_projection);
    void mouse_click_event(environment_structure &environment);
    void action_keyboard(environment_structure &environment);
    void idle_frame(environment_structure &environment);
};


#endif // CAVE_MESH_H