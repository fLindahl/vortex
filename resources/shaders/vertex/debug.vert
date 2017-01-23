layout (location = 0) in vec3 position;

out float diffuse;
void main() 
{
	vec4 wPos =  Model * vec4(position,1.0f);
	diffuse = max(dot(wPos, normalize(CameraPosition)), 0.2);
	
	gl_Position = ViewProjection * wPos;
}
