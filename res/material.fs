#version 330 core

out vec4 o_colour;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirectionalLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight 
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

// in variables

in vec3 v_frag_position;
in vec3 v_normal;
in vec2 v_texcoord;

// uniforms

uniform vec3                u_view_position;
uniform DirectionalLight    u_directional_light;
uniform PointLight          u_point_lights[NR_POINT_LIGHTS];
uniform Spotlight           u_spotlight;
uniform Material            u_material;

// function prototypes

vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction);
vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction);
vec3 calculate_spotlight(Spotlight light, vec3 normal, vec3 frag_position, vec3 view_direction);

void main()
{    
    // properties
    vec3 norm           = normalize(v_normal);
    vec3 view_direction = normalize(u_view_position - v_frag_position);
    
    // phase 1: directional lighting
    vec3 result = calculate_directional_light(u_directional_light, norm, view_direction);
    
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calculate_point_light(u_point_lights[i], norm, v_frag_position, view_direction);    
    
    // phase 3: spotlight
    result += calculate_spotlight(u_spotlight, norm, v_frag_position, view_direction);    
    
    o_colour = vec4(result, 1.0);
}

// calculates the color when using a directional light
vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec             = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    
    // combine results
    vec3 ambient  = light.ambient * vec3(texture(u_material.diffuse, v_texcoord));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(u_material.diffuse, v_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_texcoord));

    vec3 result = ambient + diffuse + specular;
    return result;
}

// calculates the color when using a point light
vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - frag_position);
    
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec             = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    
    // attenuation
    float distance    = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient  = light.ambient * vec3(texture(u_material.diffuse, v_texcoord));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(u_material.diffuse, v_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_texcoord));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}

// calculates the color when using a spotlight
vec3 calculate_spotlight(Spotlight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - frag_position);
    
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec             = pow(max(dot(view_direction, reflect_direction), 0.0), u_material.shininess);
    
    // attenuation
    float distance = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // spotlight intensity
    float theta     = dot(light_direction, normalize(-light.direction)); 
    float epsilon   = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
    
    // combine results
    vec3 ambient  = light.ambient * vec3(texture(u_material.diffuse, v_texcoord));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(u_material.diffuse, v_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_texcoord));
    
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    vec3 result = ambient + diffuse + specular;
    return result;
}