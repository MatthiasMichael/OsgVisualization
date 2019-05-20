#include "GridGeometry.h"

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

#include "osg_utils.h"


GridGeometry::GridGeometry(const Roi3DF & area, const osg::Vec3 & dist, const osg::Vec4 color, const bool fill /*= true*/) :
	m_area(area), m_dist(dist), m_color(color), m_fill(fill)
{
	createGeometry();
}


GridGeometry::GridGeometry(const osg::Vec3 & start, const osg::Vec3 & end, const osg::Vec3 & dist, const osg::Vec4 & color, const bool fill /*= true*/) :
	GridGeometry(Roi3DF(start, end), dist, color, fill)
{
	// empty
}


void GridGeometry::createGeometry()
{
	osg::ref_ptr<osg::Geode> gridGeode = new osg::Geode();
	this->addChild(gridGeode);

	osg::Vec3 numBars = osg::componentDivide(m_area.size<osg::Vec3>(), m_dist);
	for (int i = 0; i < 3; ++i)
	{
		numBars[i] = std::ceilf(numBars[i]);
	}

	const osg::Vec3 start = m_area.start<osg::Vec3>();
	const osg::Vec3 end = m_area.end<osg::Vec3>();

	const osg::Vec3 endX(end[0], start[1], start[2]);
	const osg::Vec3 endY(start[0], end[1], start[2]);
	const osg::Vec3 endZ(start[0], start[1], end[2]);

	osg::ref_ptr<osg::Geometry> beamX = createBeam(start, endX, m_color);
	osg::ref_ptr<osg::Geometry> beamY = createBeam(start, endY, m_color);
	osg::ref_ptr<osg::Geometry> beamZ = createBeam(start, endZ, m_color);

	gridGeode->addChild(beamX);
	gridGeode->addChild(beamY);
	gridGeode->addChild(beamZ);

	// TODO: Zeichnen mit weniger Code
	for (int y = 0; y < numBars[1]; ++y)
	{
		for (int z = 0; z < numBars[2]; ++z)
		{
			if (!m_fill && y != 0 && z != 0)
				continue;

			osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform();

			this->addChild(transform);
			transform->addChild(beamX);

			osg::Vec3 pos(0, y * m_dist[1], z * m_dist[2]);
			transform->setPosition(pos);
		}
	}

	for (int x = 0; x < numBars[0]; ++x)
	{
		for (int z = 0; z < numBars[2]; ++z)
		{
			if (!m_fill && x != 0 && z != 0)
				continue;

			osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform();

			this->addChild(transform);
			transform->addChild(beamY);

			osg::Vec3 pos(x * m_dist[0], 0, z * m_dist[2]);
			transform->setPosition(pos);
		}
	}

	for (int x = 0; x < numBars[0]; ++x)
	{
		for (int y = 0; y < numBars[1]; ++y)
		{
			if (!m_fill && x != 0 && y != 0)
				continue;

			osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform();

			this->addChild(transform);
			transform->addChild(beamZ);

			osg::Vec3 pos(x * m_dist[0], y * m_dist[1], 0);
			transform->setPosition(pos);
		}

	}

	setAttributesNonLightingBlendable(this);
}