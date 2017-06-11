layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
} gs_in[];  

out vec2 TexCoords;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

const vec3 xAxis = vec3(1,0,0);
const vec3 yAxis = vec3(0,1,0);
const vec3 zAxis = vec3(0,0,1);

void main() 
{    
	vec3 normal = GetNormal();

	float L1 = abs(dot(normal, xAxis));
	float L2 = abs(dot(normal, yAxis));
	float L3 = abs(dot(normal, zAxis));
	
	//Orthographical projection along the dominant axis
	mat4 projection;
	
	//This looks redundant but actually only uses 2 comparisons for each case instead of a 2-3.
	if(L1 > L2)
	{
		if(L1 > L3)
		{
			//X is the dominant axis
			projection = mat4(vec4(0,0,0,0),vec4(yAxis,0),vec4(zAxis,0), vec4(0,0,0,0));
		}
		else
		{
			//Z is dominant axis
			projection = mat4(vec4(xAxis,0),vec4(yAxis,0),vec4(0,0,0,0), vec4(0,0,0,0));
		}
		
	}
	//Y is the dominant axis
	else if(L2 > L3)
	{
		projection = mat4(vec4(xAxis,0),vec4(0,0,0,0),vec4(zAxis,0), vec4(0,0,0,0));
	}
	//Z is the dominant axis
	else
	{
		projection = mat4(vec4(xAxis,0),vec4(yAxis,0),vec4(0,0,0,0), vec4(0,0,0,0));
	}
	
	
	
	gl_Position = projection * gl_in[0].gl_Position;
	TexCoords = gs_in[0].TexCoords;
    EmitVertex();
	
	gl_Position = projection * gl_in[1].gl_Position;
	TexCoords = gs_in[1].TexCoords;
    EmitVertex();
	
	gl_Position = projection * gl_in[2].gl_Position;
	TexCoords = gs_in[2].TexCoords;
    EmitVertex();
	
    EndPrimitive();

}  