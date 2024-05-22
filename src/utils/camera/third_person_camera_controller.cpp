#include "third_person_camera_controller.hpp"

using namespace cgp;

void third_person_camera_controller::action_mouse_move(mat4& camera_matrix_view){
            // Preconditions
            assert_cgp_no_msg(inputs != nullptr);
            assert_cgp_no_msg(window != nullptr);
            if (!is_active) return;

            vec2 const& p1 = inputs->mouse.position.current;
            vec2 const& p0 = inputs->mouse.position.previous;

            bool const event_valid = !inputs->mouse.on_gui;
            bool const ctrl = inputs->keyboard.ctrl;

            if (event_valid) { // If the mouse cursor is not on the ImGui area

                if (!ctrl)     // Rotation of the camera around its center
                    camera_model.manipulator_rotate_arcball(p0, p1);
            }

            update(camera_matrix_view);
        }
