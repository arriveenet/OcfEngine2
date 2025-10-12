#version 330

out vec4 fragColor;

in vec3 fragTexCoord;

uniform samplerCube skybox;

void main()
{
	fragColor = texture(skybox, fragTexCoord);
}
