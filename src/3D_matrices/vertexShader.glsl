#version 330
in vec3 position;
in vec3 vertexColor;
in vec2 texture;

out vec3 fragmentPosition;
out vec3 fragmentColor;
out vec2 Texture;

uniform mat4 modelMatrix      = mat4(1.0);
uniform mat4 viewMatrix       = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 rotateMatrix = mat4(1.0);

void main()
{
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * rotateMatrix * vec4(position, 1.0);
		fragmentColor =  mat3(transpose(inverse(modelMatrix))) * vertexColor; 
		fragmentPosition = vec3(modelMatrix * rotateMatrix * vec4(position, 1.0f));
		Texture = texture;
}
