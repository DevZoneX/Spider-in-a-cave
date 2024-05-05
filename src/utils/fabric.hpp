#ifndef FABRIC_HPP
#define FABRIC_HPP

#include "cgp/cgp.hpp"

using namespace cgp;

class fabric
{
private:
    int bone_number=0;
    float *lengths;
    float *angles;
    float z_angle = 0;
    vec2 *positions;
    vec3 last_last, last_first;
public:
    fabric();
    ~fabric();

    bool initialized = false;
    static const int iterations = 5 ;

    int debug_counter = 0;

    void initialize(float *_lengths,cgp::vec2 *_positions,int _bone_number);
    void initialize(float *_lengths,int _bone_number);

    void calculate(cgp::vec3 start,cgp::vec3 target, cgp::rotation_transform rotation, bool debug=false);

    float getBoneAngle(int bone);

    cgp::vec3 getLast();
};

#endif // FABRIC_HPP
