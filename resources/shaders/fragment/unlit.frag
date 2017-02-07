in vec2 TexCoords;

layout(location = 0) out vec4 fragColor;

uniform sampler2D AlbedoMap;

void main()
{
	//Sample textures
	vec4 albedoDiffuseColor = texture(AlbedoMap,TexCoords);
		
	fragColor = albedoDiffuseColor;
}