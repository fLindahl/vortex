layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

out vec3 FragmentPos;
out vec2 TexCoords;
out mat3 NormalMatrix;

void main()
{
	
	mat4 modelView = View * Model;
  
	modelView[0][0] = 1.0; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0; 

	modelView[1][0] = 0.0; 
	modelView[1][1] = 1.0; 
	modelView[1][2] = 0.0; 

	modelView[2][0] = 0.0; 
	modelView[2][1] = 0.0; 
	modelView[2][2] = 1.0; 
	
	vec4 wPos = (Model * vec4(pos, 1.0f));
	
	// position in world space
	FragmentPos = wPos.xyz;	
			
	TexCoords.x = uv.x;
	TexCoords.y = 1 - uv.y;

	mat3 model33 = mat3(Model);
	
	vec3 v0 = vec3(0.0f,0.0f,0.0f);
	vec3 n = normalize(model33 * (normal));
	
	NormalMatrix = mat3(v0,v0,n);
	
	gl_Position = Projection * modelView * vec4(pos, 1.0f);
}
