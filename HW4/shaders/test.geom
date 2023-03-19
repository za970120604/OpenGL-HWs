#version 330 core

layout (triangles) in;
layout (points ,  max_vertices = 4)out;

in VS_OUT {
	vec3 normal;
	vec2 texCoord;
} gs_in[];

uniform mat4 P;
uniform float start_t ; 
uniform float cur_t ;
uniform float v0 ;
uniform float a;

out vec3 color;

void main()
{
	float real_time = cur_t - start_t ;
	for(int i = 0 ; i < 3 ; i++){
		gl_Position = P * vec4(gl_in[i].gl_Position.xyz + gs_in[i].normal * (v0 * real_time + 0.5 * a * pow(real_time , 2)) , 1.0)  ;
		color = vec3(0.0);
		color[i] = 1.0 ;
		EmitVertex() ;
	}
	vec3 avg_normal = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal)/3 ; 
	vec3 avg_plc = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz)/3 ; 
	gl_Position = P * vec4(avg_plc + avg_normal * (v0 * real_time + 0.5 * a * pow(real_time , 2)) , 1.0)  ;
	color = vec3(1 , 1 , 1) ;
	EmitVertex();
	EndPrimitive();

}