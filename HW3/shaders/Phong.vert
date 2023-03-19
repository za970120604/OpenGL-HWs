#version 330 core

// TODO:
// Implement Phong shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Model ; 
uniform mat4 View ; 
uniform mat4 Perspective ; 

out vec2 texCoord;
out vec3 Normal;
out vec3 Pos;
void main()
{
	gl_Position = Perspective * View * Model * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	Normal = mat3(transpose(inverse(Model))) * aNormal;  
	Pos = mat3(Model) * aPos;
}