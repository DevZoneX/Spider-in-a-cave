#include "environment.hpp"

// Change these global values to modify the default behavior
// ************************************************************* //
// The initial zoom factor on the GUI
float project::gui_scale = 1.0f;
// Is FPS limited automatically
bool project::fps_limiting = true;
// Maximal default FPS (used only of fps_max is true)
float project::fps_max=60.0f;
// Automatic synchronization of GLFW with the vertical-monitor refresh
bool project::vsync=true;     
// Initial dimension of the OpenGL window (ratio if in [0,1], and absolute pixel size if > 1)
float project::initial_window_size_width  = 0.5f; 
float project::initial_window_size_height = 0.5f;
// ************************************************************* //


// This path will be automatically filled when the program starts
std::string project::path = ""; 




void environment_structure::send_opengl_uniform(opengl_shader_structure const& shader, bool expected) const
{
	opengl_uniform(shader, "projection", camera_projection, expected);
	opengl_uniform(shader, "view", camera_view, expected);
	opengl_uniform(shader, "light", light, false);
	opengl_uniform(shader, "multilight", multiLight, false);
	opengl_uniform(shader, "time", time, false);
	
	opengl_uniform(shader, "has_fog", has_fog, false);
	opengl_uniform(shader, "fog_distance", fog_distance, false);
	opengl_uniform(shader, "fog_color", fog_color, false);

	for(int i=0;i<max_lights;i++){
		if(i<lights.size()){
			opengl_uniform(shader, "light_"+str(i)+"_pos", lights[i].position, false);
			opengl_uniform(shader, "light_"+str(i)+"_color", lights[i].color, false);
			opengl_uniform(shader, "light_"+str(i)+"_dist", lights[i].distance, false);
			opengl_uniform(shader, "light_"+str(i)+"_intensity", lights[i].intensity, false);
		}
		else{
			opengl_uniform(shader, "light_"+str(i)+"_pos", vec3({0,0,0}), false);
			opengl_uniform(shader, "light_"+str(i)+"_color", vec3({0,0,0}), false);
			opengl_uniform(shader, "light_"+str(i)+"_dist", 1.0f, false);
			opengl_uniform(shader, "light_"+str(i)+"_intensity", 0.0f, false);
		}
	}

	uniform_generic.send_opengl_uniform(shader, false);

}