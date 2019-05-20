#pragma once

#include <vector>

#include <osg/Geode>
#include <osg/Group>
#include <osg/Material>

class WallsGeometry : public osg::Group
{
public:
	using Polygon = std::vector<osg::Vec3>;
	using PolygonVector = std::vector<Polygon>;

	WallsGeometry();
	WallsGeometry(const PolygonVector & verticeVec, bool offset = true);

	void init(const PolygonVector & verticeVec, bool offset = true);

	void createGeometry();
	void setOffset(bool offset);

	void setMaterial(osg::ref_ptr<osg::Material> material, bool lighting);
	void setColor(osg::Vec4 color) const;
	const PolygonVector & getVertices() const { return m_verticeVector; }

private:
	static const float WALLS_OFFSET_VALUE;

	PolygonVector m_verticeVector;

	bool m_useWallOffset;

	osg::ref_ptr<osg::Geode> m_wallsGeode;
};
