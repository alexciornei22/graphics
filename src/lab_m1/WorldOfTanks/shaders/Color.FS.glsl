#version 330

// Input
in vec3 world_pos;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

float point_light_contribution(vec3 light_pos)
{
    vec3 L = normalize(light_pos - world_pos);
    vec3 V = normalize(eye_position - world_pos);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-L, world_normal);

    float ambient_light = 0.5;

    float diffuse_light = material_kd * max(dot(world_normal,L), 0);

    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(0, dot(V, R)), material_shininess);
    }
    
    float d = distance(light_pos, world_pos);
    float attenuation_factor = 1;
    float light = ambient_light + attenuation_factor * (diffuse_light + specular_light);

    return light;
}

void main()
{
    float light = point_light_contribution(light_position);
    out_color = vec4(object_color * light, 1);
}
