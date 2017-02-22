in float VSDepth;

layout(location = 0) out float depth;

void main() 
{
	float far = 1000.0f;
	float near = 0.05f;

	//Linearize depth so that we can use it later
	
	depth = -VSDepth;
	//depth = gl_FragCoord.z;
		
	// DO THIS IF YOU WANT TO LINEARIZE FROM GL Z INSTEAD
	//depth = (0.5 * Projection[3][2]) / (gl_FragCoord.z + 0.5 * Projection[2][2] - 0.5) + 1.0f;
	
	//depth = ((near * far) / (gl_FragCoord.z * (near - far) + far));
	//			(50 / (0.0 * (-999.95) + 1000)) / (-999.95)
}
