#version 330

uniform mat4 uMVPMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = uMVPMatrix * pos;
	fragTexCoord = inTexCoord;
}
