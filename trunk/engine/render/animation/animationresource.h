#pragma once 
#include "animationparser.h"
#include "foundation/util/array.h"
#include "animkeybuffer.h"

namespace Render
{

class AnimationClip;

class AnimationResource
{
public:
  AnimationResource();
  ~AnimationResource();
  
  size_t GetNumClips();
  const AnimationClip& GetClipByIndex(size_t clipIndex) const;
  
  const AnimKeyBuffer* GetKeyBuffer() const;

  void SetAnimClipsSize(const size_t& size);
  
  bool LoadAnimations(const char* fileName);
  
private:
  
  const AnimKeyBuffer* SetupKeyBuffer(size_t numKeys);
  
  Util::Array<AnimationClip> animationClips;
  
  AnimKeyBuffer* animKeyBuffer;
  
};

}