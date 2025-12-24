#version 330 core
in vec3 color;
in vec2 texCoord;

/*
uniform float greenValue;
uniform float redValue;
uniform float blueValue;
uniform sampler2D ourTex;

out vec4 FragColor;

void main()
{
    vec4 yuh = vec4(color.x + redValue, color.y + greenValue, color.z + blueValue, 1.0);
    FragColor = texture(ourTex, texCoord) * yuh;
}
*/
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0);
}
