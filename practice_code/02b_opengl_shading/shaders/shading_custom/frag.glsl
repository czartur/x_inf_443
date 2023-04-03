#version 330 core


// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// View matrix
uniform mat4 view;

struct material_structure
{
	vec3 color;  // Uniform color of the object
};
uniform material_structure material;

// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform float diffuse;
uniform float speculaire;
uniform float exp_spec;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 fog_color;

// camera position
mat3 O = transpose(mat3(view)); // get the orientation matrix
vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
vec3 camera_position = -O * last_col;

// cam and light max dist
uniform float d_max = 12.0f;
uniform float dl_max = 4.0f;

void main()
{  
    // light mitigation
    float a_l = min(length(light_position - fragment.position)/dl_max, 1);
    vec3 Cl = light_color*(1-a_l);


	vec3 current_color;
    vec3 ul = normalize(light_position - fragment.position);
    vec3 n = normalize(fragment.normal);

    vec3 uv = normalize(camera_position - fragment.position);
    vec3 ur = reflect(-ul, n);
	current_color = (ambiant + diffuse*max(0, dot(n, ul)))*material.color * Cl;
    current_color += speculaire*pow(max(0, dot(ur, uv)), exp_spec)*Cl;

    // fog effect 
    float a_f = min(length(camera_position - fragment.position)/d_max, 1);
    current_color = current_color*(1-a_f) + fog_color*a_f;

	FragColor = vec4(current_color, 1.0); 	// Note: the last alpha component is not used here

}