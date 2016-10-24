#pragma once
/* --------------------------------------------------------
A variable that can act as any type.

 -- Do not use unless there's a good reason for it! --
 ----------------------------------------------------------
 */
//#include "config.h"
#include "array.h"
#include "foundation/math/vector2.h"
#include "foundation/math/vector3.h"
#include "foundation/math/vector4.h"
#include "foundation/math/matrix4.h"
//#include "foundation/util/string.h"

namespace Util
{
	class Variable
	{
	public:
		enum Type
		{
			Void,
			Int,
			UInt,
			Float,
			Bool,
			Vector2,
			Vector3,
			Vector4,
			String,
			Matrix4,
			VoidPtr,
			IntArray,
			FloatArray,
			BoolArray,
			Vector2Array,
			Vector3Array,
			Vector4Array,
			StringArray,
			Matrix4Array,
		};

		// default constructor. Undefined value
		Variable();

		// Various constructors for directly setting values
		Variable(const Type& type, const Util::String& value);
		Variable(int value);
		Variable(uint value);
		Variable(float value);
		Variable(bool value);
		Variable(const Math::Vector2& value);
		Variable(const Math::Vector3& value);
		Variable(const Math::Vector4& value);
		Variable(const Util::String& value);
		Variable(const Math::Matrix4& value);
		Variable(void* value);
		Variable(const Array<int>& value);
		Variable(const Array<float>& value);
		Variable(const Array<bool>& value);
		Variable(const Array<Math::Vector2>& value);
		Variable(const Array<Math::Vector3>& value);
		Variable(const Array<Math::Vector4>& value);
		Variable(const Array<Util::String>& value);
		Variable(const Array<Math::Matrix4>& value);
		Variable(const Variable& value);

		// destructor
		~Variable();

		static Variable::Type StringToType(const Util::String& str);

		// set type of attribute
		void SetType(Type t);
		// get type
		Type GetType() const;
		// clear content, sets to undefined value / void type
		void Clear();

		//Get functions!
		Util::String* GetString() const;
		float* GetFloat();
		float* GetVector3();
		float* GetVector4();

		//Operators!
		void operator=(const Variable& rhs);
		void operator=(int val);
		void operator=(uint val);
		void operator=(float val);
		void operator=(bool val);
		void operator=(const Math::Vector2& val);
		void operator=(const Math::Vector3& val);
		void operator=(const Math::Vector4& val);
		void operator=(const Math::Matrix4& val);
		void operator=(const Util::String& s);
		void operator=(void* ptr);
		void operator=(const Util::Array<int>& rhs);
		void operator=(const Util::Array<float>& rhs);
		void operator=(const Util::Array<bool>& rhs);
		void operator=(const Util::Array<Math::Vector2>& rhs);
		void operator=(const Util::Array<Math::Vector3>& rhs);
		void operator=(const Util::Array<Math::Vector4>& rhs);
		void operator=(const Util::Array<Math::Matrix4>& rhs);
		void operator=(const Util::Array<Util::String>& rhs);
		
		bool operator==(const Variable& rhs) const;
		bool operator==(int rhs) const;
		bool operator==(uint rhs) const;
		bool operator==(float rhs) const;
		bool operator==(bool rhs) const;
		bool operator==(const Math::Vector2& rhs) const;
		bool operator==(const Math::Vector3& rhs) const;
		bool operator==(const Math::Vector4& rhs) const;
		bool operator==(const Util::String& rhs) const;
		bool operator==(void* ptr) const;

		bool operator!=(const Variable& rhs) const;
		bool operator!=(int rhs) const;
		bool operator!=(uint rhs) const;
		bool operator!=(float rhs) const;
		bool operator!=(bool rhs) const;
		bool operator!=(const Math::Vector2& rhs) const;
		bool operator!=(const Math::Vector3& rhs) const;
		bool operator!=(const Math::Vector4& rhs) const;
		bool operator!=(const Util::String& rhs) const;
		bool operator!=(void* ptr) const;

		bool operator>(const Variable& rhs) const;
		bool operator<(const Variable& rhs) const;
		bool operator>=(const Variable& rhs) const;
		bool operator<=(const Variable& rhs) const;
	private:
		// delete content
		void Delete();
		// copy content
		void Copy(const Variable& rhs);

		Type type;

