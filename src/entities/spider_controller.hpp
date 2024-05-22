#ifndef SPIDERCONTR_HPP
#define SPIDERCONTR_HPP

#include "../environment.hpp"

#include "spider.hpp"
#include "../utils/collision_handler.hpp"
#include "../utils/camera/third_person_camera_controller.hpp"
#include "cgp/cgp.hpp"


#define NUM_LEGS 8
#define KEYBOARD_LAYOUTS 3

class SpiderController
{
    struct debug{
        bool debug_stick_to_ground = false;
        std::vector<collision_ray> rays_to_draw;
        std::vector<vec3> rays_collision_pos;
        mesh_drawable sphere;

        void reset_stick();
    };
    struct params{
        spider::leg legs[NUM_LEGS] = {spider::FrontLeft,spider::FrontRight,spider::MiddleLeft,spider::MiddleRight,spider::Middle2Left,spider::Middle2Right,spider::BackLeft,spider::BackRight};
        float BodyHeight = 0.5f;
        float RestPositionDistance = 1.5f;
        float acceleration = 0.9f;
        float maxSpeed = 0.8f;

        float maxDt = 0.05;

        float camera_max_distance = 4.0f;

        int selected_keyboard = 1;
        std::string keyboards_control[KEYBOARD_LAYOUTS] = {"QWERTY","AZERTY","Keyboard Arrows"};
    };
private:
    //third_person_camera_controller camera_control;
    camera_controller_orbit camera_control;
    const params params;
    spider* ControlledSpider;
    bool initialized = false;
    vec3 legPositions[NUM_LEGS];
    input_devices* inputs;

    timer_basic* timer;

    vec3 position;
    vec3 velocity;
    vec3 target_velocity;
    float old_t;

    void smoothHeight(bool average=false); // Function for smoothing out the position of the body according to the position of the legs
public:
    debug debug;
    SpiderController(){}
    spider* getSpider(){return ControlledSpider;}

    void initialize(spider* _ControlledSpider,timer_basic* _timer,input_devices& _inputs, window_structure& window);
    bool stick_to_ground(collision_object* col, bool reset = true);
    void update();
    void debug_draw(environment_structure environment);

    // Control handlers
    void handleVelocity(float dt);
    void handlePosition(float dt);

    // Controls input
    void mouse_move_event(environment_structure &environment,input_devices& inputs);
    void mouse_click_event(environment_structure &environment);
    void action_keyboard(environment_structure &environment);
    void idle_frame(environment_structure &environment, collision_object* col);
};



#endif // SPIDERCONTR_HPP
