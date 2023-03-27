
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (x,y)
float evaluate_terrain_height(float x, float y, float terrain_length)
{
    // vec2 p_0 = { 0, 0 };
    // float h_0 = 2.0f;
    // float sigma_0 = 3.0f;

    // float d = norm(vec2(x, y) - p_0) / sigma_0;

    // float z = h_0 * std::exp(-d * d);

    int n = 4;
    vec2 p[] = {{-10,-10},{5,5},{-3,4},{6,4}};
    float h[] = {3,-1.5,1,2};
    float sigma[] = {10,3,4,4};
    
    float z = 0;
    for(int i=0; i<n; i++){
        float d = norm(vec2(x,y) - p[i])/sigma[i];
        z += h[i]*std::exp(-d * d);
    }

    perlin_noise_parameters parameters;
    float u = 0.5f + x/terrain_length;
    float v = 0.5f + y/terrain_length;
    float const noise = noise_perlin({u, v}, parameters.octave, parameters.persistency, parameters.frequency_gain);

    return z*noise;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);
    terrain.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y,terrain_length);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
            terrain.uv[kv+N*ku] = {6*v,6*u};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length){
    std::vector<cgp::vec3> positions(N);
    for(auto& p : positions) {
        float x = rand_interval(-terrain_length/2, terrain_length/2);
        float y = rand_interval(-terrain_length/2, terrain_length/2);
        p = vec3{x, y, evaluate_terrain_height(x,y,terrain_length)};
    }
    return positions;
}

