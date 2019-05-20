#pragma once

#include <osg/Group>
#include <osg/Vec4>

#include "Roi3DF_osg.h"

class GridGeometry : public osg::Group
{
public:
	GridGeometry(const Roi3DF & area, const osg::Vec3 & dist, const osg::Vec4 color, const bool fill = true);
	GridGeometry(const osg::Vec3 & start, const osg::Vec3 & end, const osg::Vec3 & dist, const osg::Vec4 & color, const bool fill = true);

private:
	void createGeometry();

private:
	const Roi3DF m_area;

	const osg::Vec3 m_dist;
	const osg::Vec4 m_color;

	const bool m_fill;
};