		union
		{
			int i;
			uint ui;
			bool b;
			float f[4];
			Math::Matrix4* m;
			Util::String* string;
			void* voidPtr;
			Util::Array<int>* intArray;
			Util::Array<float>* floatArray;
			Util::Array<bool>* boolArray;
			Util::Array<Math::Vector2>* vector2Array;
			Util::Array<Math::Vector3>* vector3Array;
			Util::Array<Math::Vector4>* vector4Array;
			Util::Array<Math::Matrix4>* matrix4Array;
			Util::Array<Util::String>* stringArray;
		};
	};

	inline Variable::Variable() :
		type(Void),
		string(0)
	{
	}

	inline Variable::~Variable()
	{
		this->Delete();
	}

	inline Variable::Variable(const Type& type, const Util::String& value) :
		type(type)
	{
		switch (type)
		{
		case String:
			this->string = new Util::String(value);
			return;

		case Matrix4:
			float flts[16];
			sscanf(value.c_str(), "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", &flts[0], &flts[1], &flts[2], &flts[3], &flts[4], &flts[5], &flts[6], &flts[7], &flts[8], &flts[9], &flts[10], &flts[11], &flts[12], &flts[13], &flts[14], &flts[15]);
			
			this->m = new Math::Matrix4(flts);
			
			return;

		case Float:
			float flt;
			sscanf(value.c_str(), "%f", &flt);

			this->f[0] = flt;

			return;

		case Vector2:
			float flts2[2];
			sscanf(value.c_str(), "%f, %f", &flts2[0], &flts2[1]);

			this->f[0] = flts2[0];
			this->f[1] = flts2[1];

			return;

		case Vector3:
			float flts3[3];
			sscanf(value.c_str(), "%f, %f, %f", &flts3[0], &flts3[1], &flts3[2]);

			this->f[0] = flts3[0];
			this->f[1] = flts3[1];
			this->f[2] = flts3[2];

			return;

		case Vector4:
			float flts4[4];
			sscanf(value.c_str(), "%f, %f, %f, %f", &flts4[0], &flts4[1], &flts4[2], &flts4[3]);

			this->f[0] = flts4[0];
			this->f[1] = flts4[1];
			this->f[2] = flts4[2];
			this->f[3] = flts4[3];

			return;

		default:
			printf("ERROR: Variable constructor not implemented\n");
			assert(false);
			return;
		}
	}

	inline Variable::Type Variable::StringToType(const Util::String& str)
	{
		if (str == "string")
		{
			return String;
		}
		else if (str == "void")
		{
			return Void;
		}
		else if (str == "int")
		{
			return Int;
		}
		else if (str == "uint")
		{
			return UInt;
		}
		else if (str == "float")
		{
			return Float;
		}
		else if (str == "bool")
		{
			return Bool;
		}
		else if (str == "vector2")
		{
			return Vector2;
		}
		else if (str == "vector3")
		{
			return Vector3;
		}
		else if (str == "vector4")
		{
			return Vector4;
		}
		else if (str == "matrix4")
		{
			return Matrix4;
		}
		else if (str == "voidPtr")
		{
			return VoidPtr;
		}
		else if (str == "intArray")
		{
			return IntArray;
		}
		else if (str == "floatArray")
		{
			return FloatArray;
		}
		else if (str == "boolArray")
		{
			return BoolArray;
		}
		else if (str == "vector2Array")
		{
			return Vector2Array;
		}
		else if (str == "vector3Array")
		{
			return Vector3Array;
		}
		else if (str == "vector4Array")
		{
			return Vector4Array;
		}
		else if (str == "stringArray")
		{
			return StringArray;
		}
		else if (str == "matrix4Array")
		{
			return Matrix4Array;
		}
		else
		{
			printf("ERROR: No such variable!");
			assert(false);
		}
		
		return Void;
	}

	inline void Variable::Delete()
	{
		switch (this->type)
		{
		case String:
			delete this->string;
			this->string = 0;
			break;

		case Matrix4:
			delete this->m;
			this->m = 0;
			break;

		case VoidPtr:
			this->voidPtr = 0;
			break;

		case IntArray:
			delete this->intArray;
			this->intArray = 0;
			break;

		case FloatArray:
			delete this->floatArray;
			this->floatArray = 0;
			break;

		case BoolArray:
			delete this->boolArray;
			this->boolArray = 0;
			break;
			
		case Vector2Array:
			delete this->vector2Array;
			this->vector2Array = 0;
			break;

		case Vector3Array:
			delete this->vector3Array;
			this->vector3Array = 0;
			break;

		case Vector4Array:
			delete this->vector4Array;
			this->vector4Array = 0;
			break;
			
		case Matrix4Array:
			delete this->matrix4Array;
			this->matrix4Array = 0;
			break;

		case StringArray:
			delete this->stringArray;
			this->stringArray = 0;
			break;
			
		case Void:
			break;

		default:
			break;
		}

		//set type to void so that we dont do anything stupid...
		this->type = Void;
	}

