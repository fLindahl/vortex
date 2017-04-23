#pragma once
/**
	@class	Game::Attribute

	Contains a pointer to a variable within a property.
	The difference between this class and the Util::Variable is that this only contains a pointer, not a value.

*/

#include "core/types.h"

namespace Util
{
	class String;
}

namespace Math
{
	class vec4;
	class mat4;
	class point;
	class vector;
	class quaternion;
}

namespace Game
{
	class Attribute
	{
	public:
		enum Type
		{
			Void,
			Int,
			UInt,
			Float,
			Double,
			Char,
			UChar,
			Short,
			UShort,
			Bool,
			Vector4,
			Matrix4,
			Point,
			Vector,
			Quaternion,
			String
		};
		
		Attribute();

		virtual ~Attribute();

		explicit Attribute(void* ptr);
		explicit Attribute(int* ptr);
		explicit Attribute(uint* ptr);
		explicit Attribute(float* ptr);
		explicit Attribute(double* ptr);
		explicit Attribute(char* ptr);
		explicit Attribute(uchar* ptr);
		explicit Attribute(short* ptr);
		explicit Attribute(ushort* ptr);
		explicit Attribute(bool* ptr);
		explicit Attribute(Math::vec4* ptr);
		explicit Attribute(Math::mat4* ptr);
		explicit Attribute(Math::point* ptr);
		explicit Attribute(Math::vector* ptr);
		explicit Attribute(Math::quaternion* ptr);
		explicit Attribute(Util::String* ptr);

		///returns the type
		Type GetType() const;
		/// clear content, sets to undefined value / void type
		void Clear();

		///Set operators for all pointertypes we support
		void operator=(void* ptr);
		void operator=(float* ptr);
		void operator=(double* ptr);
		void operator=(int* ptr);
		void operator=(uint* ptr);
		void operator=(char* ptr);
		void operator=(uchar* ptr);
		void operator=(short* ptr);
		void operator=(ushort* ptr);
		void operator=(bool* ptr);
		void operator=(Math::vec4* ptr);
		void operator=(Math::mat4* ptr);
		void operator=(Math::point* ptr);
		void operator=(Math::vector* ptr);
		void operator=(Math::quaternion* ptr);
		void operator=(Util::String* ptr);

		///Public read write access functions for all values
		float&				RW_Float()		{ _assert(this->type == Float);		 return *this->flt; }
		double&				RW_Double()		{ _assert(this->type == Double);	 return *this->dbl; }
		int&				RW_Int()		{ _assert(this->type == Int);		 return *this->i; }
		uint&				RW_UInt()		{ _assert(this->type == UInt);		 return *this->ui; }
		char&				RW_Char()		{ _assert(this->type == Char);		 return *this->ch; }
		uchar&				RW_UChar()		{ _assert(this->type == UChar);		 return *this->uch; }
		short&				RW_Short()		{ _assert(this->type == Short);		 return *this->shrt; }
		ushort&				RW_UShort()		{ _assert(this->type == UShort);	 return *this->ushrt; }
		bool&				RW_Bool()		{ _assert(this->type == Bool);		 return *this->blean; }
		void*				RW_GetVoidPtr()	{ _assert(this->type == Void);		 return  this->vo; }
		Math::vec4&			RW_Vec4()		{ _assert(this->type == Vector4);	 return *this->vec4; }
		Math::mat4&			RW_Mat4()		{ _assert(this->type == Matrix4);	 return *this->mat4; }
		Math::point&		RW_Point()		{ _assert(this->type == Point);		 return *this->point; }
		Math::vector&		RW_Vector()		{ _assert(this->type == Vector);	 return *this->vector; }
		Math::quaternion&	RW_Quaternion()	{ _assert(this->type == Quaternion); return *this->quat; }
		Util::String&		RW_String()		{ _assert(this->type == String);	 return *this->str; }


	protected:
		///Contains all the pointers that this class can fill. The size will only be of one pointer. @note Don't add anything but pointers to this union!
		union
		{
			float*				flt;
			double*				dbl;
			int*				i;
			uint*				ui;
			char*				ch;
			uchar*				uch;
			short*				shrt;
			ushort*				ushrt;
			bool*				blean;
			void*				vo;
			Math::vec4*			vec4;
			Math::mat4*			mat4;
			Math::point*		point;
			Math::vector*		vector;
			Math::quaternion*	quat;
			Util::String*		str;
		};

		Type type;
	};

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute()
	{
		//Empty
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(void* ptr) :
		type(Void),
		vo(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(int* ptr) :
		type(Int),
		i(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(uint* ptr) :
		type(UInt),
		ui(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(float* ptr) :
		type(Float),
		flt(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(double* ptr) :
		type(Double),
		dbl(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(char* ptr) :
		type(Char),
		ch(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(uchar* ptr) :
		type(UChar),
		uch(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(short* ptr) :
		type(Short),
		shrt(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(ushort* ptr) :
		type(UShort),
		ushrt(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(bool* ptr) :
		type(Bool),
		blean(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(Math::vec4* ptr) :
		type(Vector4),
		vec4(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(Math::mat4* ptr) :
		type(Matrix4),
		mat4(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(Math::point* ptr) :
		type(Point),
		point(ptr)
	{

	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(Math::vector* ptr) :
		type(Vector),
		vector(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(Math::quaternion* ptr) :
		type(Quaternion),
		quat(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Attribute(Util::String* ptr) :
		type(String),
		str(ptr)
	{
	}

	//---------------------------------------------------------
	/**
	*/
	inline Attribute::Type Attribute::GetType() const
	{
		return this->type;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::Clear()
	{
		this->type = Void;
		this->vo = nullptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(void * ptr)
	{
		this->type = Void;
		this->vo = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(float * ptr)
	{
		this->type = Float;
		this->flt = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(double * ptr)
	{
		this->type = Double;
		this->dbl = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(int * ptr)
	{
		this->type = Int;
		this->i = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(uint * ptr)
	{
		this->type = UInt;
		this->ui = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(char * ptr)
	{
		this->type = Char;
		this->ch = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(uchar * ptr)
	{
		this->type = UChar;
		this->uch = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(short * ptr)
	{
		this->type = Short;
		this->shrt = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(ushort * ptr)
	{
		this->type = UShort;
		this->ushrt = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(bool * ptr)
	{
		this->type = Bool;
		this->blean = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(Math::vec4 * ptr)
	{
		this->type = Vector4;
		this->vec4 = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(Math::mat4 * ptr)
	{
		this->type = Matrix4;
		this->mat4 = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(Math::point * ptr)
	{
		this->type = Point;
		this->point = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(Math::vector * ptr)
	{
		this->type = Vector;
		this->vector = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(Math::quaternion * ptr)
	{
		this->type = Quaternion;
		this->quat = ptr;
	}

	//---------------------------------------------------------
	/**
	*/
	inline void Attribute::operator=(Util::String * ptr)
	{
		this->type = String;
		this->str = ptr;
	}
}
