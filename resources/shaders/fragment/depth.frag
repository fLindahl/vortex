layout(location = 0) out float depth;

void main() 
{
	float far = 1000.0f;
	float near = 0.05f;

	//Linearize depth so that we can use it later
	depth = (0.5 * Projection[3][2]) / (gl_FragCoord.z + 0.5 * Projection[2][2] - 0.5);
	//depth = (2 * near) / (far + near -  gl_FragCoord.z * (far - near));
	//depth = (near * far) / (gl_FragCoord.z * (near - far) + far);
	
	
	//const double z_f = farPlaneZ();
    //const double z_n = nearPlaneZ();
    //Vector3(float(z_n * z_f),  float(z_n - z_f),  float(z_f));
}
