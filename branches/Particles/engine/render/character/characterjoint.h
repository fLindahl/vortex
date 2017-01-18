#pragma once
#include <vector>
#include "foundation/math/vector4.h"
#include "foundation/math/quaternion.h"
#include "foundation/math/matrix4.h"

namespace Render
{

class CharacterJoint
{
public:
  CharacterJoint();
  ~CharacterJoint();
  
  void Setup(const int& parentJointIndex, CharacterJoint* parentJoint, const Math::vec4& poseTranslation, const std::string& name, const Math::quaternion& poseRotation, const Math::vec4& poseScale);

  void EvaluateMatrix(const Math::vec4& scale, const Math::quaternion& rotation, const Math::vec4& translation);
  void ResetMatrix();
  
  CharacterJoint* getParentJoint();
  
  Math::vec4& getPoseTranslation();
  Math::quaternion& getPoseRotation();
  
  void EvaluateInverseMatrix();
  
  Math::mat4 GetInversePoseMatrix();
  Math::mat4 GetPoseMatrix();
  
  
  Math::mat4 GetEvaluatedMatrix();
  
private:
  std::string name;
    
  Math::vec4 poseTranslation;
  Math::quaternion poseRotation;
  Math::vec4 poseScale;
  
  Math::mat4 currentMatrix;
  
  Math::mat4 poseMatrix;
  Math::mat4 invPoseMatrix;
  
  int parentJointIndex;
  CharacterJoint* parentJoint;    
};

}