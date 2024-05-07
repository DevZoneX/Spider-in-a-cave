#ifndef SUBSCENE
#define SUBSCENE

#include "cgp/cgp.hpp"
#include "../environment.hpp"

using namespace cgp;


class subscene{
public:
    camera_controller_first_person_euler camera_control;


    // Functions
    subscene(){};
    void initialize();
    void display_frame();
    void display_gui();
    void idle_frame();

};


#endif // CAVE_MESH_H