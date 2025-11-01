#version 330

uniform mat4 uModelView;
uniform mat4 uProjection;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

out vec3 fragPosition;
out vec3 normal;

void main()
{
	fragPosition = vec3(uModelView * vec4(inPosition, 1.0));
	normal = mat3(transpose(inverse(uModelView))) * inNormal;

	gl_Position = uProjection * vec4(fragPosition, 1.0);
}
