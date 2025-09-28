#version 330

uniform mat4 uMVPMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec3 inColor;

out vec3 fragColor;

void main()
{
	gl_Position = uMVPMatrix * vec4(inPosition, 1.0);

	fragColor = inColor;
}
