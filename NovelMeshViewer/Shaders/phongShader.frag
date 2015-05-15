#version 330 core

uniform vec4 ambMaterial;
uniform vec4 diffuseMaterial;
uniform vec4 speculatMaterial;
uniform float specularPower;
uniform vec3 lightPos;

in vec3 Vertex0;
in vec3 Normal0;

out vec4 color;

void main()
{
	vec3 lightDirection = normalize(lightPos - Vertex0);
	vec3 E = normalize(-Vertex0);
	vec3 R = reflect(-lightDirection, normalize(Normal0));

	float cosTheta = clamp(max(dot(normalize(Normal0), -lightDirection), 0.0), 0.0, 1.0);

	vec4 diffuse = diffuseMaterial * cosTheta;

	vec4 specular = clamp(speculatMaterial * pow(max(dot(R, E), 0.0), specularPower), 0.0, 1.0);
	
	color = (0.5*ambMaterial) + diffuse + specular;
}