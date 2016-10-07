#pragma once
#include "render/character/characterskeleton.h"

namespace Render
{

class AnimationParser
{
public:
  AnimationParser();
  ~AnimationParser();
  
  static bool LoadFile(const char* fileName, CharacterSkeleton& character);
  
  
private:
  
};

}