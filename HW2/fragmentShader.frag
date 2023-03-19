#version 330 core
// TO DO:
// Implement fragment shader
in vec2 texCoord;
out vec4 fragColor ; 
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float ratio ;
void main()
{	
	fragColor = mix(texture(Texture1, texCoord), texture(Texture2, texCoord), ratio);
}