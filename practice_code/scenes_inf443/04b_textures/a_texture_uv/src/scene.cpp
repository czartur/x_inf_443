#include "scene.hpp"

#include "models_textures.hpp"

using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Geometry creation
	//-----------------------------------
	// Create a quadrangle as a mesh
	mesh quadrangle_mesh;
	quadrangle_mesh.position = { {-1,-1,0}, { 1,-1,0}, { 1, 1,0},{-1, 1, 0} };
	// quadrangle_mesh.uv = { {0,1}, {1,1}, {1,0}, {0,0} }; // Associate Texture-Coordinates to the vertices of the quadrangle
	// quadrangle_mesh.uv   = { {0,0.5f}, {0.5f,0.5f}, {0.5f,0}, {0,0} };
	// quadrangle_mesh.uv   = { {0.25,0.5}, {0.5,0.5}, {0.5, 0.25}, {0.25,0.25} };
	quadrangle_mesh.uv   = { {0,2}, {2,2}, {2,0}, {0,0} };
	// quadrangle_mesh.uv   = { {0,3}, {1,3}, {1,0}, {0,0} };
	// quadrangle_mesh.uv   = { {-1,2}, {2,2}, {2,-1}, {-1,-1} };

	// quadrangle_mesh.uv = { {0,0}, {1,0}, {1,1}, {0,1} };
	quadrangle_mesh.connectivity = { {0,1,2}, {0,2,3} };

	quadrangle_mesh.fill_empty_field(); // (fill with some default values the other buffers (colors, normals) that we didn't filled before)

	// Convert the mesh structure into a mesh_drawable structure
	shape.initialize_data_on_gpu(quadrangle_mesh);


	// Texture Image load and association
	//-----------------------------------	

	// Load an image from a file, and send the result to the GPU
	shape.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/squirrel.jpg",
		GL_REPEAT,
		GL_REPEAT);

	mesh torus_mesh  = torus_with_texture();
	torus.initialize_data_on_gpu(torus_mesh);
	torus.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/cry.png",
		GL_REPEAT, 
		GL_REPEAT);

    float h = 4.0f;
	mesh cylinder_mesh = cylinder_with_texture(h);
	cylinder.initialize_data_on_gpu(cylinder_mesh);
	cylinder.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/trunk.jpg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
	
	mesh disc_up_mesh = disc_with_texture(), disc_down_mesh = disc_with_texture();
	disc_up_mesh.apply_translation_to_position(vec3{0,0, h/2.f});
	disc_down_mesh.apply_translation_to_position(vec3{0,0, -h/2.f});
	
	disc_up.initialize_data_on_gpu(disc_up_mesh);
	disc_down.initialize_data_on_gpu(disc_down_mesh);

	disc_up.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/tree-ring.png",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);

	disc_down.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/tree-ring.png",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);
	
	// draw(shape, environment);
	// draw_wireframe(shape, environment, { 1,0,0 });

	// new elements 
	// draw(torus, environment);
	draw(cylinder, environment);
	draw(disc_up, environment);
	draw(disc_down, environment);
	if(gui.display_wireframe){
		draw_wireframe(torus, environment);
		draw_wireframe(cylinder, environment);
		draw_wireframe(disc_up, environment);
		draw_wireframe(disc_down, environment);
	}
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

