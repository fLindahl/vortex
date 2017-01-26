uniform sampler2D AlbedoMap;
//in float diffuse;
in vec2 TexCoords;
in vec4 inColor;
out vec4 diffuseColor;

void main()
{

	diffuseColor = texture(AlbedoMap, TexCoords)*inColor;
}