	inline void	Variable::Clear()
	{
		this->Delete();
	}

	inline Util::String* Variable::GetString() const
	{
		return this->string;
	}

	inline float* Variable::GetFloat()
	{
		return f;
	}

	inline float* Variable::GetVector3()
	{
		return this->f;
	}

	inline float* Variable::GetVector4()
	{
		return this->f;
	}

	inline void Variable::Copy(const Variable& rhs)
	{
		assert(Void == this->type);
		this->type = rhs.type;
		switch (rhs.type)
		{
		case Void:
			break;
		case Int:
			this->i = rhs.i;
			break;
		case UInt:
			this->ui = rhs.ui;
			break;
		case Float:
			this->f[0] = rhs.f[0];
			break;
		case Bool:
			this->b = rhs.b;
			break;
		case Vector2:
			this->f[0] = rhs.f[0];
			this->f[1] = rhs.f[1];
		case Vector3:
			this->f[0] = rhs.f[0];
			this->f[1] = rhs.f[1];
			this->f[2] = rhs.f[2];
			break;
		case Vector4:
			this->f[0] = rhs.f[0];
			this->f[1] = rhs.f[1];
			this->f[2] = rhs.f[2];
			this->f[3] = rhs.f[3];
			break;
		case String:
			this->Delete();
			this->string = new Util::String(*rhs.string);
			break;
		case Matrix4:
			this->Delete();
			this->m = new Math::Matrix4(*rhs.m);
			break;
		case VoidPtr:
			this->voidPtr = rhs.voidPtr;
			break;
		case IntArray:
			this->Delete();
			this->intArray = new Util::Array<int>(*rhs.intArray);
			break;
		case FloatArray:
			this->Delete();
			this->floatArray = new Util::Array<float>(*rhs.floatArray);
			break;
		case BoolArray:
			this->Delete();
			this->boolArray = new Util::Array<bool>(*rhs.boolArray);
			break;
		case Vector2Array:
			this->Delete();
			this->vector2Array = new Util::Array<Math::Vector2>(*rhs.vector2Array);
			break;
		case Vector3Array:
			this->Delete();
			this->vector3Array = new Util::Array<Math::Vector3>(*rhs.vector3Array);
			break;
		case Vector4Array:
			this->Delete();
			this->vector4Array = new Util::Array<Math::Vector4>(*rhs.vector4Array);
			break;
		case Matrix4Array:
			this->Delete();
			this->matrix4Array = new Util::Array<Math::Matrix4>(*rhs.matrix4Array);
			break;
		case StringArray:
			this->Delete();
			this->stringArray = new Util::Array<Util::String>(*rhs.stringArray);
			break;
		default:
			//invalid type!
			assert(false);
			break;
		}
	}

	inline Variable::Variable(const Variable& rhs) :
		type(Void)
	{
		this->Copy(rhs);
	}

	inline Variable::Variable(int rhs) :
		type(Int),
		i(rhs)
	{
	}

	inline Variable::Variable(uint rhs) :
		type(UInt),
		ui(rhs)
	{
	}

	inline Variable::Variable(float rhs) :
		type(Float)
	{
		this->f[0] = rhs;
	}

	inline Variable::Variable(bool rhs) :
		type(Bool),
		b(rhs)
	{
	}

	inline Variable::Variable(const Math::Vector2& rhs) :
		type(Vector2)
	{
		this->f[0] = rhs.x();
		this->f[1] = rhs.y();
	}

	inline Variable::Variable(const Math::Vector3& rhs) :
		type(Vector3)
	{
		this->f[0] = rhs.x();
		this->f[1] = rhs.y();
		this->f[2] = rhs.z();
	}

