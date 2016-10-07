#include "config.h"
#include "characterjoint.h"

namespace Render
{

CharacterJoint::CharacterJoint()
{
  
}

CharacterJoint::~CharacterJoint()
{
  
}

void CharacterJoint::Setup(const int& parentJointIndex, CharacterJoint* parentJoint, const Math::Vector4& poseTranslation, const string& name, const Math::Quaternion& poseRotation, const Math::Vector4& poseScale)
{
  this->parentJointIndex = parentJointIndex;
  this->parentJoint = parentJoint;
  this->poseTranslation = poseTranslation;
  this->poseRotation = poseRotation;
  this->poseScale = poseScale;
  this->name = name;
}

Math::Matrix4 CharacterJoint::EvaluateInverseMatrix()
{
  if(this->parentJointIndex == -1)
  {
		this->poseMatrix = Math::Matrix4::RotationQuaternion(this->poseRotation) * Math::Matrix4::translation(this->poseTranslation);
		this->currentMatrix = this->poseMatrix;
		this->invPoseMatrix = poseMatrix.invert();
  }
  else
  { 
		this->poseMatrix = Math::Matrix4::RotationQuaternion(this->poseRotation) * Math::Matrix4::translation(this->poseTranslation) * this->parentJoint->GetEvaluatedMatrix();
		this->currentMatrix = this->poseMatrix;
		this->invPoseMatrix = poseMatrix.invert();
  }
}

Math::Matrix4 CharacterJoint::EvaluateMatrix(const Math::Vector4& scale, const Math::Quaternion& rotation, const Math::Vector4& translation)
{
  if(this->parentJointIndex == -1)
  {
	  this->currentMatrix = Math::Matrix4::RotationQuaternion(rotation) * Math::Matrix4::translation(translation);
  }
  else
  { 
	  this->currentMatrix = Math::Matrix4::RotationQuaternion(rotation) * Math::Matrix4::translation(translation) * this->parentJoint->GetEvaluatedMatrix();
  }
}

Math::Matrix4 CharacterJoint::GetEvaluatedMatrix()
{
  return this->currentMatrix;
}

void CharacterJoint::ResetMatrix()
{
	this->currentMatrix = Math::Matrix4::scale(this->poseScale) * Math::Matrix4::RotationQuaternion(this->poseRotation) * Math::Matrix4::translation(this->poseTranslation);
}

CharacterJoint* CharacterJoint::getParentJoint()
{
  return this->parentJoint;
}

Math::Vector4& CharacterJoint::getPoseTranslation()
{
  return this->poseTranslation;
}

Math::Quaternion& CharacterJoint::getPoseRotation()
{
  return this->poseRotation;
}

Math::Matrix4 CharacterJoint::GetPoseMatrix()
{
  return this->poseMatrix;
}

Math::Matrix4 CharacterJoint::GetInversePoseMatrix()
{
  return this->invPoseMatrix;
}

}