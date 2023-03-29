#include "scene.hpp"



using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the hierarchy
	// ************************************ //

	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	// mesh_drawable cube_base;
	// mesh_drawable cylinder_base;
	// mesh_drawable sphere_junction;
	// mesh_drawable cylinder1;
	// mesh_drawable cube1;
	// mesh_drawable cylinder1_son;
	// mesh_drawable cylinder1_son_son; 

	// // Create the geometry of the meshes
	// //   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	// cube_base.initialize_data_on_gpu(mesh_primitive_cube()); cube_base.model.scaling = 0.15f;
	// cylinder_base.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 0,0,0.5f }));
	// sphere_junction.initialize_data_on_gpu(mesh_primitive_sphere(0.1f));
	// cylinder1.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 1.0f,0,0 }));
	// cube1.initialize_data_on_gpu(mesh_primitive_cube()); cube1.model.scaling = 0.15f;
	// cylinder1_son.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f, { 0,0,-0.25f }, { 0.0f,0,0.25f }));
	// cylinder1_son_son.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f, {0,-0.25f,0}, {0.0f,0.25f,0}));

	// // Set the color of some elements
	// vec3 color1 = { 0.8f, 0.5f, 0.7f };
	// vec3 color2 = { 1, 0.9f, 0};
	// cylinder1.material.color = color1;
	// cube1.material.color = color1;
	// cylinder1.material.color = color1;
	// cylinder1_son.material.color = color1;
	// cylinder1_son_son.material.color = color2;

	// // Add the elements in the hierarchy
	// //   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	// //   Notes: 
	// //     - An element must necessarily be added after its parent
	// //     - The first element (without explicit name of its parent) is assumed to be the root.
	// hierarchy.add(cube_base, "Cube base");
	// hierarchy.add(cylinder_base, "Cylinder base", "Cube base");
	// hierarchy.add(sphere_junction, "Sphere junction", "Cylinder base", {0,0,0.5f}); // the translation is used to place the sphere at the extremity of the cylinder
	// hierarchy.add(cylinder1, "Cylinder1", "Sphere junction");
	// hierarchy.add(cube1, "Cube1", "Cylinder1", {1.0f,0,0}); // the translation is used to place the cube at the extremity of the cylinder
	// hierarchy.add(cylinder1_son, "Cylinder1 son", "Cube1");
	// hierarchy.add(cylinder1_son_son, "Cylinder1 son son", "Cylinder1 son", {0,0,0.25f});
	// hierarchy.add(cylinder1_son_son, "Cylinder2 son son", "Cylinder1 son", {0,0,-0.25f});

	// initialize mesh_drawables
	mesh_drawable body, head, eye, beak, wing, wing_son;

	body.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.7f, 0.9f, 0.5f}));
	// body.model.scaling = 0.5;

	float phi, theta;
	float r = 0.35f;
	
	head.initialize_data_on_gpu(mesh_primitive_sphere(0.35f));
	eye.initialize_data_on_gpu(mesh_primitive_sphere(0.05f));
	theta = Pi*100.f/180.f;
	beak.initialize_data_on_gpu(mesh_primitive_cone(0.2, 0.3f, r*0.7f*vec3(0, sin(theta), cos(theta)), {0, sin(theta), cos(theta)}));
	wing.initialize_data_on_gpu(mesh_primitive_quadrangle({0,0.5f,0}, {0,-0.5f,0}, {0.6f,-0.5f,0}, {0.6f,0.5f,0}));
	wing_son.initialize_data_on_gpu(mesh_primitive_quadrangle({0,0.5f,0}, {0,-0.5f,0}, {0.4f, 0, 0}, {0.4f, 0.5f, 0}));

	vec3 color_black = {0, 0, 0};
	vec3 color_orange = {0.9, 0.4, 0};
	eye.material.color = color_black;
	beak.material.color = color_orange;
	
	hierarchy.add(body, "Body");
	
	// hierarchy.add(mesh_drawable(), "Wing", "Body");
	hierarchy.add(wing, "WingR", "Body", {0.5, 0, 0});
	hierarchy.add(wing_son, "WingR son", "WingR", {0.6f, 0, 0});
	
	wing.model.scaling_xyz = {-1, 1, 1};
	wing_son.model.scaling_xyz = {-1, 1, 1};

	hierarchy.add(wing, "WingL", "Body", {-0.5f, 0, 0});
	hierarchy.add(wing_son, "WingL son", "WingL", {-0.6f, 0, 0});

	// hierarchy["WingR"].transform_local.matrix = {-1, 1, 1};

	hierarchy.add(head, "Head", "Body", {0, 1, 0.3f});
	hierarchy.add(beak, "Beak", "Head");

	phi = Pi*20.f/180.f, theta = Pi*60.f/180.f;
	hierarchy.add(eye, "EyeL", "Head", r*vec3(sin(theta)*cos(phi + Pi*90.f/180.f), sin(theta)*sin(phi + Pi*90.f/180.f), cos(theta)));
	hierarchy.add(eye, "EyeR", "Head", r*vec3(sin(theta)*cos(-phi + Pi*90.f/180.f), sin(theta)*sin(-phi + Pi*90.f/180.f), cos(theta)));
}





void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();

	// hierarchy["Body"].transform_local.scaling = 0.5f;
	// // Apply transformation to some elements of the hierarchy \addindex
	// hierarchy["Cylinder1"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, timer.t);
	// hierarchy["Cube1"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -3 * timer.t);
	// hierarchy["Cylinder base"].transform_local.rotation = rotation_transform::from_axis_angle({ 1, 0, 0}, sin(timer.t));
	// hierarchy["Cylinder1 son son"].transform_local.rotation = rotation_transform::from_axis_angle({0, 0, 1}, 12*timer.t);
	// hierarchy["Cylinder2 son son"].transform_local.rotation = rotation_transform::from_axis_angle({0, 0, 1}, 12*timer.t);
	
	float angle_head = Pi*10.f/180.f, angle_wing = Pi*30.f/180.f, angle_wing_son = Pi*60.f/180.f;
	hierarchy["Head"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, angle_head*sin(2*timer.t));

	
	hierarchy["WingR"].transform_local.rotation = rotation_transform::from_axis_angle({0, 1, 0}, angle_wing*sin(4*timer.t)); 
	hierarchy["WingL"].transform_local.rotation = rotation_transform::from_axis_angle({0, 1, 0}, -angle_wing*sin(4*timer.t)); 

	hierarchy["WingR son"].transform_local.rotation = rotation_transform::from_axis_angle({0, 1, 0}, angle_wing_son*sin(4*timer.t));
    hierarchy["WingL son"].transform_local.rotation = rotation_transform::from_axis_angle({0, 1, 0}, -angle_wing_son*sin(4*timer.t));
	
	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);
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
