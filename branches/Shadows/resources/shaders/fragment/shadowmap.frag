/// Shadow Map ///

void main() 
{
	gl_FragDepth = ((0.5 * Projection[3][2]) / (gl_FragCoord.z + 0.5 * Projection[2][2] - 0.5) / 999.95f);
}