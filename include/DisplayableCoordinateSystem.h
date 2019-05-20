#pragma once

#include "OsgDisplayable.h"

class DisplayableCoordinateSystem : public OsgDisplayable
{
public:
	DisplayableCoordinateSystem(const osg::Vec3 & position, float size, float beamLength);

	osg::ref_ptr<osg::Group> getGeometry() const override;

private:
	osg::Vec3 m_position;

	float m_size;
	float m_beamLength;
};