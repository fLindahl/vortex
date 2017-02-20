/*
	Compute pass that generates a reflection buffer that we can multiply with our specular buffer
*/

#pragma once
#include "framepass.h"

namespace Render
{
class ReflectionPass : public FramePass
{
public:
	enum ReflectionQuality
	{
		HIGH,
		MEDIUM,
		LOW
	};

	struct SSRSettings
	{
		GLfloat zThickness;		// How thick is each depth fragment? higher value yields some wierd smudging at edges of reflection, but thinner z means we might miss some geometry. This should essentially be the average thickness of the geometry. to do dynamically would be a nightmare however...
		GLfloat stride;			// lets you skip pixels during iteration. Larger stride means longer rays with same number of samples.
		GLfloat jitter;			// if stride > 1 we ge banding results. This can be traded for noise by jittering the start position of each ray.
		GLfloat maxSteps;		// Less steps means shorter reflections, but better performance
		GLfloat maxDistance;	// Not orthogonal to max steps. reflection of surfaces far away in world space are prone to rapid shifts with only a slight change in camera positioning, which can lead to objectionable temporal flicker. Setting this parameter can help mitigate that.
		GLuint workGroups[2];	
		GLfloat padding; //TODO: padding might not be needed here
	};
	
	ReflectionPass();
	~ReflectionPass();

    void Setup();

    void Execute();

	void UpdateResolution();

	GLuint GetReflectionBuffer() { return this->reflectionBuffer; }

	SSRSettings& GetSSRSettings() { return this->uniformBlock; }

	ReflectionQuality& GetReflectionQuality() { return this->quality; }

private:
	GLuint numCubemaps;
	struct CubemapData
	{		
		Math::mat4 geometryproxy;
		Math::point cubemapposition;
		GLfloat blendfactor;
	};

	Util::Array<CubemapData> cubemapData;

	ReflectionQuality quality;

	SSRSettings uniformBlock;
	
	//Uniform Buffer Object
	GLuint ubo[1];

	GLuint cubemapUBO[1];

	GLuint SSRComputeProgram;
	GLuint CubemapProgram;
	GLuint PCCubemapProgram;
	GLuint reflectionBuffer;
};

}