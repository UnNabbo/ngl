#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in mat4 vProj[]; 

in vec4 vColor[]; 

in vec2 vP1[];
in vec2 vP2[];
in float vThickness[]; 

out vec4 gColor; 


void MakeQuad(vec2 p1, vec2 p2)
{   
	gColor = vColor[0];
    // a: left-bottom 
	
	vec3 c = cross(vec3(abs(p2.x - p1.x), abs(p2.y - p1.y), 0), vec3(0, 0, 1));
	vec3 n = normalize(c) * vThickness[0];
	
	vec2 v1 = p1 + n.xy;
	vec2 v2 = p1 - n.xy;
	vec2 v3 = p2 + n.xy;
	vec2 v4 = p2 - n.xy;
	
    // a: left-bottom 
	gl_Position = vec4(v1, 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // b: left-top
	gl_Position = vec4(v2, 0, 1)* vProj[0] ;
    EmitVertex();  

    // d: right-bottom
	gl_Position = vec4(v3, 0, 1)* vProj[0] ;
    EmitVertex();  
	
    // c: right-top
	gl_Position = vec4(v4, 0, 1)* vProj[0] ;
    EmitVertex();  
	
    EndPrimitive();	
}

void main() {    
    MakeQuad(vP1[0], vP2[0]);
}