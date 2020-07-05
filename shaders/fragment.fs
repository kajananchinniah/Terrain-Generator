#version 330 core

struct Light
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Color;
in vec3 Normal;

out vec4 FragColor; 

uniform vec3 viewPos;
uniform Light light;

void main()
{
    float levels = 4.0f;
    // ambient 
    vec3 ambient = light.ambient * Color;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    float level = floor(diff * levels);
    diff = level / levels;
    vec3 diffuse = light.diffuse * diff * Color;

    // specular 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    level = floor(spec * levels);
    spec = level / levels;
    vec3 specular = light.specular * spec * Color;

    vec3 result = ambient + diffuse + specular; 

    FragColor = vec4(Color, 1.0f);
}

