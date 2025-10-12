#version 330

uniform mat4 uMVPMatrix;

layout(location = 0) in vec3 inPosition;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = uMVPMatrix * pos;
}