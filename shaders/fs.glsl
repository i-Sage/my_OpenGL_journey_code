#version 450 core
out vec4 FragColor;

// texture samplers
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor * objectColor, 1.0);
}