	inline Variable::Variable(const Math::Vector4& rhs) :
		type(Vector4)
	{
		this->f[0] = rhs.x();
		this->f[1] = rhs.y();
		this->f[2] = rhs.z();
		this->f[3] = rhs.w();
	}

	inline Variable::Variable(const Util::String& rhs) :
		type(String)
	{
		if (this->string != nullptr)
			delete this->string;

		this->string = new Util::String(rhs);
	}

	inline Variable::Variable(void* ptr) :
		type(VoidPtr)
	{
		this->voidPtr = ptr;
	}

	inline Variable::Variable(const Math::Matrix4& rhs) :
		type(Matrix4)
	{
		if (this->matrix4Array != nullptr)
			delete this->matrix4Array;

		this->m = new Math::Matrix4(rhs);
	}

	inline Variable::Variable(const Util::Array<int>& rhs) :
		type(IntArray)
	{
		if (this->intArray!= nullptr)
			delete this->intArray;

		this->intArray = new Util::Array<int>(rhs);
	}

	inline Variable::Variable(const Util::Array<float>& rhs) :
		type(FloatArray)
	{
		if (this->floatArray != nullptr)
			delete this->floatArray;

		this->floatArray = new Util::Array<float>(rhs);
	}

	inline Variable::Variable(const Util::Array<bool>& rhs) :
		type(BoolArray)
	{
		if (this->boolArray != nullptr)
			delete this->boolArray;

		this->boolArray = new Util::Array<bool>(rhs);
	}

	inline Variable::Variable(const Util::Array<Math::Vector2>& rhs) :
		type(Vector2Array)
	{
		if (this->vector2Array != nullptr)
			delete this->vector2Array;

		this->vector2Array = new Util::Array<Math::Vector2>(rhs);
	}

	inline Variable::Variable(const Util::Array<Math::Vector3>& rhs) :
		type(Vector3Array)
	{
		if (this->vector3Array != nullptr)
			delete this->vector3Array;

		this->vector3Array = new Util::Array<Math::Vector3>(rhs);
	}

	inline Variable::Variable(const Util::Array<Math::Vector4>& rhs) :
		type(Vector4Array)
	{
		this->vector4Array = new Util::Array<Math::Vector4>(rhs);
	}

	inline Variable::Variable(const Util::Array<Math::Matrix4>& rhs) :
		type(Matrix4Array)
	{
		if (this->matrix4Array != nullptr)
			delete this->matrix4Array;

		this->matrix4Array = new Util::Array<Math::Matrix4>(rhs);
	}

	inline Variable::Variable(const Util::Array<Util::String>& rhs) :
		type(StringArray)
	{
		if (this->stringArray != nullptr)
			delete this->stringArray;

		this->stringArray = new Util::Array<Util::String>(rhs);
	}

	inline void	Variable::SetType(Type t)
	{
		this->Delete();
		this->type = t;
		switch (t)
		{
		case String:
			this->string = new Util::String();
			break;
		case Matrix4:
			this->m = new Math::Matrix4();
			break;
		case VoidPtr:
			this->voidPtr = 0;
			break;
		case IntArray:
			this->intArray = new Util::Array<int>();
			break;
		case FloatArray:
			this->floatArray = new Util::Array<float>();
			break;
		case BoolArray:
			this->boolArray = new Util::Array<bool>();
			break;
		case Vector2Array:
			this->vector2Array = new Util::Array<Math::Vector2>();
			break;
		case Vector3Array:
			this->vector3Array = new Util::Array<Math::Vector3>();
			break;
		case Vector4Array:
			this->vector4Array = new Util::Array<Math::Vector4>();
			break;
		case Matrix4Array:
			this->matrix4Array = new Util::Array<Math::Matrix4>();
			break;
		case StringArray:
			this->stringArray = new Util::Array<Util::String>();
			break;
		default:
			break;
		}
	}

	
	inline Variable::Type Variable::GetType() const
	{
		return this->type;
	}

	inline void Variable::operator=(const Variable& rhs)
	{
		this->Delete();
		this->Copy(rhs);
	}

	inline void Variable::operator=(int val)
	{
		this->Delete();
		this->type = Int;
		this->i = val;
	}

	inline void Variable::operator=(uint val)
	{
		this->Delete();
		this->type = UInt;
		this->ui = val;
	}

	inline void Variable::operator=(float val)
	{
		this->Delete();
		this->type = Float;
		this->f[0] = val;
	}

