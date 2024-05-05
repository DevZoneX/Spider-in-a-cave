#include "cgp/cgp.hpp"

#include "environment.hpp"

/** Helping structure handling the display and interaction with the set of key-frames: key positions + times 
	Note: this structure doesn't perform the interpolation */
struct keyframe_structure {


public:

	// Storage of the key positions and times
	//  key_positions and key_times should have the same number of elements
    cgp::numarray<cgp::vec3> key_positions;


	// This function must be called once at the initialization step.
	//  It sets the positions and times, but also initialize the mesh_drawable elements
    void initialize(cgp::numarray<cgp::vec3> const& key_positions);
	
	// This function should be called every time the mouse cursor is moved - it handles the interactive drag of positions
	void update_picking(cgp::input_devices const& inputs, camera_generic_base const& camera, camera_projection_perspective const& projection);

	// Draw all the key positions as well as the polygon linking these positions
	void display_key_positions(environment_structure const& environment);


	// Draw the GUI elements associated to the keyframe
	void display_gui();

	cgp::trajectory_drawable trajectory;
private:

	cgp::mesh_drawable sphere_key_positions; // spheres used to display the key positions
	cgp::mesh_drawable sphere_current;       // sphere used to display the current interpolated position
	cgp::curve_drawable polygon_key_positions; //
	
	cgp::picking_structure picking;

	// Gui information
    bool display_polygon = false;
    bool display_keyposition = true;


};
