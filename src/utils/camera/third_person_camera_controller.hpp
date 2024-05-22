#pragma once

#include "cgp/cgp.hpp"





namespace cgp
{
    class third_person_camera_controller: public camera_controller_orbit
    {
    public:
        void action_mouse_move(mat4& camera_matrix_view);
    };
} // namespace cgp


