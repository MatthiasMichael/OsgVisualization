#pragma once

#include "OsgDisplayable.h"
#include "GridGeometry.h"

class DisplayableGrid : public OsgDisplayable
{
public:
	DisplayableGrid(const Roi3DF & area, osg::Vec3 barDistance);

	osg::ref_ptr<osg::Group> getGeometry() const override;

private:
	static const osg::Vec4 LINE_COLOR;

	osg::ref_ptr<GridGeometry> m_gridGeometry;
};