/// Shadow Map ///

in vec2 oUV;
in vec4 Shadowcoord;

layout(location = 0) out vec3 color;

uniform sampler2D AlbedoMap;
uniform sampler2DShadow shadowMap;

void main() 
{
	vec3 lightColor = vec3(0.0,1.0,0.0);
	
	vec3 matDiffuseColor = texture(AlbedoMap, oUV).rgb;

	float visiblity = texture(shadowMap, vec3(Shadowcoord.xy, (Shadowcoord.z)/Shadowcoord.w));
	
	color = visiblity * matDiffuseColor * lightColor;
	
	gl_FragDepth = ((0.5 * Projection[3][2]) / (gl_FragCoord.z + 0.5 * Projection[2][2] - 0.5) / 99.95f);
}