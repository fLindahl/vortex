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

void CharacterJoint::Setup(const int& parentJointIndex, CharacterJoint* parentJoint, const Math::vec4& poseTranslation, const std::string& name, const Math::quaternion& poseRotation, const Math::vec4& poseScale)
{
  this->parentJointIndex = parentJointIndex;
  this->parentJoint = parentJoint;
  this->poseTranslation = poseTranslation;
  this->poseRotation = poseRotation;
  this->poseScale = poseScale;
  this->name = name;
}

void CharacterJoint::EvaluateInverseMatrix()
{
  if(this->parentJointIndex == -1)
  {
		this->poseMatrix = Math::mat4::multiply(Math::mat4::rotationquaternion(this->poseRotation), Math::mat4::translation(this->poseTranslation));
		this->currentMatrix = this->poseMatrix;
		this->invPoseMatrix = Math::mat4::inverse(poseMatrix);
  }
  else
  { 
		this->poseMatrix = Math::mat4::multiply(Math::mat4::rotationquaternion(this->poseRotation), Math::mat4::translation(this->poseTranslation));
		this->poseMatrix = Math::mat4::multiply(this->poseMatrix,this->parentJoint->GetEvaluatedMatrix());
		this->currentMatrix = this->poseMatrix;
		this->invPoseMatrix = Math::mat4::inverse(poseMatrix);
  }
}

void CharacterJoint::EvaluateMatrix(const Math::vec4& scale, const Math::quaternion& rotation, const Math::vec4& translation)
{
  if(this->parentJointIndex == -1)
  {
	  this->currentMatrix = Math::mat4::multiply(Math::mat4::rotationquaternion(rotation), Math::mat4::translation(translation));
  }
  else
  { 
	  this->currentMatrix = Math::mat4::multiply(Math::mat4::rotationquaternion(rotation), Math::mat4::translation(translation));
	  this->currentMatrix = Math::mat4::multiply(this->currentMatrix, this->parentJoint->GetEvaluatedMatrix());
  }
}

Math::mat4 CharacterJoint::GetEvaluatedMatrix()
{
  return this->currentMatrix;
}

void CharacterJoint::ResetMatrix()
{
	this->currentMatrix = Math::mat4::multiply(Math::mat4::scaling(this->poseScale), Math::mat4::rotationquaternion(this->poseRotation));
	this->currentMatrix = Math::mat4::multiply(this->currentMatrix, Math::mat4::translation(this->poseTranslation));
}

CharacterJoint* CharacterJoint::getParentJoint()
{
  return this->parentJoint;
}

Math::vec4& CharacterJoint::getPoseTranslation()
{
  return this->poseTranslation;
}

Math::quaternion& CharacterJoint::getPoseRotation()
{
  return this->poseRotation;
}

Math::mat4 CharacterJoint::GetPoseMatrix()
{
  return this->poseMatrix;
}

Math::mat4 CharacterJoint::GetInversePoseMatrix()
{
  return this->invPoseMatrix;
}

}