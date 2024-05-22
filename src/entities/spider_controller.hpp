#ifndef SPIDERCONTR_HPP
#define SPIDERCONTR_HPP

#include "../environment.hpp"

#include "spider.hpp"
#include "../utils/collision_handler.hpp"
#include "cgp/cgp.hpp"


#define NUM_LEGS 8

class SpiderController
{
    struct debug{
        bool debug_stick_to_ground = false;
        std::vector<collision_ray> rays_to_draw;
        std::vector<vec3> rays_collision_pos;
        mesh_drawable sphere;
    };
    struct params{
        spider::leg legs[NUM_LEGS] = {spider::FrontLeft,spider::FrontRight,spider::MiddleLeft,spider::MiddleRight,spider::Middle2Left,spider::Middle2Right,spider::BackLeft,spider::BackRight};
        float BodyHeight = 0.5f;
        float RestPositionDistance = 1.5f;
    };
private:
    const params params;
    spider* ControlledSpider;
    bool initialized = false;
    vec3 legPositions[NUM_LEGS];

    void smoothHeight(); // Function for smoothing out the position of the body according to the position of the legs
public:
    debug debug;
    SpiderController(){}
    spider* getSpider(){return ControlledSpider;}

    void initialize(spider* _ControlledSpider);
    bool stick_to_ground(collision_object* colh);
    void update();
    void debug_draw(environment_structure environment);
};



#endif // SPIDERCONTR_HPP
