in vec3 FragmentPos;
//in vec3 Normal;
in vec2 TexCoords;
// For normalmapping
in mat3 NormalMatrix;
in vec4 FragPosLightSpace;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec4 specularAndRoughness;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform sampler2D ShadowMap;

struct PointLight
{
	vec4 color;
	vec4 position;
	vec4 radiusAndPadding;
};

struct DirectionalLight 
{
	int lightType;
	
	vec4 color;
	vec4 direction;
};

uniform DirectionalLight sun;

struct SpotLight 
{
	//centerAndRadius:
	//Contains the center of the cone bounding sphere and the radius of it's sphere.
	vec4 centerAndRadius;
	
	//colorAndCenterOffset:
	//Color of the light and center offset
	//if angle is greater than 50 (tan(50) = 1) the distance from the cornerpoints to the center is greater than the distance to the spotlight position.
	//In this special case we need to apply a center offset for the position of the spotlight
    vec4 colorAndCenterOffset;
	
	//params:
	//Contains direction, angle and the falloff radius of the cone
	//XY is direction (we can reconstruct Z because we know the direction is normalized and we know the sign of Z dir)
	//Z is cosine of cone angle and lightdir Z sign (Sign bit is used to store sign for the z component of the light direction)
	//W is falloff radius
	vec4 params;
	
	//We can reconstruct position of spotlight by knowing that the top of the cone will be the 
	//bounding spheres radius away from it's center in the direction of the spotlight 
};

struct VisibleIndex 
{
	int index;
};

layout (std140, binding = 24) uniform uniformBlock
{
	int pointLightCount;
	int spotLightCount;
	int directionalLightCount;
	int tileLights;
};

// Shader storage buffer objects
layout(std430, binding = 1) readonly buffer PointLightBuffer 
{
	PointLight data[];
} pointLightBuffer;

layout(std430, binding = 2) readonly buffer SpotLightBuffer 
{
	SpotLight data[];
} spotLightBuffer;

layout(std430, binding = 3) readonly buffer DirectionalLightBuffer 
{
	DirectionalLight data[];
} directionalLightBuffer;

layout(std430, binding = 4) readonly buffer VisiblePointLightIndicesBuffer 
{
	VisibleIndex data[];
} visiblePointLightIndicesBuffer;

layout(std430, binding = 5) readonly buffer VisibleSpotLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleSpotLightIndicesBuffer;

layout(std430, binding = 6) readonly buffer VisibleDirectionalLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleDirectionalLightIndicesBuffer;

// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.005f, 0.005f, 0.005f);

// Attenuate the point light intensity
float attenuate(vec3 lightDirection, float radius)
{
	vec3 l = lightDirection / radius;
    float atten = max(0.0, 1.0 - dot(l,l));

	return atten;
}

// Assume the monitor is calibrated to the sRGB color space
const float screenGamma = 2.2;


