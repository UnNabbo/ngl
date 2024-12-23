#version 460 core

out vec4 FragColor;

in vec4 gColor;

void main()
{
	vec4 Color = gColor;
	
	FragColor = Color;
}
