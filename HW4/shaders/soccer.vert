#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 normalM;

uniform float h ;

out vec2 texCoord;
out vec3 normal;

void main()
{	
	vec3 obj_pos = aPos ;
	obj_pos.y += h;
	gl_Position = P * V * M * vec4(obj_pos, 1.0);
	texCoord = aTexCoord;
	normal = normalize((normalM * vec4(aNormal, 0.0)).xyz);;
}