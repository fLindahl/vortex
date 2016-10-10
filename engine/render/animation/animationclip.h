#pragma once
#include "foundation/util/array.h"
#include "foundation/math/quaternion.h"
#include "animcurve.h"
#include <string>

namespace Render
{

class AnimationClip
{	
public:
  AnimationClip();
  ~AnimationClip();
  
  /// set number of curves in clip
  void SetNumCurves(const size_t& numCurves);
  /// get number of curves in clip
  size_t GetNumCurves() const;
  /// access to curve by index
  AnimCurve& CurveByIndex(const int& curveIndex);
  
  /// set start key index (actual start time of the curve)
  void SetStartKeyIndex(const int& keyIndex);
  /// get start key index
  int GetStartKeyIndex() const;
  
  /// set the number of keys per animation curve in the clip
  void SetNumKeys(const size_t& numKeys);
  /// get the number of keys per animation curve in the clip
  size_t GetNumKeys() const;
  
  /// set the key stride (number of float's between keys of the same curve)
  void SetKeyStride(const size_t& stride);
  /// get the key stride
  size_t GetKeyStride() const;
  
  /// set the duration of a key in ticks
  void SetKeyDuration(const float& d);
  /// get the duration of a key
  float GetKeyDuration() const;
  
  /// set the name of the clip
  void SetName(const std::string& n);
  /// get the name of the clip
  const std::string& GetName() const;
  
  
  
private:
  std::string name;
  int startKeyIndex;
  size_t numKeys;
  size_t keyStride;
  
  float keyDuration;
  
  Util::Array<AnimCurve> curves;

  int keySliceFirstKeyIndex;
  size_t keySliceByteSize;
  bool keySliceValuesValid;
  
  
};

inline AnimationClip::AnimationClip()
{

}

inline AnimationClip::~AnimationClip()
{

}

inline void AnimationClip::SetKeyStride(const size_t& stride)
{
  this->keyStride = stride;
}

inline size_t AnimationClip::GetKeyStride() const
{
  return this->keyStride;
}

inline void AnimationClip::SetKeyDuration(const float& d)
{
  this->keyDuration = d;
}

inline float AnimationClip::GetKeyDuration() const
{
  return this->keyDuration;
}

inline size_t AnimationClip::GetNumKeys() const
{
  return this->numKeys;
}

inline void AnimationClip::SetName(const string& n)
{
  this->name = n;
}

inline const string& AnimationClip::GetName() const
{
  return this->name;
}

inline void AnimationClip::SetNumKeys(const size_t& numKeys)
{
  this->numKeys = numKeys;
}

inline void AnimationClip::SetStartKeyIndex(const int& keyIndex)
{
  this->startKeyIndex = keyIndex;
}

inline int AnimationClip::GetStartKeyIndex() const
{
  return this->startKeyIndex;
}

inline void AnimationClip::SetNumCurves(const size_t& numCurves)
{
  this->curves.Reserve(numCurves);
}

inline size_t AnimationClip::GetNumCurves() const
{
  return this->curves.Size();
}

inline AnimCurve& AnimationClip::CurveByIndex(const int& curveIndex)
{
  return this->curves[curveIndex];
}

}