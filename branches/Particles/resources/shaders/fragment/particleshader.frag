uniform sampler2D AlbedoMap;
//in float diffuse;
in vec2 TexCoords;
in vec4 inColor;
in float render;
out vec4 diffuseColor;


void main()
{
	vec4 text = texture(AlbedoMap, TexCoords);
	vec4 col = text*inColor;
	if(col.a  < 0.05)
		discard;
	diffuseColor = col;
}
