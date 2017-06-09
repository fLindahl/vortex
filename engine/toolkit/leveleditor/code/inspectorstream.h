#pragma once
#include "application/game/baseproperty.h"


namespace Toolkit
{
class InspectorStream
{
public:
    InspectorStream();
    ~InspectorStream();

    void operator<<(const Attribute<int>& attr);

private:

};

inline InspectorStream::operator<<(const Attribute<int>& attr)
{
    
}

}