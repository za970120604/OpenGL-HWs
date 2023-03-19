#version 330 core

uniform sampler2D Texture;
in vec2 texCoord;
in vec3 Normal;
in vec3 Pos;
out vec4 fragColor ; 

uniform vec3 light_pos;
uniform vec3 diffuse_m;

void main()
{	
	vec3 L = normalize(light_pos - Pos);
	vec3 N = normalize(Normal);
	vec3 Kd = diffuse_m;
	vec4 object_color = texture(Texture, texCoord);
	vec3 intensity = vec3(0 , 0 , 0);
	if(dot(L , N) >= 0.7){
		intensity = vec3(1 , 1 , 1) ;
	}
	else if(dot(L , N) <= 0.7 && dot(L , N) >= 0.4){
		intensity = vec3(0.7 , 0.7 , 0.7) ;
	}
	else if(dot(L , N) <= 0.1 && dot(L , N) >= 0.4){
		intensity = vec3(0.4 , 0.4 , 0.4) ;
	}
	else{
		intensity = vec3(0.2 , 0.2 , 0.2) ;
	}
	fragColor = vec4(Kd * vec3(object_color.x , object_color.y , object_color.z) * intensity , 1.0) ;
}