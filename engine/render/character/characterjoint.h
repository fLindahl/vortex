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
  
  void Setup(const int& parentJointIndex, CharacterJoint* parentJoint, const Math::Vector4& poseTranslation, const std::string& name, const Math::Quaternion& poseRotation, const Math::Vector4& poseScale);

  void EvaluateMatrix(const Math::Vector4& scale, const Math::Quaternion& rotation, const Math::Vector4& translation);
  void ResetMatrix();
  
  CharacterJoint* getParentJoint();
  
  Math::Vector4& getPoseTranslation();
  Math::Quaternion& getPoseRotation();
  
  void EvaluateInverseMatrix();
  
  Math::Matrix4 GetInversePoseMatrix();
  Math::Matrix4 GetPoseMatrix();
  
  
  Math::Matrix4 GetEvaluatedMatrix();
  
private:
  std::string name;
    
  Math::Vector4 poseTranslation;
  Math::Quaternion poseRotation;
  Math::Vector4 poseScale;
  
  Math::Matrix4 currentMatrix;
  
  Math::Matrix4 poseMatrix;
  Math::Matrix4 invPoseMatrix;
  
  int parentJointIndex;
  CharacterJoint* parentJoint;    
};

}