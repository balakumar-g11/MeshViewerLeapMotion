#version 330 core

layout(location=0) in vec3 vertexPos_modelspace;
layout(location=1) in vec3 vertexNormal;

out vec3 Vertex0;
out vec3 Normal0;

uniform mat4 MVP;
uniform mat4 Normal;

void main(){
	
	vec4 vPos = vec4(vertexPos_modelspace, 1.0);
	Vertex0 = vPos.xyz;
	Normal0 = (Normal * vec4(vertexNormal, 0.0)).xyz;
	
	gl_Position = MVP * vPos;
}
