uniform sampler2D AlbedoMap;
//in float diffuse;
in vec2 TexCoords;
in vec4 inColor;
in float render;
out vec4 diffuseColor;

const int spriteX = 8;
const int totSprites = 64;

void main()
{
	vec4 text = texture(AlbedoMap, TexCoords);
	vec4 col = text*inColor;

	diffuseColor = col;
}
