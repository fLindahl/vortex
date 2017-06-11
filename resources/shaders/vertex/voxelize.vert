layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 binormal;

out VS_OUT {
    vec2 TexCoords;
} vs_out; 

void main()
{
	vec4 wPos = (vec4(pos, 1.0f));
	gl_Position = wPos;
	
	vs_out.TexCoords.x = uv.x;
	vs_out.TexCoords.y = 1 - uv.y;
}
