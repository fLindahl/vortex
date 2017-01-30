out float VSDepth;

layout (location = 0) in vec3 position;

void main() 
{	

	VSDepth = (View * Model * vec4(position, 1.0f)).z;
	gl_Position = ViewProjection * Model * vec4(position, 1.0);
}
