layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 binormal;

out vec3 FragmentPos;
out vec2 TexCoords;
out mat3 NormalMatrix;
out vec3 ViewSpaceNormal;

void main()
{
	vec4 wPos = (Model * vec4(pos, 1.0f));
	
	// position in world space
	FragmentPos = wPos.xyz;	
		
	mat3 model33 = mat3(Model);
		
	vec3 t = normalize(model33 * (tangent));
	vec3 b = normalize(model33 * (binormal));
	vec3 n = normalize(model33 * (normal));
	
	NormalMatrix = mat3(t,b,n);
		
	TexCoords.x = uv.x;
	TexCoords.y = 1 - uv.y;

	ViewSpaceNormal = mat3(View * Model) * normal;
	
	gl_Position = ViewProjection * wPos;
}
