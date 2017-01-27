#include "config.h"
#include "bbox.h"

namespace Math
{

point bbox::corner_point(const int& index) const
{
	assert((index >= 0) && (index < 8));
	switch (index)
	{
	case 0:     return this->minPoint;
	case 1:     return point(this->minPoint.x(), this->maxPoint.y(), this->minPoint.z());
	case 2:     return point(this->maxPoint.x(), this->maxPoint.y(), this->minPoint.z());
	case 3:     return point(this->maxPoint.x(), this->minPoint.y(), this->minPoint.z());
	case 4:     return this->maxPoint;
	case 5:     return point(this->minPoint.x(), this->maxPoint.y(), this->maxPoint.z());
	case 6:     return point(this->minPoint.x(), this->minPoint.y(), this->maxPoint.z());
	default:    return point(this->maxPoint.x(), this->minPoint.y(), this->maxPoint.z());
	}
}

}