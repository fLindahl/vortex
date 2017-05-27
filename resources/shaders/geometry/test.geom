layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 FragmentPos;
	vec2 TexCoords;
	mat3 NormalMatrix;
} gs_in[];  

out vec3 FragmentPos;
out vec2 TexCoords;
out mat3 NormalMatrix;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.2f;
    vec3 direction = normal * ((sin(TimeAndRandom.x) + 1.0f) / 2.0f) * magnitude; 
    return position + vec4(direction, 0.0f);
} 

void main() 
{    
	vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
	FragmentPos = gs_in[0].FragmentPos;
	TexCoords = gs_in[0].TexCoords;
	NormalMatrix = gs_in[0].NormalMatrix;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
	FragmentPos = gs_in[1].FragmentPos;
	TexCoords = gs_in[1].TexCoords;
	NormalMatrix = gs_in[1].NormalMatrix;
    EmitVertex();
    
	gl_Position = explode(gl_in[2].gl_Position, normal);
	FragmentPos = gs_in[2].FragmentPos;
	TexCoords = gs_in[2].TexCoords;
	NormalMatrix = gs_in[2].NormalMatrix;
    EmitVertex();
	
    EndPrimitive();
}  