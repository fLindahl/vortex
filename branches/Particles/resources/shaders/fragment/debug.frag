uniform vec4 color;

in float depth;
out vec4 diffuseColor;

void main()
{
	diffuseColor = color*depth;
}
