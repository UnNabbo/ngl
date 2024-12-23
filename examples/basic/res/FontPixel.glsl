#version 460 core

out vec4 FragColor;
in vec4 gColor;
in vec2 gTexCoord;

flat in int gTextureID; 

#define TEXUTURE_DECL(n)  layout(binding = n) uniform sampler2D uTexture##n
#define TEXUTURE_MUL(n)  case  n: glyphShape = texture(uTexture##n, gTexCoord).r; break

TEXUTURE_DECL(0);
TEXUTURE_DECL(1);
TEXUTURE_DECL(2);
TEXUTURE_DECL(3);
TEXUTURE_DECL(4);
TEXUTURE_DECL(5);
TEXUTURE_DECL(6);
TEXUTURE_DECL(7);
TEXUTURE_DECL(8);
TEXUTURE_DECL(9);
TEXUTURE_DECL(10);
TEXUTURE_DECL(11);
TEXUTURE_DECL(12);
TEXUTURE_DECL(13);
TEXUTURE_DECL(14);
TEXUTURE_DECL(15);

void main()
{
	float glyphShape = 0;
	
	switch(gTextureID)
	{
		TEXUTURE_MUL(1);
		TEXUTURE_MUL(2);
		TEXUTURE_MUL(3);
		TEXUTURE_MUL(4);
		TEXUTURE_MUL(5);
		TEXUTURE_MUL(6);
		TEXUTURE_MUL(7);
		TEXUTURE_MUL(8);
		TEXUTURE_MUL(9);
		TEXUTURE_MUL(10);
		TEXUTURE_MUL(11);
		TEXUTURE_MUL(12);
		TEXUTURE_MUL(13);
		TEXUTURE_MUL(14);
		TEXUTURE_MUL(15);
	}
	
	
	if(glyphShape > 0.5){
		FragColor = gColor;
	} 
}
