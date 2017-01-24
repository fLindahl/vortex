layout(location = 0) out float depth;

void main() 
{
	//Linearize depth so that we can use it later
	depth = (0.5 * Projection[3][2]) / (gl_FragCoord.z + 0.5 * Projection[2][2] - 0.5);
}
