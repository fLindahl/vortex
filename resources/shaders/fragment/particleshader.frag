uniform sampler2D AlbedoMap;
//in float diffuse;
in vec2 TexCoords;
in vec4 inColor;
in float render;
layout(location = 0) out vec4 diffuseColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec4 specularAndRoughness;

const int spriteX = 8;
const int totSprites = 64;

void main()
{
	vec4 text = texture(AlbedoMap, TexCoords);
	vec4 col = text*inColor;

	diffuseColor = col;
	specularAndRoughness = vec4(0.0f,0.0f,0.0f,1.0f);
}
