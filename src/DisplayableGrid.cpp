#include "DisplayableGrid.h"

#include "GridGeometry.h"

const osg::Vec4 DisplayableGrid::LINE_COLOR = osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f);


DisplayableGrid::DisplayableGrid(const Roi3DF & area, osg::Vec3 barDistance) :
	m_gridGeometry(new GridGeometry(area, barDistance, LINE_COLOR, false))
{
	// empty
}


osg::ref_ptr<osg::Group> DisplayableGrid::getGeometry() const
{
	return m_gridGeometry;
}