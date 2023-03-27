#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"

using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 15.0f,6.0f,6.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	int N_terrain_samples = 100;
	float terrain_length = 20;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.material.color = { 0.6f,0.85f,0.5f };
	terrain.material.phong.specular = 0.0f; // non-specular terrain material
	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.jpg",
		GL_REPEAT,
		GL_REPEAT);

	mesh const tree_mesh = create_tree();
	tree.initialize_data_on_gpu(tree_mesh);
	tree.model.translation = vec3{0,0,evaluate_terrain_height(0,0,terrain_length)};

	// mesh const cylinder_mesh = create_cylinder_mesh(1.0f, 6.0f);
	// cylinder.initialize_data_on_gpu(cylinder_mesh);

	// mesh const cone_mesh = create_cone_mesh(3.0f, 4.0f, 6.0f);
	// cone.initialize_data_on_gpu(cone_mesh);

	mesh quad_mesh = mesh_primitive_quadrangle({ -0.25f,0,0 }, { 0.25f,0,0 }, { 0.25f,0,0.5f }, { -0.25f,0,0.5f });
	grass.initialize_data_on_gpu(quad_mesh);
	grass.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/grass.png",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
	grass.material.phong = { 0.4f, 0.6f,0,1 };


	quad_mesh = mesh_primitive_quadrangle({ -0.15,0,0 }, { 0.15f,0,0 }, { 0.15f,0,0.3f }, { -0.15f,0,0.3f });
	flor.initialize_data_on_gpu(quad_mesh);
	flor.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/flor1.png",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);

	tree_position = generate_positions_on_terrain(150, terrain_length);
	grass_position = generate_positions_on_terrain(100, terrain_length);
	flor_position = generate_positions_on_terrain(20, terrain_length);
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(terrain, environment);
	draw(tree, environment);

	// draw(cylinder, environment);
	// draw(cone, environment);

	if (gui.display_wireframe){
		draw_wireframe(terrain, environment);
		draw_wireframe(tree, environment);
	}

	for(auto p: tree_position){
		tree.model.translation = p;
		draw(tree, environment);
		if(gui.display_wireframe) draw_wireframe(tree, environment);
	}
	
	display_semiTransparent();
}

void scene_structure::display_semiTransparent(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(false);

	auto const& camera = camera_control.camera_model;
	vec3 const right = camera.right();
	rotation_transform R = rotation_transform::from_frame_transform({ 1,0,0 }, { 0,0,1 }, right, { 0,0,1 });
	for(auto p : grass_position){
		grass.model.rotation = R;
		grass.model.translation = p;
		draw(grass, environment);
		if(gui.display_wireframe) draw_wireframe(grass, environment);
	}

	for(auto p : flor_position){
		flor.model.rotation = R;
		flor.model.translation = p;
		draw(flor, environment);
		if(gui.display_wireframe) draw_wireframe(flor, environment);
	}

	glDepthMask(true);
	glDisable(GL_BLEND);
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

