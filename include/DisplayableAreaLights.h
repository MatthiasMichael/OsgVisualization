#pragma once

#include "OsgDisplayable.h"

#include "Roi3DF_osg.h"


class DisplayableAreaLight : public OsgDisplayable
{
public:
	DisplayableAreaLight(Roi3DF area);

	osg::ref_ptr<osg::Group> getGeometry() const override;

private:
	Roi3DF m_area;
};