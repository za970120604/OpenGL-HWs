#version 330 core

// TODO:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Model ; 
uniform mat4 View ; 
uniform mat4 Perspective ; 

uniform vec3 ambient_m;
uniform vec3 diffuse_m;
uniform vec3 specular_m;
uniform float gloss;

uniform vec3 ambient_l;
uniform vec3 diffuse_l;
uniform vec3 specular_l;
uniform vec3 light_pos;

uniform vec3 camera_pos;

out vec3 ambient ;
out vec3 diffuse ; 
out vec3 specular ;
out vec2 texCoord;

void main()
{
	gl_Position = Perspective * View * Model * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	vec3 Normal = mat3(transpose(inverse(Model))) * aNormal;  
	vec3 Pos = mat3(Model) * aPos;

	vec3 L = normalize(light_pos - Pos);
	vec3 V = normalize(camera_pos - Pos);
	vec3 N = normalize(Normal);
	vec3 R = normalize(2*(dot(L , N))*N-L) ;

	vec3 Ka = ambient_m;
	vec3 Kd = diffuse_m;
	vec3 Ks = specular_m;

	vec3 La = ambient_l;
	vec3 Ld = diffuse_l;
	vec3 Ls = specular_l;

	ambient = La * Ka ;
	diffuse = Ld * Kd  * max(dot(L,N) , 0);
	specular = Ls * Ks *  pow(max(dot(V , R) , 0) , gloss) ;

}