void main()
{
	//uint tileLights = 512;
	
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	//Sample textures
	vec3 albedoDiffuseColor = texture(AlbedoMap,TexCoords).rgb;
	vec3 normal = texture(NormalMap, TexCoords).rgb;
	vec3 spec = texture(SpecularMap, TexCoords).rgb;
	float roughness = texture(RoughnessMap, TexCoords).r;

	float shininess = (roughness * 128) + 1;

	//Linearize colors
	vec3 N = NormalMatrix * ((normal*2.0f) - 1.0f);

	vec3 V = normalize(CameraPosition.xyz - FragmentPos.xyz);

	/// Loop for Point Lights
	uint offset = index * tileLights;
	for (uint i = 0; i < tileLights && visiblePointLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visiblePointLightIndicesBuffer.data[offset + i].index;
		PointLight light = pointLightBuffer.data[lightIndex];
		
		vec3 L = light.position.xyz - FragmentPos.xyz;

		float attenuation = attenuate(L, light.radiusAndPadding.x);

		L = normalize(L);
		float diffuse = max(dot(L, N), 0.0);
		float specular = 0.0f;

		//Hope this looks better with shadows...
		//if(diffuse > 0.0f)
		//{
			vec3 H = normalize(L + V);
			specular = pow(max(dot(H, N), 0.0), shininess);
		//}
		
		vec3 irradiance = (light.color.rgb * (albedoDiffuseColor.rgb * diffuse) + (vec3(specular) * spec)) * attenuation;
		
		color.rgb += irradiance;
	}

	/// Loop for SpotLights
	for (uint i = 0; i < tileLights && visibleSpotLightIndicesBuffer.data[offset + i].index != -1; i++)
	{		
		uint lightIndex = visibleSpotLightIndicesBuffer.data[offset + i].index;
		SpotLight light = spotLightBuffer.data[lightIndex];
		
		vec3 spotDir;
		spotDir.xy = light.params.xy;
		
		//Reconstruct z component
		//Because we know that our direction is normalized, we can easily calculate Z. however, we still don't know the sign of it.
		spotDir.z = sqrt(1 - spotDir.x*spotDir.x - spotDir.y*spotDir.y);
		//sign bit for cone is used for storing the sign of direction Z
		spotDir.z = (light.params.z > 0) ? spotDir.z : -spotDir.z;
		//Spotlight direction is now reconstructed
		
		//We can reconstruct position of spotlight by knowing that the top of the cone will be the 
		//bounding spheres radius away from it's center in the direction of the spotlight 
		vec3 spotPos = light.centerAndRadius.xyz - ((light.colorAndCenterOffset.a) * spotDir);

		vec3 projcords = FragPosLightSpace.xyz / FragPosLightSpace.w;
		
		vec3 uvcords = 0.5f * projcords + 0.5f;
		
		float depth = texture(ShadowMap, uvcords.xy).r;
		
		float z = uvcords.z;
		
		float shadowfactor = 0.0f;
		float nordir = clamp(dot(normal, spotDir), 0,1);
		float bias = 0.0005f * tan(acos(nordir));
		bias = min(max(bias, 0.005f), 0.0005f);
		
		
		//if (projcords.z > 1.0 && projcords.z < 0.0) //outside of shadowmap and therefore it should not be shaded
			//shadowfactor = 0.0f;
		if(projcords.z < 1.0 && projcords.z > 0.0)
		{
			//PCF, sample pixles for smooth shadows. DO NOT INCREASE THE SAMPLE RATE: IT WILL HAVE MASSIVE PERFORMANCE IMPACT
			vec2 texelsize = 1.0 / textureSize(ShadowMap, 0);
			for (int x=-3; x<=3; ++x)
			{
				for(int y = -3; y <= 3; ++y)
				{
					float pcfDepth = texture(ShadowMap, uvcords.xy + vec2(x, y) * texelsize).r; 
					shadowfactor += z-bias > pcfDepth ? 1.0f : 0.0f;      
				}
			}
			shadowfactor /= 49.0f;
		}
		
		/// Light Direction
		vec3 L = spotPos - FragmentPos.xyz;
		float distance = length(L);
		//As we already have distance, we can normalize by dividing by it.
		//normalize
		L = L / distance;
		
		//float attenuation = DoAttenuation(light, distance);
		//float spotIntensity = DoSpotCone(light, L);
		float cosineOfCurrentConeAngle = dot(-L, spotDir);
		
		//Falloff radius gives us more freedom to fiddle with attenuation.
		float falloffRadius = light.params.w;
		
		//params.z is angle
		float cosineOfConeAngle = (light.params.z > 0) ? light.params.z : -light.params.z;
		
		// If we're outside of distance or not within the cone, we can just discard doing any more work
		if (distance < falloffRadius && cosineOfCurrentConeAngle > cosineOfConeAngle)
        {
			float radialAttenuation = (cosineOfCurrentConeAngle - cosineOfConeAngle) / (1.0 - cosineOfConeAngle);
			//square the attenuation
			radialAttenuation = radialAttenuation * radialAttenuation;
			
			float x = distance / falloffRadius;
			// fast inverse squared falloff for a bit more accurate falloff. This is only approximative though
			//This is borrowed by AMD and their DX11 example of spotlights
            // -(1/k)* (1-(k+1) / (1+k*x^2))
            // k=20: -(1/20)*(1 - 21/(1+20*x^2))
			float falloff = -0.05 + 1.05/(1+20*x*x);
			
			float diffuse = max(dot(L, N), 0.0);
			
			vec3 H = normalize(L + V);
			float specular = pow(max(dot(H, N), 0.0), shininess);
			
			vec3 irradiance = (light.colorAndCenterOffset.rgb * (albedoDiffuseColor.rgb * diffuse * (1.0f-shadowfactor)) + (vec3(specular) * spec) * (1.0f-shadowfactor)) * falloff * radialAttenuation;
			color.rgb += irradiance;
		} 	
	}
	/*
	for (uint i = 0; i < tileLights && visibleDirectionalLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visibleDirectionalLightIndicesBuffer.data[offset + i].index;
		DirectionalLight light = directionalLightBuffer.data[lightIndex];
		
		vec3 L = normalize(light.direction.xyz);

		float attenuation = 1.0f;
		
		float diffuse = max(dot(L, N), 0.0);
		float specular = 0.0f;

		//Hope this looks better with shadows...
		//if(diffuse > 0.0f)
		//{
			vec3 H = normalize(L + V);
			specular = pow(max(dot(H, N), 0.0), shininess);
		//}

		vec3 irradiance = (light.color.rgb * (albedoDiffuseColor.rgb * diffuse) + (vec3(specular) * spec)) * attenuation;
		
		color.rgb += irradiance;
	}
	*/
	color.rgb += albedoDiffuseColor.rgb * u_lightAmbientIntensity;

	fragColor = color;
	normalColor = N;
	specularAndRoughness.rgb = spec;
	specularAndRoughness.a = roughness;

}