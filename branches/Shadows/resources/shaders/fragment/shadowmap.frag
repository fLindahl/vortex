/// Shadow Map ///

in vec2 TexCoordsO;
in vec4 lightSpacePos;
in vec3 worldPos; //fragmentpos
in vec3 NormalO;

out vec4 FragColor;

//constant for the entire mesh
uniform sampler2D ShadowMap;
uniform sampler2D AlbedoMap;

struct SpotLight 
{
    vec4 color;
	vec4 position;
	vec4 coneDirection;
	vec4 midPoint;
	float length;
	float radius;
	float fRadius;
	float angle;
};

layout (std140, binding = 8) uniform spotLightBuffer
{
	SpotLight spotlight;
}spotLight;

float CalcShadowFactor(vec4 LightSpacePos)
{
	vec3 projcords = lightSpacePos.xyz / lightSpacePos.w;
	vec2 uvcords;
	
	uvcords.x = 0.5 * projcords.x + 0.5;                                                  
    uvcords.y = 0.5 * projcords.y + 0.5;                                                  
    float z = 0.5 * projcords.z + 0.5; 
	
	float depth = texture(ShadowMap, TexCoordsO).x;
	
	if (depth < z + 0.00001)
    {
		return 0.5;
    }
    else
    {
		return 1.0;
    }
}

vec4 CalcLight(vec4 color, vec4 position, vec4 direction, vec3 Normal, vec4 LightSpacePos, float shadowFactor)
{
	vec4 ambientcolor = vec4(color.xyz * vec3(0.1,0.1,0.1), 1.0f);
	float diffuseFactor = dot(Normal, -direction.xyz);
	
	vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 SpecularColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0)
	{
		DiffuseColor = vec4(color.xyz * diffuseFactor, 1.0);
		vec3 vertToEye = normalize(CameraPosition.xyz - worldPos);
		vec3 reflects = normalize(reflect(direction.xyz, Normal));
		float specFactor = dot(vertToEye, reflects);
        
		if(specFactor > 0)
		{
			specFactor = pow(specFactor, 2.0);
			SpecularColor = vec4(color.xyz, 1.0) * 2.0 * specFactor;
		}
	}
	
	return (ambientcolor + shadowFactor * (DiffuseColor + SpecularColor));
} 
	
vec4 CalcSpotLight(SpotLight l, vec3 Normal, vec4 LightSpacePos)                     
{                                                                                           
    vec3 LightToPixel = normalize(worldPos - l.position.xyz);                             
    float SpotFactor = dot(LightToPixel, l.coneDirection.xyz);
	float shadowFactor = CalcShadowFactor(LightSpacePos);
                                                                                            
    if (SpotFactor > l.length) 
    {                                                            
        vec4 Color = CalcLight(l.color, l.position,l.coneDirection, Normal, LightSpacePos, shadowFactor);                         
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.length));                   
    }                                                                                       
    else 
    {                                                                                  
        return vec4(0,0,0,0);                                                               
    }                                                                                       
}

	
	
void main() 
{
	///MAKE THE SCENE BLACK AND WHITE INSTED OF WHITE AND SPOTS OF BLACK
	//gl_FragDepth = ((0.5 * Projection[3][2]) / (gl_FragCoord.z + 0.5 * Projection[2][2] - 0.5) / 99.95f);
	
	
	//ogldev-tutorial-24
	vec3 Normal = normalize(NormalO);
	vec4 colorRGB = CalcSpotLight(spotLight.spotlight, Normal, lightSpacePos);
	
	vec4 scolor = texture(AlbedoMap, TexCoordsO.xy);
	FragColor = scolor * colorRGB;	
}