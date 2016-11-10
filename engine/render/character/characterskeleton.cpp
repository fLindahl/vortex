#include "config.h"
#include "characterskeleton.h"
#include "foundation/math/vector4.h"
#include "foundation/math/quaternion.h"
#include "GLFW/glfw3.h"
#include "render/animation/animationresource.h"
#include "render/animation/animationclip.h"

namespace Render
{

CharacterSkeleton::CharacterSkeleton()
{
  animationFrame = 0;
  frameTime = 0;
  time = glfwGetTime();
  
}

CharacterSkeleton::~CharacterSkeleton()
{

}

/* Load
 * Loads a skeleton from nebulas .constants file (xml)
 * Parsing is done with tinyxml
 */
void CharacterSkeleton::Load(const char* fileName)
{
  //Parse file!
  tinyxml2::XMLDocument data;
  data.LoadFile(fileName);
    
  tinyxml2::XMLElement* element = data.RootElement();
  
  //Find first joint
  while (true)
  {
    if(element->FirstChildElement("Joint") == nullptr)
    {
      element = element->FirstChildElement();
      continue;
    }
    else
    {
      element = element->FirstChildElement("Joint");
      break;
    }  
  }
  
  tinyxml2::XMLElement* rootJointElement = element;
  
  //Start by counting how many joints we need to allocate
  int jointArraySize = 0;
  while (element != nullptr)
  {
    jointArraySize++;
    element = element->NextSiblingElement();
  } 

  this->jointArray.reserve(jointArraySize);
  
  for (int a = 0; a < jointArraySize; a++)
    this->jointArray.push_back(new CharacterJoint());
  
  //Fill joint array with joints
  element = rootJointElement;
  int i = 0;
  const char* value;
  while (element != nullptr)
  {
    
    //Joint data
    std::string name;
    int index;
    int parent;
    Math::vec4 position;
    Math::quaternion rotation;
	Math::vec4 scale;
    
    //NAME
    const tinyxml2::XMLAttribute* attribute = element->FirstAttribute();
    value = attribute->Value();
    name = value;
    
    //INDEX
    attribute = attribute->Next();
    value = attribute->Value();
    index = atoi(value);
    
    //PARENT
    attribute = attribute->Next();
    value = attribute->Value();
    parent = atoi(value);
    
    //POSITION
    attribute = attribute->Next();
    value = attribute->Value();
    float x,y,z,w;
    sscanf(value, "%f,%f,%f,%f", &x, &y, &z, &w);
    position = Math::vec4(x,y,z,w);
    
    //ROTATION
    attribute = attribute->Next();
    value = attribute->Value();
    sscanf(value, "%f,%f,%f,%f", &x, &y, &z, &w);
    rotation = Math::quaternion(x,y,z,w);
    
    //SCALE
    attribute = attribute->Next();
    value = attribute->Value();
    sscanf(value, "%f,%f,%f,%f", &x, &y, &z, &w);
    scale = Math::vec4(x,y,z,w);
    
    
    CharacterJoint* parentJoint;
    if(parent < 0)
      parentJoint = nullptr;
    else
      parentJoint = this->jointArray[parent];
    
    this->jointArray[index]->Setup(parent, parentJoint, position, name, rotation, scale);
    
    element = element->NextSiblingElement();
  }
  
  //
  // Parse Skins
  
  element = data.RootElement();
  
  //Find first Skin
  
  //go to skins elements.
  tinyxml2::XMLElement* firstSkin = element->FirstChildElement()->FirstChildElement()->NextSiblingElement();
  
  skin = new int[jointArraySize];
  
  element = firstSkin->FirstChildElement()->FirstChildElement()->FirstChildElement();
  
  const char* list = element->GetText();
  
  sscanf(list, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",
	 &skin[0], &skin[1], &skin[2], &skin[3], &skin[4], &skin[5], &skin[6], &skin[7], &skin[8], &skin[9], &skin[10],
	 &skin[11], &skin[12], &skin[13], &skin[14], &skin[15], &skin[16], &skin[17], &skin[18], &skin[19], &skin[20]);
  
  
  
  for (CharacterJoint* J : this->jointArray)
  {
    J->EvaluateInverseMatrix();    
  }
  
  
}

void CharacterSkeleton::Setup(const int& jointArraySize)
{

}

void CharacterSkeleton::SetupJoint(const int& jointIndex, const Math::vec4& position, const std::string& jointName)
{

}

std::vector<Math::mat4> CharacterSkeleton::EvaluateSkeleton(const int &animIndex)
{
  AnimationClip clip = animator->GetClipByIndex(animIndex);
  
  double frameTime = (glfwGetTime() - time) * 1000.0f;
  if(frameTime > clip.GetKeyDuration())
  {
    time = glfwGetTime();
    frameTime = 0.0f;    
    animationFrame = (animationFrame + 1 >= clip.GetNumKeys() ? 0 : animationFrame + 1);
  }
  
  const AnimKeyBuffer* keyBuffer = animator->GetKeyBuffer();
    
  double t = frameTime / clip.GetKeyDuration();
  
  for (index_t index = 0; index < this->jointArray.size(); index++)
  {
    CharacterJoint* joint = this->jointArray[index];
    //joint->ResetMatrix();
    
    int numJointCurves = clip.GetNumCurves() / jointArray.size();
    
	Math::vec4 jointTranslation;
	Math::vec4 nextJointTranslation;

	Math::quaternion jointRotation;
	Math::quaternion nextJointRotation;
    
	Math::vec4 jointScale;
	Math::vec4 nextJointScale;
    
	Math::vec4 jointVelocity;
	Math::vec4 nextJointVelocity;
    
    int nextAnimationFrame = (animationFrame + 1 >= clip.GetNumKeys() ? 0 : animationFrame + 1);
    
    for (int i = 0; i < numJointCurves; i++)
    {
		AnimCurve jointCurve = clip.CurveByIndex((index * numJointCurves) + i);
      
		Math::vec4* key = (keyBuffer->GetKeyBufferPointer() + (jointCurve.GetFirstKeyIndex() + animationFrame * clip.GetKeyStride()));
      
		Math::vec4* nextKey = (keyBuffer->GetKeyBufferPointer() + (jointCurve.GetFirstKeyIndex() + nextAnimationFrame * clip.GetKeyStride()));
      
		if(jointCurve.GetCurveType() == CurveType::Code::Translation)
		{
			jointTranslation = *key;
			nextJointTranslation = *nextKey;
		}
		else if(jointCurve.GetCurveType() == CurveType::Code::Rotation)
		{
			jointRotation = Math::quaternion(*key);
			nextJointRotation = Math::quaternion(*nextKey);
		}
		else if(jointCurve.GetCurveType() == CurveType::Code::Scale)
		{
			jointScale = *key;
			nextJointScale = *nextKey;
		}
		else if(jointCurve.GetCurveType() == CurveType::Code::Velocity)
		{
			jointVelocity = *key;
			nextJointVelocity = *nextKey;
		}
      
    }
    
	jointRotation = Math::quaternion::slerp(jointRotation, nextJointRotation, (float)t);
    
	jointTranslation = Math::vec4::lerp(jointTranslation, nextJointTranslation, (float)t);
    
    joint->EvaluateMatrix(jointScale, jointRotation, jointTranslation);
  }
  
  std::vector<Math::mat4> ret;
  
  for (index_t index = 0; index < this->jointArray.size(); index++)
  { 
    CharacterJoint* joint = this->jointArray[skin[index]];    
    
    ret.push_back((Math::mat4::multiply(joint->GetInversePoseMatrix(), joint->GetEvaluatedMatrix())));
  }
  
  return ret;
}

void CharacterSkeleton::SetAnimationResource(AnimationResource* animator)
{
  this->animator = animator;
}

}