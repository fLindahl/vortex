layout (location = 0) in vec3 position;

void main() 
{	
	gl_Position = ViewProjection * Model * vec4(position, 1.0);
}
