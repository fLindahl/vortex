uniform vec4 color;

in float diffuse;
out vec4 diffuseColor;

void main()
{
	diffuseColor = vec4(color.xyz*diffuse, color.w);
}
