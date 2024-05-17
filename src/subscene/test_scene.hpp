#ifndef TESTSCENE
#define TESTSCENE

#include "cgp/cgp.hpp"
#include "../environment.hpp"
#include "subscene.hpp"



#include "../entities/spider.hpp"
#include "../map/cave_mesh.hpp"
#include "../utils/key_positions_structure.hpp"
#include "../utils/collision_object.hpp"
#include "../utils/collision_handler.hpp"

using namespace cgp;


struct test_gui {

    int selected_scene = 2;
    int num_scenes = 4;
    const char* const listc[4] = {"Spider", "Colisions", "Partition and collisions", "Terrain and collisions"};


    float spider_rotation_around_x = 0;
    float spider_rotation_around_y = 0;
    float spider_rotation_around_z = 0;
	vec3 director = {0,2,1};

    bool show_partition = false;
    bool show_box_partition = false;
    bool show_triangle_partition = false;
    bool show_cave = true;
};


class test_scene: public subscene {
public:

    test_gui gui;

    // Props


    keyframe_structure legs_positions;
    keyframe_structure col_positions;
    keyframe_structure col_positions_partition;
    keyframe_structure col_positions_scene3;
    spider Spider;
    collision_sphere* colsphere;
    collision_sphere* colsphere2;
    collision_box* colsbox;
    collision_box* colsbox2;
    collision_ray* colray;
    collision_triangle* coltri;
    mesh_drawable sphere;

    collision_partition *cpart;
    partition_coordinates C;
    cave_mesh cave;



	timer_basic timer;
    timer_event_periodic debug_timer;

    // Functions
    test_scene(){};
    ~test_scene();
    void initialize(input_devices& inputs, window_structure& window);
    void display_frame(environment_structure &environment);
    void display_gui();
    void mouse_move_event(environment_structure &environment,input_devices& inputs,camera_projection_perspective &_camera_projection);
    void mouse_click_event(environment_structure &environment);
    void action_keyboard(environment_structure &environment);
    void idle_frame(environment_structure &environment);
};


#endif // CAVE_MESH_H