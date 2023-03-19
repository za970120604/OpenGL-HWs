#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;
uniform float transparency ;
uniform vec3 color_change ;
uniform int is_change ;

out vec4 FragColor;

void main()
{	if(is_change == 1){
		FragColor = vec4(texture(ourTexture, texCoord).rgb * color_change , 1) ;
	}
	else{
		FragColor = vec4(texture(ourTexture, texCoord).rgb , min(transparency ,1));
	}
}