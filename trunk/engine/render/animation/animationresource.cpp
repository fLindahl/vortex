#include "config.h"
#include "naxfileformatstructs.h"
#include "animationresource.h"
#include "animationclip.h"

namespace Render
{

AnimationResource::AnimationResource()
{
    
}

AnimationResource::~AnimationResource()
{

}


const AnimationClip& AnimationResource::GetClipByIndex(size_t clipIndex) const
{
  return this->animationClips[clipIndex];
}

const AnimKeyBuffer* AnimationResource::GetKeyBuffer() const
{
  return animKeyBuffer;
}

const AnimKeyBuffer* AnimationResource::SetupKeyBuffer(size_t numKeys)
{
  this->animKeyBuffer = new AnimKeyBuffer();
  this->animKeyBuffer->Setup(numKeys);
  return this->animKeyBuffer;
}

size_t AnimationResource::GetNumClips()
{
  return this->animationClips.Size();
}

void AnimationResource::SetAnimClipsSize(const size_t& size)
{
  this->animationClips.Reserve(size);
}


bool AnimationResource::LoadAnimations(const char* fileName)
{
    FILE * file;
    size_t bufferSize;
    uchar* buffer;
    size_t result;

	file = fopen(fileName , "rb" );

    // obtain file size:
    fseek (file , 0 , SEEK_END);
    bufferSize = ftell (file);
    rewind (file);

    // allocate memory to contain the whole file:
    buffer = new uchar[bufferSize];

    // copy the file into the buffer:
    result = fread(buffer,1,bufferSize,file);
    
    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (file);
    
    //LETS GO
    
    uchar* ptr = (uchar*) buffer;
	
    // read header
    Nax3Header* naxHeader = (Nax3Header*) ptr;
    ptr += sizeof(Nax3Header);

    

    // load animation if it has clips in it
    if (naxHeader->numClips > 0)
    {
	    // setup animation clips
	    size_t numClips = (size_t)naxHeader->numClips;
	    this->SetAnimClipsSize(numClips);
	    size_t clipIndex;
	    for (clipIndex = 0; clipIndex < numClips; clipIndex++)
	    {
		    Nax3Clip* naxClip = (Nax3Clip*)ptr;
		    ptr += sizeof(Nax3Clip);

		    // setup anim clip object
		    AnimationClip& clip = this->animationClips[clipIndex];
		    clip.SetNumCurves(naxClip->numCurves);
		    clip.SetStartKeyIndex(naxClip->startKeyIndex);
		    clip.SetNumKeys(naxClip->numKeys);
		    clip.SetKeyStride(naxClip->keyStride);
		    clip.SetKeyDuration(naxClip->keyDuration);
		    clip.SetName(naxClip->name);
	 
		    for (int eventIndex = 0; eventIndex < naxClip->numEvents; eventIndex++)
		    {
		      ptr += sizeof(Nax3AnimEvent);
		    }
		    
		    // setup anim curves
		    int curveIndex;
		    for (curveIndex = 0; curveIndex < naxClip->numCurves; curveIndex++)
		    {
			    Nax3Curve* naxCurve = (Nax3Curve*)ptr;
			    ptr += sizeof(Nax3Curve);

			    AnimCurve& animCurve = clip.CurveByIndex(curveIndex);
			    animCurve.SetFirstKeyIndex(naxCurve->firstKeyIndex);
			    animCurve.SetActive(naxCurve->isActive != 0);
			    animCurve.SetStatic(naxCurve->isStatic != 0);
			    animCurve.SetCurveType((CurveType::Code)naxCurve->curveType);
			    animCurve.SetStaticKey(Math::vec4(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW));
		    }
	    }

	    // load keys
	    const AnimKeyBuffer* animKeyBuffer = this->SetupKeyBuffer(naxHeader->numKeys);
		Math::vec4* keyPtr = animKeyBuffer->GetKeyBufferPointer();
	    std::memcpy(keyPtr, ptr, animKeyBuffer->GetByteSize());
	    
	    
    }
    
    //delete[] buffer;
    
    return true; 
}

}