in vec3 FragmentPos;
//in vec3 Normal;
in vec2 TexCoords;
// For normalmapping
in mat3 NormalMatrix;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec4 specularAndRoughness;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;

//const float metallic = 0.0f;
#define PI 3.14159265358979323846

struct PointLight
{
	vec4 color;
	vec4 position;
	vec4 radiusAndPadding;
};

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

// Shader storage buffer objects
layout(std430, binding = 1) readonly buffer PointLightBuffer 
{
	PointLight data[];
} pointLightBuffer;

layout(std430, binding = 11) readonly buffer SpotLightBuffer 
{
	SpotLight data[];
} spotLightBuffer;

layout(std430, binding = 3) readonly buffer VisiblePointLightIndicesBuffer 
{
	VisibleIndex data[];
} visiblePointLightIndicesBuffer;

layout(std430, binding = 12) readonly buffer VisibleSpotLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleSpotLightIndicesBuffer;

// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.01f, 0.01f, 0.01f);

// Attenuate the point light intensity
float attenuate(vec3 lightDirection, float radius)
{
	vec3 l = lightDirection / radius;
    float atten = max(0.0, 1.0 - dot(l,l));

	return atten;
}

//Inverse gamma
#define GAMMA 2.2f
const float screenGamma = 1.0f / GAMMA;


