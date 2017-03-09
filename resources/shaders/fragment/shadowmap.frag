/// Shadow Map ///
uniform mat4 lightProj;

void main() 
{
	//doesnt need to do anything
	//gl_FragDepth = ((0.5 * lightProj[3][2]) / (gl_FragCoord.z + 0.5 * lightProj[2][2] - 0.5) /99.95f);	
}