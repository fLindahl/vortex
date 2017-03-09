in vec3 FragmentPos;
//in vec3 Normal;
in vec2 TexCoords;
// For normalmapping
in mat3 NormalMatrix;
in vec4 lightSpaceCoordinates[10];

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec4 specularAndRoughness;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform sampler2D ShadowMap;
uniform sampler2D VSMShadowMap;

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

layout (std140, binding = 24) uniform uniformBlock
{
	int pointLightCount;
	int spotLightCount;
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

layout(std430, binding = 4) readonly buffer VisiblePointLightIndicesBuffer 
{
	VisibleIndex data[];
} visiblePointLightIndicesBuffer;

layout(std430, binding = 5) readonly buffer VisibleSpotLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleSpotLightIndicesBuffer;

/// Shadow Map
layout(std430, binding = 17) readonly buffer TextureOffsetCoordinates
{
	vec4 data[];
} textureOffsetCoordinates;

/*layout(std430, binding = 18) readonly buffer VisibleSpotLightIndicesBuffer
{
	VisibleIndex data[];
} visibleSpotLightIndicesBuffer;*/

/// Shadow Map

layout(std140, binding = 20) uniform directionalLight
{
	vec4 color;
	vec4 direction;
}dirLight;

// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.05f, 0.05f, 0.05f);

// Attenuate the point light intensity
float attenuate(vec3 lightDirection, float radius)
{
	vec3 l = lightDirection / radius;
    float atten = max(0.0, 1.0 - dot(l,l));

	return atten;
}

//////////////////////////////MSM//////////////////////////////////////////
vec4 UndoQuantTransform(vec4 oldmoments)
{
	vec4 moments;
	mat4 quant = mat4(-0.333333333333333f, 0.0f, 1.732050807568877f, 0.0f,
					   0.0f, 0.125f, 0.0f, 1.0f,
					   -0.75f, 0.0f, (0.75*1.732050807568877f), 0.0f,
					   0.0f, -0.125f, 0.0f, 1.0f);
	vec4 changecoordspace = vec4(0.5f, 0.0f,0.5f,0.0f);
	mat4 transposed = transpose(quant);
	moments = transposed*(oldmoments-changecoordspace);
	
	return moments;
}

vec4 GetOptimisedMoments(float depth)
{
	float square = depth * depth;
    vec4 moments = vec4(depth, square, square * depth, square * square);
	mat4 optimizedvalues = mat4(-2.07224649f,    13.7948857237f,  0.105877704f,   9.7924062118f,
                                 32.23703778f,  -59.4683975703f, -1.9077466311f, -33.7652110555f,
                                -68.571074599f,  82.0359750338f,  9.3496555107f,  47.9456096605f,
                                 39.3703274134f,-35.364903257f,  -6.6543490743f, -23.9728048165f);
	vec4 optimized = moments *  optimizedvalues;
    optimized[0] += 0.035955884801f;
	return optimized;
}

vec4 ConvertMoments(vec4 optmoments)
{
	optmoments[0] -= 0.035955884801f;
	mat4 temp = mat4(0.2227744146f, 0.1549679261f, 0.1451988946f, 0.163127443f,
                     0.0771972861f, 0.1394629426f, 0.2120202157f, 0.2591432266f,
                     0.7926986636f, 0.7963415838f, 0.7258694464f, 0.6539092497f,
					 0.0319417555f,-0.1722823173f,-0.2758014811f,-0.3376131734f);
	return optmoments * temp;
}

float ComputeMSMUnbound(vec4 moments, float fragdepth, float depthbias, float momentbias)
{
	//use bias to avoid artifacts
	vec4 b = mix(moments, vec4(0.5f,0.5f,0.5f,0.5f), momentbias);
	vec3 z;
	z.x = fragdepth - depthbias;
	//vec4 b = moments;
	//compute Cholesky factorization
	//mad = The result of mvalue * avalue + bvalue.
	float L32D22 = -b.x * b.y + b.z;
	float D22 = -b.x * b.x + b.y;
	float squaredzvariance = -b.y * b.y + b.w;
	float D33D22 = dot(vec2(squaredzvariance, -L32D22), vec2(D22, L32D22));
	float InvD22 = 1.0f / D22;
	float L32 = L32D22 * InvD22;
	
	//get scaled image of b
	vec3 c = vec3(1.0f, z.x, z.x * z.x);
	
	c.y -= b.x;
	c.z -= b.y + L32 * c.y;
	
	//scale to solve D*c2=c1
	c.y *= InvD22;
	c.z *= D22 / L32D22;
	
	//solve L^T*c3 = c2
	c.y -= L32 * c.z;
	c.x -= dot(c.yz, b.xy);
	
	//solve c.x+c.y*z+c.z^2
	float p = c.y / c.z;
	float q = c.x / c.z;
	float D = (p * p * 0.25f) - q;
	float r = sqrt(D);
	z.y = -p * 0.5f - r;
	z.z = -p * 0.5f + r;
	
	//compute the shadow intensity by summing
	vec4 switchvalue = (z.z < z.x) ? vec4(z.y, z.x, 1.0f, 1.0f) :
                       ((z.y < z.x) ? vec4(z.x, z.y, 0.0f, 1.0f) :
                       vec4(0.0f,0.0f,0.0f,0.0f));
	
	
	
	float quotient = (switchvalue.x * z.z - b.x * (switchvalue.x + z.y) + b.y) 
						/ ((z.z - switchvalue.y) * (z.x - z.y));
	float shadowintesity = switchvalue.z + switchvalue.w * quotient;
	
	return clamp(shadowintesity, 0, 1);
}
float ComputeMSM(vec4 moments, float fragdepth, float depthbias, float momentbias)
{
	//use bias to avoid artifacts
	vec4 b = mix(moments, vec4(0.5f,0.5f,0.5f,0.5f), momentbias);
	vec3 z;
	z.x = fragdepth - depthbias;
	
	//compute Cholesky factorization
	//mad = The result of mvalue * avalue + bvalue.
	float L32D22 = -b.x * b.y + b.z;
	float D22 = -b.x * b.x + b.y;
	float squaredzvariance = -b.y * b.y + b.w;
	float D33D22 = dot(vec2(squaredzvariance, -L32D22), vec2(D22, L32D22));
	float InvD22 = 1.0f / D22;
	float L32 = L32D22 * InvD22;
	
	//get scaled image of b
	vec3 c = vec3(1.0f, z.x, z.x * z.x);
	
	c.y -=b.x;
	c.z -= b.y + L32 * c.y;
	
	//scale to solve D*c2=c1
	c.y *= InvD22;
	c.z *= D22 / D33D22;
	
	//solve L^T*c3 = c2
	c.y -= L32 * c.z;
	c.x -= dot(c.yz, b.xy);
	
	//solve c.x+c.y*z+c.z^2
	float p = c.y / c.z;
	float q = c.x / c.z;
	float D = ((p * p) * 0.25f) - q;
	float r = sqrt(D);
	z.y =- p * 0.5f - r;
	z.z =- p * 0.5f + r;
	
	float shadowintesity = 1.0f;
	
	//use four deltas if 3 three delta solution is invalid
	if (z.y < 0.0f || z.z > 1.0f)
	{
		float FreeZ = ((b.z-b.y) * z.x + b.z -b.w) / ((b.y-b.x) * z.x +b.y-b.z);
		float w1Factor = (z.x > FreeZ) ? 1.0f : 0.0f; //maybe swicth 1 and 0 here
		
		shadowintesity = ( b.y - b.x + (b.z - b.x - (FreeZ + 1.0f) * (b.y - b.x)) * (FreeZ - w1Factor - z.x) 
						   / (z.x * (z.x - FreeZ))) / (FreeZ - w1Factor) + 1.0f - b.x;
		
	}
	else
	{
		vec4 switchvalue =  (z.z < z.x) ? vec4(z.y, z.x, 1.0f, 1.0f) :
							((z.y < z.x) ? vec4(z.x, z.y, 0.0f, 1.0f) :
							vec4(0.0f, 0.0f, 0.0f, 0.0f));
							
		float quotient = (switchvalue.x * z.z - b.x * (switchvalue.x + z.z) + b.y) / ((z.z - switchvalue.y) * (z.x - z.y));
		
		shadowintesity = switchvalue.z + switchvalue.w * quotient;
	}
	
	return clamp(shadowintesity, 0.0f, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Assume the monitor is calibrated to the sRGB color space
const float screenGamma = 2.2;
///VSM
//for lightbleeding reduction
float linstep(float low, float high, float v)
{
	return clamp((v-low)/(high-low), 0.0, 1.0);
}
float VSMexperiment(sampler2D smap, vec2 scoords, float compare, float bias)
{
	
	vec2 dmoments = texture(smap, scoords.xy).rg;
	
	float p = step(compare, dmoments.x);
	///0.00002 is a bias value to make sure variance aint 0
	float variance = max(dmoments.y - dmoments.x*dmoments.x, bias);
	float distance = compare - dmoments.x;
	//upper bound percentage
	float varianceeq = variance / (variance + distance*distance);
	//the first 1 here is the lightbleedreduction, keep at 1 or we have shadows on all walls
	float upperp = linstep(1.0, 1.0, varianceeq);
	
	return min(max(p, upperp), 1.0);
	
	
	//return step(compare, texture(smap, scoords.xy).r);
}

float VSMexperimentMM(sampler2D smap, vec2 scoords, float compare, float bias)
{
	vec2 dmoments = texture(smap, scoords).rg;
	vec2 texelsize = 1.0 / textureSize(smap, 0);

	//small filtering, makes it a tiny bit smoother

	vec2 c1 = scoords+vec2(0, texelsize.y);
	vec2 c2 = scoords+vec2(texelsize.x, 0);
	vec2 c3 = scoords+vec2(texelsize.x, texelsize.y);
	vec2 c4 = scoords;
	vec2 c5 = scoords+vec2(0, -texelsize.y);
	vec2 c6 = scoords+vec2(-texelsize.x, 0);
	vec2 c7 = scoords+vec2(-texelsize.x, -texelsize.y);

	vec2 d1 = texture(smap, c1).rg;
	vec2 d2 = texture(smap, c2).rg;
	vec2 d3 = texture(smap, c3).rg;
	vec2 d4 = texture(smap, c4).rg;
	vec2 d5 = texture(smap, c5).rg;
	vec2 d6 = texture(smap, c6).rg;
	vec2 d7 = texture(smap, c7).rg;

	vec2 SevenAAmoments = (d1+d2+d3+d4+d5+d6+d7)/7.0;
	vec2 FourAAmoments = (d1+d2+d3+d4) /4.0;
	
	//gaussian filtering
	vec2 color = vec2(0,0);
	//small gaussian filter 7x1 filter
	color += texture( smap, scoords + vec2( -3.0*texelsize.x, -3.0*texelsize.y ) ).rg * 0.015625;
	color += texture( smap, scoords + vec2( -2.0*texelsize.x, -2.0*texelsize.y ) ).rg * 0.09375;
	color += texture( smap, scoords + vec2( -1.0*texelsize.x, -1.0*texelsize.y ) ).rg * 0.234375;
	color += texture( smap, scoords + vec2( 0.0 , 0.0) ).rg*0.3125;
	color += texture( smap, scoords + vec2( 1.0*texelsize.x,  1.0*texelsize.y ) ).rg * 0.234375;
	color += texture( smap, scoords + vec2( 2.0*texelsize.x,  2.0*texelsize.y ) ).rg * 0.09375;
	color += texture( smap, scoords + vec2( 3.0*texelsize.x,  3.0*texelsize.y ) ).rg * 0.015625;

	vec2 GAdmoments = color;

	//dmoments = (FourAAmoments + GAdmoments)/ 2;
	//dmoments = (SevenAAmoments + GAdmoments)/ 2;

	//dmoments = SwarleyAA(smap, scoords, 1);
	dmoments = SevenAAmoments;

	float p = step(compare, dmoments.x);
	///0.00002 is a bias value to make sure variance aint 0
	float variance = max(dmoments.y - dmoments.x*dmoments.x, bias);
	float distance = compare - dmoments.x;
	//upper bound percentage
	float varianceeq = variance / (variance + distance*distance);
	//the first 1 here is the lightbleedreduction, keep at 1 or we have shadows on all walls
	float upperp = linstep(1.0, 1.0, varianceeq);

	return min(max(p, upperp), 1.0);

	//return step(compare, texture(smap, scoords.xy).r);
}

//PCF-------------------------------------------------------------------------

float PCFsampling(sampler2D shamap, vec2 uvcords, float fragdepth, float bias)
{
		
	//Percentage closer filtering, be careful with incresing the sample rate. It looks weird
	float shadowfactor = 0;
	vec2 texelsize = 1.0 / textureSize(shamap, 0);
	for (float x=-1; x <= 1; ++x)
	{
		for(float y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shamap, uvcords.xy + vec2(x, y) * texelsize).x;
			shadowfactor += fragdepth-bias > pcfDepth ? 0.0f : 1.0f; 					
		}
	}
	return shadowfactor /= 9.0f;
}

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

		vec3 projcords = lightSpaceCoordinates[lightIndex].xyz / lightSpaceCoordinates[lightIndex].w;
		//vec3 projcords = lightPositionCoordinates.data[lightIndex].xyz / lightPositionCoordinates.data[lightIndex].w;
		
		vec3 uvcords = 0.5f * projcords + 0.5f;

		/// Sets the scale of the Shadow Maps in the Atlas
		uvcords.x *= textureOffsetCoordinates.data[lightIndex].z;
		uvcords.y *= textureOffsetCoordinates.data[lightIndex].z;
		// A Shadow Maps Offset in the Atlas
		uvcords.x += textureOffsetCoordinates.data[lightIndex].x;
		uvcords.y += textureOffsetCoordinates.data[lightIndex].y;

		//float depth = texture(ShadowMap, uvcords.xy).r;

		float z = uvcords.z;

		float shadowfactor = 0.0f;

		//dont shade fragments thats outside of the shadowmap
		if (z < 1.0)
		{
			///PCF
			//bias calc for PCF
			float nordir = clamp(dot(normal, spotDir), 0,1);
			float bias = 0.005f * tan(acos(nordir));
			bias = min(max(bias, 0.005f), 0.0005f);
			shadowfactor = PCFsampling(ShadowMap, uvcords.xy, z, bias);

			///VSM without filtering, lots of light leaking compared to pcf, however the shadows look better(or rahter the edges look better)
			//shadowfactor = VSMexperiment(VSMShadowMap, uvcords.xy, z-bias, 0.00001);
			//shadowfactor = VSMexperimentMM(VSMShadowMap, uvcords.xy, z-bias, 0.000038);
		}

		///MSM with no filtering, Atleast I thinks its msm
		//vec4 moments = texture(MSMShadowMap, uvcords.xy).rgba;
		////vec4 usefulmoments = UndoQuantTransform(moments);
		//float momentbias = 0.08f;
		//uhm the CamputeMSM function only works on a lenght of 31 ish otherwise its retarded ??
		//shadowfactor = ComputeMSM(ConvertMoments(GetOptimisedMoments(depth)), z, bias, momentbias);
		//shadowfactor = 1.0f-ComputeMSMUnbound(usefulmoments, gl_FragCoord.z , bias, momentbias);

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
			
			vec3 irradiance = (light.colorAndCenterOffset.rgb * (albedoDiffuseColor.rgb * diffuse * (shadowfactor)) + (vec3(specular) * spec) * (shadowfactor)) * falloff * radialAttenuation;
			color.rgb += irradiance;
		} 	
	}
	
	if(dirLight.direction.w == 0.0)
	{
		vec3 L = normalize(dirLight.direction.xyz);

		float attenuation = 1.0f;
		
		float diffuse = max(dot(L, N), 0.0);
		float specular = 0.0f;

		//Hope this looks better with shadows...
		//if(diffuse > 0.0f)
		//{
			vec3 H = normalize(L + V);
			specular = pow(max(dot(H, N), 0.0), shininess);
		//}

		vec3 irradiance = (dirLight.color.rgb * (albedoDiffuseColor.rgb * diffuse) + (vec3(specular) * spec)) * attenuation;
		
		color.rgb += irradiance;
	}
	
	color.rgb += albedoDiffuseColor.rgb * u_lightAmbientIntensity;

	fragColor = color;
	normalColor = N;
	specularAndRoughness.rgb = spec;
	specularAndRoughness.a = roughness;

}