#pragma once
#include "characterjoint.h"
#include "tinyxml2.h"
#include <vector>
#include <map>
#include "render/properties/graphicsproperty.h"

namespace Render
{

class AnimationResource;

class CharacterSkeleton
{
public:
  CharacterSkeleton();
  ~CharacterSkeleton();
    
  void Load(const char* fileName);
  
  std::vector<Math::mat4> EvaluateSkeleton(const int& animIndex);
  
  void SetAnimationResource(AnimationResource* a);
  
private:
  AnimationResource* animator;
  
  std::vector<CharacterJoint*> jointArray;
  int* skin;
  
  index_t animationFrame;
  double time;
  double frameTime;
  
  void Setup(const int& jointArraySize);
  void SetupJoint(const int& jointIndex, const Math::vec4& position, const std::string& jointName /*, const Quaternion& rotation, const vec4& scale*/);
    
};
}