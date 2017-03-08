///MSM Shadow Map ///

uniform mat4 lightProj;
out vec4 FragColor;
										
/*mat4 basispolynomialsmat = mat4(1.5f, 0.0f, -2.0f, 0.0f,
								0.0f, 4.0f, 0.0f, -4.0f,
								0.8660254037844386f, 0.0f, -0.3849001794597505f, 0.0f,
								0.5f, 0.0f, 0.5f, 0.0f);*/

void main() 
{
	//doesnt need to do anything
	//gl_FragDepth = ((0.5 * lightProj[3][2]) / (gl_FragCoord.z + 0.5 * lightProj[2][2] - 0.5) /99.95f);
	//gl_FragDepth = (0.5 * lightProj[3][2]) / (gl_FragCoord.z + 0.5 * lightProj[2][2] - 0.5); //linearize depth
	
	//for VSM store depth, depth*depth in a colour fbo
	float depth = gl_FragCoord.z ;// gl_FragCoord.w;
	//depth = depth * 0.5 + 0.5;  //move to [0,1]
	
	float depth1 = depth;
	float depth2 = depth*depth;
	//float depth3 = depth2*depth;
	//float depth4 = depth3*depth;
	
	//small filtering of the colour
	
	//vec4 depthvec = vec4(depth1,depth2,depth3,depth4);
	//mat4 transposed = transpose(basispolynomialsmat);
	//vec4 color = transposed * depthvec + vec4(0.5f,0.0f,0.5f,0.0);
	//FragColor = color;
						
	//adjust moments with a sort of bias for vsm
	float dx = dFdx(depth1);
	float dy = dFdy(depth1);
	depth2 += 0.25*(dx*dx+dy*dy);
	
	FragColor = vec4(depth1, depth2, 0, 0);
}
