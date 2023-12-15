#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float health_percentage;

// Output values to fragment shader
out vec3 world_pos;
out vec3 world_normal;

void main()
{
    world_pos = (Model * vec4(v_position,1)).xyz;
    world_normal = normalize( mat3(Model) * v_normal );

    // choose a displacement amount
    float displacement = (1 - health_percentage) * 0.2 * sin(v_position.x);
    // displace it in the direc tion of the normal
    vec4 displacementDirection = vec4(1, 1, 0, 0) * vec4(v_normal, 0);
    
    gl_Position = Projection * View * Model * (vec4(v_position, 1.0) + displacement * displacementDirection);
}
