#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    // ambient lighting
    float ambient_strength = 0.1;
    vec3  ambient = ambient_strength * lightColor;

    // diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;

    // specular lighting
    float specular_strength = 0.6;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specular_strength * spec * lightColor;  

    vec3  result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
