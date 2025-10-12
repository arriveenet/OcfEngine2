#version 330

layout(location = 0) in vec3 inPosition;

out vec3 fragTexCoord;

uniform mat4 uMVPMatrix;

void main()
{
	vec4 pos = uMVPMatrix * vec4(inPosition, 1.0);
	gl_Position = pos.xyww;

	fragTexCoord = inPosition;
}
