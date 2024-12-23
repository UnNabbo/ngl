#version 430 core

layout (location = 0) in vec4 aDim;
layout (location = 1) in int aColor;
layout (location = 2) in ivec2 aUVs;
layout (location = 3) in int TextureID;
layout (location = 4) in float Angle;

out mat4 vProj;

out vec4 vColor; 
out vec4 vUVs; 
out vec4 vDim; 

out int vTextureID;
out float vAngle;

out int vScreenHeight;
out int vScreenWidth;

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
	
	float Left = (aUVs.x & 65535) / 65535.0;
	float Top = ((aUVs.x >> 16) & 65535) / 65535.0;
	float Right = (aUVs.y & 65535) / 65535.0;
	float Bottom = ((aUVs.y >> 16) & 65535) / 65535.0;
	
    vProj = ViewProjection; 
    vColor = vec4(R, G, B, A); 
    vDim = aDim; 
	vUVs = vec4(Left, Bottom, Right, Top);
	vTextureID = TextureID;
	vAngle = Angle;
	
	vScreenWidth = ScreenWidth;
	vScreenHeight = ScreenHeight;
}