	inline void Variable::operator=(bool val)
	{
		this->Delete();
		this->type = Bool;
		this->b = val;
	}

	inline void	Variable::operator=(const Math::Vector2& val)
	{
		this->Delete();
		this->type = Vector2;
		this->f[0] = val.x();
		this->f[1] = val.y();
	}

	inline void	Variable::operator=(const Math::Vector3& val)
	{
		this->Delete();
		this->type = Vector3;
		this->f[0] = val.x();
		this->f[1] = val.y();
		this->f[2] = val.z();		
	}

	inline void	Variable::operator=(const Math::Vector4& val)
	{
		this->Delete();
		this->type = Vector4;
		this->f[0] = val.x();
		this->f[1] = val.y();
		this->f[2] = val.z();
		this->f[3] = val.w();
	}

	inline void	Variable::operator=(const Util::String& s)
	{
		if (String == this->type)
		{
			*this->string = s;
		}
		else
		{
			this->Delete();
			this->string = new Util::String(s);
		}
		this->type = String;
	}

	inline void	Variable::operator=(const Math::Matrix4& val)
	{
		if (Matrix4 == this->type)
		{
			*this->m = val;
		}
		else
		{
			this->Delete();
			this->m = new Math::Matrix4(val);
		}
		this->type = Matrix4;
	}

	inline void Variable::operator=(void* ptr)
	{
		this->Delete();
		this->type = VoidPtr;
		this->voidPtr = ptr;
	}

	inline void	Variable::operator=(const Util::Array<int>& val)
	{
		if (IntArray == this->type)
		{
			*this->intArray = val;
		}
		else
		{
			this->Delete();
			this->intArray = new Util::Array<int>(val);
		}
		this->type = IntArray;
	}

	inline void	Variable::operator=(const Util::Array<float>& val)
	{
		if (FloatArray == this->type)
		{
			*this->floatArray = val;
		}
		else
		{
			this->Delete();
			this->floatArray = new Util::Array<float>(val);
		}
		this->type = FloatArray;
	}

	inline void	Variable::operator=(const Util::Array<bool>& val)
	{
		if (BoolArray == this->type)
		{
			*this->boolArray = val;
		}
		else
		{
			this->Delete();
			this->boolArray = new Util::Array<bool>(val);
		}
		this->type = BoolArray;
	}

	inline void	Variable::operator=(const Util::Array<Math::Vector2>& rhs)
	{
		if (Vector2Array == this->type)
		{
			*this->vector2Array = rhs;
		}
		else
		{
			this->Delete();
			this->vector2Array = new Util::Array<Math::Vector2>(rhs);
		}
		this->type = Vector2Array;
	}

	inline void Variable::operator=(const Util::Array<Math::Vector3>& val)
	{
		if (Vector3Array == this->type)
		{
			*this->vector3Array = val;
		}
		else
		{
			this->Delete();
			this->vector3Array = new Util::Array<Math::Vector3>(val);
		}
		this->type = Vector3Array;
	}

	inline void Variable::operator=(const Util::Array<Math::Vector4>& val)
	{
		if (Vector4Array == this->type)
		{
			*this->vector4Array = val;
		}
		else
		{
			this->Delete();
			this->vector4Array = new Util::Array<Math::Vector4>(val);
		}
		this->type = Vector4Array;
	}

	inline void	Variable::operator=(const Util::Array<Math::Matrix4>& val)
	{
		if (Matrix4Array == this->type)
		{
			*this->matrix4Array = val;
		}
		else
		{
			this->Delete();
			this->matrix4Array = new Util::Array<Math::Matrix4>(val);
		}
		this->type = Matrix4Array;
	}

	inline void	Variable::operator=(const Util::Array<Util::String>& val)
	{
		if (StringArray == this->type)
		{
			*this->stringArray = val;
		}
		else
		{
			this->Delete();
			this->stringArray = new Util::Array<Util::String>(val);
		}
		this->type = StringArray;
	}

