#version 330

uniform vec3 uLightPosition;
uniform vec3 uViewPosition;
uniform vec3 uLightColor;
uniform vec3 uObjectColor;

in vec3 fragPosition;
in vec3 normal;

out vec4 fragColor;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(uLightPosition - fragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPosition - fragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * uLightColor;

	vec3 result = (ambient + diffuse + specular) * uObjectColor;
	fragColor = vec4(result, 1.0);
}
