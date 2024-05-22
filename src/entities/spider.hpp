#ifndef SPIDER_HPP
#define SPIDER_HPP

#include "../environment.hpp"
#include "utils/fabric.hpp"
#include "utils/touchable_object.hpp"

#include "cgp/cgp.hpp"

class spider: public touchable_object
{
public:
    enum leg {FrontLeft, FrontRight, MiddleLeft, MiddleRight, Middle2Left, Middle2Right, BackLeft, BackRight};
    enum bone {BaseBone, MiddleBone, FootBone};
private:
    float getBoneLength(leg whichLeg,bone whichBone);

    void initializeLegHierarchy(leg whichLeg, std::string baseName, vec3 bindPosition);
    void updateLegHierarchy(leg whichLeg, std::string baseName);

    fabric* getLegFabric(leg whichLeg);
    void initializeLegFabric(leg whichLeg);
    cgp::hierarchy_mesh_drawable spider_hierarchy;
    rotation_transform rotation = cgp::rotation_axis_angle({0,0,1},0);
    fabric legFabric[8];
    float c_angle = Pi/7;
    float c_angle_2 = Pi/6;
public:
    spider();
    ~spider();

    vec3 translation = {0,0,0};

    void initialize();

    void draw(environment_structure environment);

    void updateGlobal();
    void setLegPosition(leg whichLeg, vec3 target, bool debug=false);

    vec3 getLegPosition(leg whichLeg);
    vec3 getLegJoint(leg whichLeg);
    vec3 getUpVector();
    vec3 getRightVector();
    vec3 getFrontVector();
    std::string getLegPrefix(leg whichLeg);

    void set_rotation(rotation_transform _rotation);
    rotation_transform get_rotation();
};



#endif // SPIDER_HPP
