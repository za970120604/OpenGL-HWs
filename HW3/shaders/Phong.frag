#version 330 core

uniform sampler2D Texture;
in vec2 texCoord;
in vec3 Normal;
in vec3 Pos;

out vec4 fragColor ; 

uniform vec3 ambient_m;
uniform vec3 diffuse_m;
uniform vec3 specular_m;
uniform float gloss;

uniform vec3 ambient_l;
uniform vec3 diffuse_l;
uniform vec3 specular_l;
uniform vec3 light_pos;

uniform vec3 camera_pos;
 
void main()
{	
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

	vec4 object_color = texture(Texture, texCoord);
	vec3 ambient = La * Ka * vec3(object_color.x , object_color.y , object_color.z);
	vec3 diffuse = Ld * Kd * vec3(object_color.x , object_color.y , object_color.z) * max(dot(L,N) , 0);
	vec3 specular = Ls * Ks *  pow(max(dot(V , R) , 0) , gloss) ;
	fragColor = vec4(ambient + diffuse + specular , 1.0);
}