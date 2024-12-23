#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in mat4 vProj[]; 

in vec4 vColor[]; 
in vec4 vDim[];
in vec4 vUVs[];

in int vTextureID[];
in float vAngle[];

in int vScreenHeight[];
in int vScreenWidth[];

out vec4 gColor; 
flat out vec2 gTexCoord; 
flat out vec4 gDim;

flat out int gTextureID; 
flat out int gPrimitive;

flat out int gScreenHeight;
flat out int gScreenWidth;

vec2 Rotate(vec2 v, vec2 pivot, mat2 RotMat) {
	return RotMat * (v - pivot) + pivot;
}

void MakeQuad(vec4 Dim)
{   
	gColor = vColor[0];
	gTextureID = vTextureID[0];
	gPrimitive = 0;
	gScreenHeight = vScreenHeight[0];
	gScreenWidth = vScreenWidth[0];
	
	gDim = Dim;
    // a: left-bottom 
	
	float Angle = vAngle[0];
	float s = sin(Angle);
	float c = cos(Angle);
	mat2 RotMat = mat2(c, s, -s, c);
	
	vec2 Pivot = vec2(Dim.x + ((Dim.z - Dim.x) * 0.5), Dim.y + ((Dim.w - Dim.y) * 0.5));
	
	vec4 UVs = vUVs[0]; 
	
    // a: left-bottom 
    gTexCoord = vec2(UVs.x,UVs.y);
    gl_Position = vec4(Rotate(Dim.xw, Pivot, RotMat), 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // b: left-top
    gTexCoord = vec2(UVs.x,UVs.w);
    gl_Position = vec4(Rotate(Dim.xy, Pivot, RotMat), 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // d: right-bottom
    gTexCoord = vec2(UVs.z,UVs.y);
    gl_Position = vec4(Rotate(Dim.zw, Pivot, RotMat), 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // c: right-top
    gTexCoord = vec2(UVs.z,UVs.w);
    gl_Position = vec4(Rotate(Dim.zy, Pivot, RotMat), 0, 1)* vProj[0] ;
    EmitVertex();  
	
    EndPrimitive();	
}

void main() {    
    MakeQuad(vDim[0]);
}