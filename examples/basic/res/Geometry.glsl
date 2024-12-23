#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in mat4 vProj[]; 

in vec4 vColor[]; 
in vec4 vDim[];
in vec4 vUVs[];

in int vTextureID[];
in int vPrimitive[];

out vec4 gColor; 
flat out vec2 gTexCoord; 
flat out vec4 gDim;

flat out int gTextureID; 
flat out int gPrimitive;

void MakeQuad(vec4 Dim)
{   
	gColor = vColor[0];
	gTextureID = vTextureID[0];
	gPrimitive = vPrimitive[0];
	
	gDim = Dim;
    // a: left-bottom 
	
	vec4 UVs = vUVs[0]; 
	
    // a: left-bottom 
    gTexCoord = vec2(UVs.x,UVs.y);
    gl_Position = vec4(Dim.x, Dim.w, 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // b: left-top
    gTexCoord = vec2(UVs.x,UVs.w);
    gl_Position = vec4(Dim.x, Dim.y, 0, 1)* vProj[0] ;
    EmitVertex();  

    // d: right-bottom
    gTexCoord = vec2(UVs.z,UVs.y);
    gl_Position = vec4(Dim.z, Dim.w, 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // c: right-top
    gTexCoord = vec2(UVs.z,UVs.w);
    gl_Position = vec4(Dim.z, Dim.y, 0, 1)* vProj[0] ;
    EmitVertex();  
	
    EndPrimitive();	
}

void main() {    
    MakeQuad(vDim[0]);
}