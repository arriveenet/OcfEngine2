#version 330

uniform mat4 uMVPMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main()
{
	gl_Position = uMVPMatrix * vec4(inPosition, 1.0);
	fragTexCoord = inTexCoord;
}
