layout(location=0) in vec2 Texcoords;
in vec3 Normal;
in vec3 toLight;
in vec3 toCamera;

out vec4 outColor;
uniform sampler2D texSampler;

void main()
{	
	float intensity;
	vec4 color;
	intensity = dot(toLight,normalize(Normal));

	float intensity2 = dot(toCamera, normalize(Normal));
	
	// diffuse color of the object from texture
	vec3 diffuseColor = texture(texSampler,Texcoords).rgb;
	
	if (intensity > 0.95)
		color = vec4(intensity2,intensity2,intensity2,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.6,0.6,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.4,0.4,1.0);
	else
		color = vec4(0.2,0.2,0.2,1.0);
		
	outColor = normalize(color * vec4(diffuseColor, 1));
}