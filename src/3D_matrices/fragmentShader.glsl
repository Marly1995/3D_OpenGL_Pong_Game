#version 330
in vec3 fragmentColor;
in vec3 fragmentPosition;
in vec2 Texture;

uniform sampler2D tex;
uniform vec3 lightPosition; 
uniform vec3 cameraPosition;
uniform vec3 lightColor;
uniform int texBool;

out vec4 outputColor;
void main()
{
	//ambient light
	float ambientStrength = 0.6f;
	vec3 ambient = ambientStrength * lightColor;
	
	//Diffuse lighting
	vec3 normal = normalize(fragmentColor);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;
	
	//Specular lighting
	float specularStrength = 0.9f;
	vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * fragmentColor;

		outputColor = texture(tex, Texture) * vec4(result, 1.0f);

}
