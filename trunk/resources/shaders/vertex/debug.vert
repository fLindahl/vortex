layout (location = 0) in vec3 position;
out float depth;
void main() 
{
	vec4 wPos = Model * vec4(position,1.0f);
	depth = ((wPos.z / wPos.w) + 1.0) * 0.5;
	gl_Position = ViewProjection * wPos;
}
