#version 330 core
// TO DO:
// Implement vertex shader
// note: remember to set gl_Position

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 normal;

uniform mat4 M ; 
uniform mat4 V ; 
uniform mat4 P ; 
uniform mat4 normalM;

uniform float time , xs , zs ;


void main()
{	
	
	float s = 1.0 + 0.1*sin(xs*time)*sin(zs*time) ;
	gl_Position = P * V * M *vec4(aPos.x , aPos.y*s , aPos.z , 1);
	texCoord = aTexCoord;
	normal = normalize((normalM * vec4(aNormal, 0.0)).xyz);
}