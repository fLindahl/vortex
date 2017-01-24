uniform vec4 color;

//in float diffuse;
in vec4 inColor;
out vec4 diffuseColor;

void main()
{
	diffuseColor = vec4(color.xyz, color.w);
}
