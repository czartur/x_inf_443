#include "tree.hpp"

using namespace cgp;


mesh create_cylinder_mesh(float radius, float height)
{
    mesh m;
    int maximalSize = 40;
    m.position.resize(maximalSize);
    for(int i=0; i<maximalSize; i++){
        float angle = 2*Pi*(i/2)/(maximalSize/2);
        m.position[i] = vec3{radius*cos(angle), radius*sin(angle), height*(i&1)};
    }

    for(int i=0; i<maximalSize; i++){
        if(i&1) m.connectivity.push_back(uint3{i, (i+1)%maximalSize, (i+2)%maximalSize});
        else m.connectivity.push_back(uint3{i, (i+2)%maximalSize, (i+1)%maximalSize});
    }
    m.fill_empty_field(); 
    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m;
    m.position.push_back(vec3{0, 0, z_offset});
    int baseDimension = 20;
    for(int i=0; i<baseDimension; i++) {
        float angle = 2*Pi*(float)i/(float)baseDimension;
        m.position.push_back(vec3{radius*cos(angle), radius*sin(angle), z_offset});
    }
    m.position.push_back(vec3{0, 0, height + z_offset});
    for(int i=0; i<baseDimension; i++){
        m.connectivity.push_back(uint3{0, i+1, 1+((i+1)%baseDimension)});
        m.connectivity.push_back(uint3{1+baseDimension, i+1, 1+((i+1)%baseDimension)});
    }
    m.fill_empty_field();
    return m;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.3f, 0.3f});


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4*r, 6*r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4*r, 6*r, 2*r));   // middle-cone
    foliage.push_back(create_cone_mesh(4*r, 6*r, 4*r));   // top-cone
    foliage.apply_translation_to_position({0,0,h});       // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});
       
    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}

