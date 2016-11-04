#pragma once
#include <memory>
#include "shaderobject.h"
#include "textureresource.h"
#include "foundation/util/array.h"
#include <map>
#include "foundation/util/variable.h"
//#include "foundation/util/string.h"

namespace Render
{
class Surface;

struct MaterialParameter
{
	Util::String name;
	Util::Variable var;
};

class Material
{
public:
	Material();
	~Material();

	// Used for sorting by shaderobject.
	bool operator<(const Material& rhs) const;

    void SetName(const Util::String& name);
	Util::String GetName();

    void SetShader(const Util::String& name);
    std::shared_ptr<ShaderObject> GetShader() const;

    Util::Array<std::shared_ptr<TextureResource>>& TextureList();

	MaterialParameter* GetParameterByName(const Util::String& name);
	void AddParameter(const Util::String& name, const Util::Variable& variable);
	
    Util::Array<std::shared_ptr<Surface>>& SurfaceList();

	void SetFramePass(const Util::String& pass);
	index_t GetFramePass();
	
	Util::Array<MaterialParameter*>& ParameterList() { return this->parameters; }

	//TEMPORARY
	Util::Array<ModelInstance*>& getModelInstances() { return this->modelInstances; }

private:
	friend class ResourceServer;

	//TEMPORARY
	Util::Array<ModelInstance*> modelInstances;

	// name of the material
	Util::String name;

	// loaded shader object
	std::shared_ptr<ShaderObject> shader;

	// What framepass are we rendering this material in?
	// TODO: this should probably be it's own class that we just point to... maybe...
	index_t framePass;

	// loaded textures
	Util::Array<std::shared_ptr<TextureResource>> textures;
	
	std::map<Util::String, MaterialParameter*> parametersByName;
	Util::Array<MaterialParameter*> parameters;

	// all surfaces that currently use this material
	Util::Array<std::shared_ptr<Surface>> surfaces;
};

}