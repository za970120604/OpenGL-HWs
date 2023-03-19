#version 330 core
uniform sampler2D Texture;
in vec2 texCoord;
in vec3 Normal;
in vec3 Pos;

uniform vec3 camera_pos;
out vec4 fragColor ; 
void main()
{	
	vec3 V = normalize(camera_pos - Pos);
	vec3 N = normalize(Normal);
	if(abs(dot(V , N)) < 0.15){
		fragColor = vec4(0 , 0 , 1 , 1);
	}
	else if(abs(dot(V , N)) < 0.3){
		fragColor = vec4(0 , 0 , abs(pow(dot(V , N) , 2)) , 1);
	}
	else if(abs(dot(V , N)) < 0.5){
		fragColor = vec4(0 , 0 , abs(pow(dot(V , N) , 4)) , 1);
	}
	else{
		fragColor = vec4(0 , 0 , 0 , 1);
	}
	//fragColor = vec4(0 , 0 , abs(pow(dot(V , N) , 2)) , 1);
}