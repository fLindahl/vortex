#include "config.h"
#include "baseinspector.h"

namespace LevelEditor
{
	BaseInspector::BaseInspector()
	{
	}

	BaseInspector::~BaseInspector()
	{
	}

	void BaseInspector::Update()
	{

	}

	bool BaseInspector::IsModified() const
	{
		return isModified;
	}
}