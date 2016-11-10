layout(location=0) in vec2 Texcoords;
in vec3 Normal;
in vec3 toLight;
in vec3 toCamera;

out vec4 Texture;
uniform sampler2D AlbedoMap;

//////////////////////////////////////////////////////////

// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.1f, 0.1f, 0.1f);
const vec3 u_lightDiffuseIntensity = vec3(0.8f, 0.8f, 0.8f);
const vec3 u_lightSpecularIntensity = vec3(1.0f, 1.0f, 1.0f);

// parameters of the material and possible values
uniform vec4 AmbientReflectance; 
uniform vec4 DiffuseReflectance;
uniform vec4 SpecularReflectance;
uniform float Shininess;


/////////////////////////////////////////////////////////

// returns intensity of reflected ambient lighting
vec3 ambientLighting()
{
   return AmbientReflectance.xyz * u_lightAmbientIntensity;
}

// returns intensity of diffuse reflection
vec3 diffuseLighting(in vec3 N, in vec3 L)
{
   // calculation as for Lambertian reflection
   float diffuseTerm = clamp(dot(N, L), 0, 1) ;
   return DiffuseReflectance.xyz * u_lightDiffuseIntensity * diffuseTerm;
}

// returns intensity of specular reflection
vec3 specularLighting(in vec3 N, in vec3 L, in vec3 V)
{
   float specularTerm = 0;

   // calculate specular reflection only if
   // the surface is oriented to the light source
   if(dot(N, L) > 0)
   {
      // half vector
      vec3 H = normalize(L + V);
      specularTerm = pow(max(dot(N, H), 0.0), Shininess);
   }
   return SpecularReflectance.xyz * u_lightSpecularIntensity * specularTerm;
}

void main()
{
	// normalize vectors after interpolation
	vec3 L = normalize(toLight);
	vec3 V = normalize(toCamera);
	vec3 N = normalize(Normal);

	// get Blinn-Phong reflectance components
	vec3 Iamb = ambientLighting();
	vec3 Idif = diffuseLighting(N, L);
	vec3 Ispe = specularLighting(N, L, V);

	// diffuse color of the object from texture
	vec3 diffuseColor = texture(AlbedoMap,Texcoords).rgb;

	// combination of all components and diffuse color of the object
	Texture.xyz = diffuseColor * (Iamb + Idif + Ispe);
	Texture.a = 1;

}