	inline bool	Variable::operator==(const Variable& rhs) const
	{
		if (rhs.type == this->type)
		{
			switch (rhs.type)
			{
			case Void:
				return true;
			case Int:
				return (this->i == rhs.i);
			case UInt:
				return (this->ui == rhs.ui);
			case Bool:
				return (this->b == rhs.b);
			case Float:
				return (this->f[0] == rhs.f[0]);
			case String:
				return ((*this->string) == (*rhs.string));
			case Vector2:
				return ((this->f[0] == rhs.f[0]) && (this->f[1] == rhs.f[1]));
			case Vector3:
				return ((this->f[0] == rhs.f[0]) && (this->f[1] == rhs.f[1]) && (this->f[2] == rhs.f[2]));
			case Vector4:
				return ((this->f[0] == rhs.f[0]) &&
					(this->f[1] == rhs.f[1]) &&
					(this->f[2] == rhs.f[2]) &&
					(this->f[3] == rhs.f[3]));
			case VoidPtr:
				return (this->voidPtr == rhs.voidPtr);
			case Matrix4:
				return ((*this->m) == (*rhs.m));
			default:
				//invalid variant type!
				assert(false);

				return false;
			}
		}
		return false;
	}

	inline bool Variable::operator>(const Variable& rhs) const
	{
		if (rhs.type == this->type)
		{
			switch (rhs.type)
			{
			case Void:
				return true;
			case Int:
				return (this->i > rhs.i);
			case UInt:
				return (this->ui > rhs.ui);
			case Bool:
				return (this->b > rhs.b);
			case Float:
				return (this->f[0] > rhs.f[0]);
			case String:
				return ((*this->string) > (*rhs.string));
			case Vector2:
				return ((this->f[0] > rhs.f[0]) && (this->f[1] > rhs.f[1]));
			case Vector3:
				return ((this->f[0] > rhs.f[0]) && (this->f[1] > rhs.f[1]) && (this->f[2] > rhs.f[2]));
			case Vector4:
				return ((this->f[0] > rhs.f[0]) &&
					(this->f[1] > rhs.f[1]) &&
					(this->f[2] > rhs.f[2]) &&
					(this->f[3] > rhs.f[3]));
			case VoidPtr:
				return (this->voidPtr > rhs.voidPtr);
			default:
				assert(false);
				return false;
			}
		}
		return false;
	}

	inline bool Variable::operator<(const Variable& rhs) const
	{
		if (rhs.type == this->type)
		{
			switch (rhs.type)
			{
			case Void:
				return true;
			case Int:
				return (this->i < rhs.i);
			case UInt:
				return (this->ui < rhs.ui);
			case Bool:
				return (this->b < rhs.b);
			case Float:
				return (this->f[0] < rhs.f[0]);
			case String:
				return ((*this->string) < (*rhs.string));
			case Vector2:
				return ((this->f[0] < rhs.f[0]) && (this->f[1] < rhs.f[1]));
			case Vector3:
				return ((this->f[0] < rhs.f[0]) && (this->f[1] < rhs.f[1]) && (this->f[2] < rhs.f[2]));
			case Vector4:
				return ((this->f[0] < rhs.f[0]) &&
					(this->f[1] < rhs.f[1]) &&
					(this->f[2] < rhs.f[2]) &&
					(this->f[3] < rhs.f[3]));
			case VoidPtr:
				return (this->voidPtr < rhs.voidPtr);
			default:
				assert(false);
				return false;
			}
		}
		return false;
	}

	inline bool Variable::operator>=(const Variable& rhs) const
	{
		if (rhs.type == this->type)
		{
			switch (rhs.type)
			{
			case Void:
				return true;
			case Int:
				return (this->i >= rhs.i);
			case UInt:
				return (this->ui >= rhs.ui);
			case Bool:
				return (this->b >= rhs.b);
			case Float:
				return (this->f[0] >= rhs.f[0]);
			case String:
				return ((*this->string) >= (*rhs.string));
			case Vector2:
				return ((this->f[0] >= rhs.f[0]) && (this->f[1] >= rhs.f[1]));
			case Vector3:
				return ((this->f[0] >= rhs.f[0]) && (this->f[1] >= rhs.f[1]) && (this->f[2] >= rhs.f[2]));
			case Vector4:
				return ((this->f[0] >= rhs.f[0]) &&
					(this->f[1] >= rhs.f[1]) &&
					(this->f[2] >= rhs.f[2]) &&
					(this->f[3] >= rhs.f[3]));
			case VoidPtr:
				return (this->voidPtr >= rhs.voidPtr);
			default:
				assert(false);
				return false;
			}
		}
		return false;
	}