//returns the ratio between specular and diffuse reflection or how much the surface reflects light versus how much it refracts light
//creates a rim-lighting effect, as the closer we get to zero incidence, the more the light the material will reflect.
vec3 fresnelSchlick(in float cosTheta, in vec3 F0, float roughness)
{
	//Spherical Gaussian approximation
	//return F0 + (1.0f - F0) * pow(2,((-5.55473f*cosTheta - 6.98316f) * cosTheta));
	
	//With roughness
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(2,((-5.55473f*cosTheta - 6.98316f) * cosTheta));
	
	//Old, yields artifacts every now and then
	//return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

//We use GGX for our normal distribution function (NDF)
//This approximates the ratio of microfacets aligned to given halfway vector H
float NormalDistributionGGX(in vec3 N, in vec3 H, in float roughness)
{
	float a = pow(roughness, 4);
	
	float NdotH = max(dot(N, H), 0.0f);
	NdotH = NdotH * NdotH;
	
	float denom = (NdotH * (a - 1.0f) + 1.0f);
	denom = PI * denom * denom;
	
	return (a / (denom + 0.00001f));
}

//We use Smith with Schlick-GGX for our geometry function
//This statistically approximates the ratio of microfacets that overshadow each other
//TODO: These two functions can be optimized
float GeometrySchlickGGX(in float NdotV, in float roughness)
{
	//Optimization: Moved to GeometrySmith, so that we don't calculate it twice
	//float r = roughness + 1.0f;
	//float k = (r*r) * 0.125f; // (r^2 / 8)
	float k = roughness;
	
	float denom = NdotV * (1.0f - k) + k;
	return NdotV / denom;
}

float GeometrySmith(in float NdotV, in float NdotL, in float roughness)
{	
	//Optimization: Moved here from GeometrySchlickGGX so that we don't calculate it twice
	float r = roughness + 1.0f;
	float k = (r*r) * 0.125f; // (r^2 / 8)
	
	float ggxNV = GeometrySchlickGGX(NdotV, k);
	float ggxNL = GeometrySchlickGGX(NdotL, k);
	
	return ggxNV * ggxNL;
}

void main()
{
	uint tileLights = 512;
	
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	vec3 color = vec3(0.0, 0.0, 0.0);

	float reflectance = 0;
	
	//Sample textures
	vec4 albedo = texture(AlbedoMap,TexCoords).rgba;
	
	//Adjust to linear space
	albedo.rgb = pow(albedo.rgb, vec3(GAMMA));
		
	vec3 normal = texture(NormalMap, TexCoords).rgb;
	//vec3 spec = texture(SpecularMap, TexCoords).rgb;
	float metallic = 0.2;//texture(SpecularMap, TexCoords).r;
	float roughness = texture(RoughnessMap, TexCoords).r;

	vec3 N = normalize(NormalMatrix * ((normal*2.0f) - 1.0f));
	vec3 V = normalize(CameraPosition.xyz - FragmentPos.xyz);
	
	//F0 as 0.04 will usually look good for all dielectric (non-metal) surfaces
	vec3 F0 = vec3(0.04);
	//for metallic surfaces we interpolate between F0 and the albedo value with metallic value as our lerp weight
	F0 = mix(F0, albedo.rgb, metallic);
	
	vec3 irradiance = vec3(0.0f, 0.0f, 0.0f);
	
	/// Loop for Point Lights
	uint offset = index * tileLights;
	for (uint i = 0; i < tileLights && visiblePointLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visiblePointLightIndicesBuffer.data[offset + i].index;
		PointLight light = pointLightBuffer.data[lightIndex];
		
		vec3 L = light.position.xyz - FragmentPos.xyz;

		float lightDistance = length(L);
		float lightRadius = light.radiusAndPadding.x;
		
		if( lightDistance < lightRadius )
		{
			float x = lightDistance / lightRadius;
            // fast inverse squared falloff for a bit more accurate falloff. This is only approximative though
			//This is borrowed from AMD and their DX11 example of spotlights
            // -(1/k)* (1-(k+1) / (1+k*x^2))
            // k=20: -(1/20)*(1 - 21/(1+20*x^2))
            float attenuation = -0.05 + 1.05/(1+20*x*x);
			
			vec3 radiance = light.color.rgb * attenuation;
			
			L = normalize(L);
			vec3 H = normalize(V + L);
			
			//if we are using IBL we should use dot(N,V) for cosTheta. 
			//The correct way of doing it for direct lighting is using the halfway H for each lightsource
			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0, roughness);

			//F denotes the specular contribution of any light that hits the surface
			//We set kS (specular) to F and because PBR requires the condition that our equation is
			//energy conserving, we can set kD (diffuse contribution) to 1 - kS directly
			//as kS represents the energy of light that gets reflected, the remeaining energy gets refracted, resulting in our diffure term
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;

			//Fully metallic surfaces won't refract any light
			kD *= 1.0f - metallic;			
						
			float NdotL = max(dot(L, N), 0.0f);
			float NdotV = max(dot(V, N), 0.0f);
			
			float NDF = NormalDistributionGGX(N, H, roughness);
			float G = GeometrySmith(NdotV, NdotL, roughness);
			
			//Calculate Cook-Torrance BRDF
			vec3 nominator = NDF * G * F;
			float denominator = 4 * NdotV * NdotL + 0.001f; //We add 0.001f in case dot ends up becoming zero.
			vec3 brdf = nominator / denominator;
			
			irradiance += (kD * albedo.rgb / PI + brdf) * radiance * NdotL;
		}
		
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
		
		/// Light Direction
		vec3 L = spotPos - FragmentPos.xyz;
		float distance = length(L);
		//As we already have distance, we can normalize by dividing by it.
		//normalize
		L = L / distance;
		
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
			//This is borrowed from AMD and their DX11 example of spotlights
            // -(1/k)* (1-(k+1) / (1+k*x^2))
            // k=20: -(1/20)*(1 - 21/(1+20*x^2))
			float falloff = -0.05 + 1.05/(1+20*x*x);
			
			vec3 H = normalize(V + L);
			
			vec3 radiance = light.colorAndCenterOffset.rgb * radialAttenuation * falloff;
			
			//if we are using IBL we should use dot(N,V) for cosTheta. 
			//The correct way of doing it for direct lighting is using the halfway H for each lightsource
			vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0, roughness);

			//F denotes the specular contribution of any light that hits the surface
			//We set kS (specular) to F and because PBR requires the condition that our equation is
			//energy conserving, we can set kD (diffuse contribution) to 1 - kS directly
			//as kS represents the energy of light that gets reflected, the remeaining energy gets refracted, resulting in our diffure term
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;
			
			//Fully metallic surfaces won't refract any light
			kD *= 1.0f - metallic;			
						
			float NdotL = max(dot(L, N), 0.0f);
			float NdotV = max(dot(V, N), 0.0f);
			
			float NDF = NormalDistributionGGX(N, H, roughness);
			float G = GeometrySmith(NdotV, NdotL, roughness);
			
			//Calculate Cook-Torrance BRDF
			vec3 nominator = NDF * G * F;
			float denominator = 4 * NdotV * NdotL + 0.001f; //We add 0.001f in case dot ends up becoming zero.
			vec3 brdf = nominator / denominator;
			
			irradiance += (kD * albedo.rgb / PI + brdf) * radiance * NdotL;
		}
	}
	
	color.rgb = irradiance + (albedo.rgb * u_lightAmbientIntensity);

	color.rgb = color / (color + vec3(1.0f));
	color = pow(color, vec3(screenGamma));
	
	fragColor.rgb = color.rgb;
	fragColor.a = albedo.a;
	normalColor = N;
	//Specular term based on fresnel. This is for IBL and works for our reflection pass
	specularAndRoughness.rgb = fresnelSchlick(max(dot(N, V), 0.0), F0, roughness);
	specularAndRoughness.a = roughness;

}