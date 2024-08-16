#version 330 core
out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 spec;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 spec;
};

in vec3 Normal;
in vec3 FragPos;
// in vec2 TexCoord;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm            = normalize(Normal);
    vec3 light_direction = normalize(light.position - FragPos);
    float diff           = max(dot(norm, light_direction), 0.0);
    vec3 diffuse         = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 view_direction     = normalize(viewPos - FragPos);
    vec3 reflect_direction  = reflect(-light_direction, norm);
    float spec              = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular           = light.spec * (spec * material.spec);
        
    vec3 result = ambient + diffuse + specular;
    FragColor   = vec4(result, 1.0);
} 