	inline bool	Variable::operator<=(const Variable& rhs) const
	{
		if (rhs.type == this->type)
		{
			switch (rhs.type)
			{
			case Void:
				return true;
			case Int:
				return (this->i <= rhs.i);
			case UInt:
				return (this->ui <= rhs.ui);
			case Bool:
				return (this->b <= rhs.b);
			case Float:
				return (this->f[0] <= rhs.f[0]);
			case String:
				return ((*this->string) <= (*rhs.string));
			case Vector2:
				return ((this->f[0] <= rhs.f[0]) && (this->f[1] <= rhs.f[1]));
			case Vector3:
				return ((this->f[0] <= rhs.f[0]) && (this->f[1] <= rhs.f[1]) && (this->f[2] <= rhs.f[2]));
			case Vector4:
				return ((this->f[0] <= rhs.f[0]) &&
					(this->f[1] <= rhs.f[1]) &&
					(this->f[2] <= rhs.f[2]) &&
					(this->f[3] <= rhs.f[3]));
			case VoidPtr:
				return (this->voidPtr <= rhs.voidPtr);
			default:
				assert(false);
				return false;
			}
		}
		return false;
	}

	inline bool	Variable::operator!=(const Variable& rhs) const
	{
		return !(*this == rhs);
	}

	inline bool	Variable::operator==(int rhs) const
	{
		assert(Int == this->type);
		return (this->i == rhs);
	}

	inline bool	Variable::operator==(uint rhs) const
	{
		assert(UInt == this->type);
		return (this->ui == rhs);
	}
	
	inline bool	Variable::operator==(float rhs) const
	{
		assert(Float == this->type);
		return (this->f[0] == rhs);
	}

	inline bool	Variable::operator==(bool rhs) const
	{
		assert(Bool == this->type);
		return (this->b == rhs);
	}

	inline bool	Variable::operator==(const Util::String& rhs) const
	{
		assert(String == this->type);
		return ((*this->string) == rhs);
	}

	inline bool	Variable::operator==(const Math::Vector2& rhs) const
	{
		assert(Vector2 == this->type);
		return (this->f[0] == rhs.x() && this->f[1] == rhs.y());
	}

	inline bool	Variable::operator==(const Math::Vector3& rhs) const
	{
		assert(Vector3 == this->type);
		return (this->f[0] == rhs.x() && this->f[1] == rhs.y() && this->f[2] == rhs.z());
	}

	inline bool	Variable::operator==(const Math::Vector4& rhs) const
	{
		assert(Vector4 == this->type);
		return ((this->f[0] == rhs.x()) &&
			(this->f[1] == rhs.y()) &&
			(this->f[2] == rhs.z()) &&
			(this->f[3] == rhs.w()));
	}

	inline bool	Variable::operator==(void* ptr) const
	{
		assert(VoidPtr == this->type);
		return this->voidPtr == ptr;
	}

	inline bool	Variable::operator!=(int rhs) const
	{
		assert(Int == this->type);
		return (this->i != rhs);
	}

	inline bool	Variable::operator!=(uint rhs) const
	{
		assert(UInt == this->type);
		return (this->ui != rhs);
	}

	inline bool	Variable::operator!=(float rhs) const
	{
		assert(Float == this->type);
		return (this->f[0] != rhs);
	}

	inline bool	Variable::operator!=(bool rhs) const
	{
		assert(Bool == this->type);
		return (this->b != rhs);
	}

	inline bool	Variable::operator!=(const Util::String& rhs) const
	{
		assert(String == this->type);
		return (*this->string) != rhs;
	}

	inline bool Variable::operator!=(const Math::Vector2& rhs) const
	{
		assert(Vector2 == this->type);
		return (this->f[0] != rhs.x() || this->f[1] != rhs.y());
	}

	inline bool Variable::operator!=(const Math::Vector3& rhs) const
	{
		assert(Vector3 == this->type);
		return (this->f[0] != rhs.x() || this->f[1] != rhs.y() || this->f[2] != rhs.z());
	}

	inline bool	Variable::operator!=(const Math::Vector4& rhs) const
	{
		assert(Vector4 == this->type);
		return ((this->f[0] != rhs.x()) ||
			(this->f[1] != rhs.y()) ||
			(this->f[2] != rhs.z()) ||
			(this->f[3] != rhs.w()));
	}
	
	inline bool	Variable::operator!=(void* ptr) const
	{
		assert(VoidPtr == this->type);
		return (this->voidPtr == ptr);
	}
}