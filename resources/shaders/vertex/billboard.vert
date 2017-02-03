layout(location=0) in vec3 pos;
layout(location=2) in vec2 uv;

out vec2 TexCoords;

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
	
	TexCoords.x = uv.x;
	TexCoords.y = 1 - uv.y;
	
	gl_Position = Projection * modelView * vec4(pos, 1.0f);
}
