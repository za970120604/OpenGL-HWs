#version 330 core

uniform sampler2D Texture ;

in vec2 texCoord;
in vec3 ambient ;
in vec3 diffuse ; 
in vec3 specular ;
out vec4 fragColor ; 

void main()
{	
	vec4 object_color = texture(Texture , texCoord);
	vec3 a_color = ambient * object_color.xyz;
	vec3 d_color = diffuse * object_color.xyz;
	vec3 s_color = specular ;
	fragColor = vec4(a_color + d_color + s_color , 1.0);
}