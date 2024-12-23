#version 430 core

layout (location = 0) in vec2 aP1;
layout (location = 1) in vec2 aP2;
layout (location = 2) in int aColor;
layout (location = 3) in float aThickness;

out mat4 vProj;

out vec4 vColor; 
out vec2 vP1; 
out vec2 vP2; 
out float vThickness; 

layout(std140, binding = 0) uniform RenderData
{
    mat4 ViewProjection;
	int ScreenWidth, ScreenHeight; 
};



void main()
{
	float R = ((aColor) & 255) / 255.0;
	float G = ((aColor >> 8) & 255) / 255.0;
	float B = ((aColor >> 16) & 255) / 255.0;
	float A = ((aColor >> 24) & 255) / 255.0;
	
    vProj = ViewProjection; 
    //vColor = vec4(R, G, B, A); 
    vColor = vec4(R, G, B, 1); 
    vP1 = aP1; 
    vP2 = aP2; 
	vThickness = aThickness;
}
