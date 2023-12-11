#version 330

// Input
in vec3 world_pos;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 point_light_pos[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform bool is_spotlight;
uniform float cutOff;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_pos)
{
    // TODO(student): Define ambient, diffuse and specular light components
    vec3 L = normalize( light_pos - world_pos );
    vec3 V = normalize( eye_position - world_pos );
    vec3 H = normalize( L + V );
    vec3 R = reflect(-L, world_normal);

    // TODO(student): Define ambient light component
    float ambient_light = 0.25;
    
    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max(dot(world_normal,L), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(0, dot(V, R)), material_shininess);
    }

    float light = ambient_light;
    
    if (is_spotlight)
    {
        float cut_off = radians(cutOff);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        if (spot_light > spot_light_limit)
        {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
	        // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	        // se calculeaza atenuarea luminii
	        float d = distance(light_pos, world_pos);
            float attenuation_factor = 1 / (1 + 0.1 * d + 0.05 * d * d);
	        light = ambient_light + light_att_factor * attenuation_factor * (diffuse_light + specular_light);
        }
    }
    else 
    {
        // TODO(student): Compute light
        float d = distance(light_pos, world_pos);
        float attenuation_factor = 1 / (1 + 0.1 * d + 0.05 * d * d);
	    light = ambient_light + attenuation_factor * (diffuse_light + specular_light);
    }
	
	return object_color * light;
}

void main()
{    
    vec3 light1 = point_light_contribution(point_light_pos[0]);
    vec3 light2 = point_light_contribution(point_light_pos[1]);
    out_color = vec4(light1 + light2, 1);
}
