#pragma once
#include "foundation/math/quaternion.h"

namespace Render
{

class CurveType
{
public:
    /// animation curve types
    enum Code
    {
        Translation,    //> keys in curve describe a translation
        Scale,          //> keys in curve describe a scale
        Rotation,       //> keys in curve describe a rotation quaternion
        Color,          //> keys in curve describe a color
        Velocity,       //> keys describe a linear velocition
        Float4,         //> generic 4D key

        NumCurveTypes,
        InvalidCurveType,
    };

    /// convert from string
    static Code FromString(const char* str)
    {
      if (str == "Translation") return Translation;
      else if (str == "Scale") return Scale;
      else if (str == "Rotation") return Rotation;
      else if (str == "Color") return Color;
      else if (str == "Velocity") return Velocity;
      else if (str == "Float4") return Float4;
      else
      {
		return InvalidCurveType;
      }
    };
    
    /// convert to string
    static char* ToString(Code c)
    {
		switch (c)
		{
			case Translation:   return "Translation";
			case Scale:         return "Scale";
			case Rotation:      return "Rotation";
			case Color:         return "Color";
			case Velocity:      return "Velocity";
			case Float4:        return "Float4";
			default:
				return "CurveType::ToString(): invalid curve type code!";
		}
    };

};

class AnimCurve
{
public:
  AnimCurve();
  ~AnimCurve();
  
  /// activate/deactivate the curve, only active curves will be evaluated
  void SetActive(bool b);
  /// return true if the animation curve is active
  bool IsActive() const;
  /// set/clear the static flag
  void SetStatic(bool b);
  /// return true if the curve is static
  bool IsStatic() const;
  /// set the static key of the curve
  void SetStaticKey(const Math::vec4& staticKey);
  /// get the static key of the curve
  const Math::vec4& GetStaticKey() const;
  /// set index of the first key in the AnimKeyBuffer
  void SetFirstKeyIndex(int index);
  /// get index of the first key in the AnimKeyBuffer
  int GetFirstKeyIndex() const;
  /// set the curve type
  void SetCurveType(CurveType::Code curveType);
  /// get the curve type
  CurveType::Code GetCurveType() const;
  
  
private:
	Math::vec4 staticKey;
  int firstKeyIndex;
  CurveType::Code curveType;
  bool isActive;
  bool isStatic;
  
};


inline AnimCurve::AnimCurve() :
    staticKey(0.0f, 0.0f, 0.0f, 0.0f),
    firstKeyIndex(0),
    curveType(CurveType::Float4),
    isActive(true),
    isStatic(false)
{
    // empty
}

inline AnimCurve::~AnimCurve()
{

}

inline void AnimCurve::SetActive(bool b)
{
    this->isActive = b;
}

inline bool AnimCurve::IsActive() const
{
    return this->isActive;
}

inline void AnimCurve::SetStatic(bool b)
{
    this->isStatic = b;
}

inline bool AnimCurve::IsStatic() const
{
    return this->isStatic;
}

inline void AnimCurve::SetStaticKey(const Math::vec4& key)
{
    this->staticKey = key;
}

inline const Math::vec4& AnimCurve::GetStaticKey() const
{
    return this->staticKey;
}

inline void AnimCurve::SetFirstKeyIndex(int i)
{
    this->firstKeyIndex = i;
}

inline int AnimCurve::GetFirstKeyIndex() const
{
    return this->firstKeyIndex;
}

inline void AnimCurve::SetCurveType(CurveType::Code t)
{
    this->curveType = t;
}

inline CurveType::Code AnimCurve::GetCurveType() const
{
    return this->curveType;
}

}