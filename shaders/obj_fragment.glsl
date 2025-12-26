#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;

struct Light {
    vec3 position;
    vec3 direction; // for flashlight
    float cutoff;   // for flashlight

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
uniform Light flashlight;

struct Material {
    sampler2D diffuse_tex;
    sampler2D specular_tex;
    sampler2D emissive_tex;
    float shininess;
};

uniform Material material;

void main()
{
    // ambient lighting
    vec3  ambient = light.ambient * vec3(texture(material.diffuse_tex, TexCoord));

    // diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_tex, TexCoord));

    // specular lighting
    float specular_strength = 0.6;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 256.0);
    vec3 specular = light.specular * spec * vec3(texture(material.specular_tex, TexCoord));

    // emissive lighting
    //vec3 emitt = vec3(texture(material.emissive_tex, TexCoord));

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
        light.quadratic * (distance * distance));

    vec3  result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}
