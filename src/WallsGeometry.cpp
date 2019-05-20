#include "WallsGeometry.h"

#include <osg/Geode>

#include "osg_utils.h"


osg::ref_ptr<osg::Geometry> createVertice(const osg::Vec3 & a, const osg::Vec3 & b, const osg::Vec3 & c, const osg::Vec4 & color)
{
	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array();
	points->push_back(a);
	points->push_back(b);
	points->push_back(c);

	osg::ref_ptr<osg::DrawElementsUInt> triangleFace = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	triangleFace->push_back(0);
	triangleFace->push_back(1);
	triangleFace->push_back(2);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(color);
	//colors->push_back(color);
	//colors->push_back(color);

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	osg::ref_ptr<osg::Geometry> triangle = new osg::Geometry();
	triangle->setVertexArray(points.get());
	triangle->addPrimitiveSet(triangleFace);
	triangle->setColorArray(colors.get());
	triangle->setColorBinding(osg::Geometry::BIND_OVERALL);
	triangle->setNormalArray(normals.get());
	triangle->setNormalBinding(osg::Geometry::BIND_OVERALL);

	return triangle;
}


osg::ref_ptr<osg::Geometry> createVerticeWithOffset(const osg::Vec3 & a, const osg::Vec3 & b, const osg::Vec3 & c, const osg::Vec4 & color, const float aOffset, const float bOffset, const float cOffset)
{
	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array();
	points->push_back(a + osg::Vec3(aOffset, aOffset, aOffset));
	points->push_back(b + osg::Vec3(bOffset, bOffset, bOffset));
	points->push_back(c + osg::Vec3(cOffset, cOffset, cOffset));

	osg::DrawElementsUInt * triangleFace = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	triangleFace->push_back(0);
	triangleFace->push_back(1);
	triangleFace->push_back(2);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(color);
	//colors->push_back(color);
	//colors->push_back(color);

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array();
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	osg::ref_ptr<osg::Geometry> triangle = new osg::Geometry();
	triangle->setVertexArray(points.get());
	triangle->addPrimitiveSet(triangleFace);
	triangle->setColorArray(colors.get());
	triangle->setColorBinding(osg::Geometry::BIND_OVERALL);
	triangle->setNormalArray(normals.get());
	triangle->setNormalBinding(osg::Geometry::BIND_OVERALL);

	return triangle;
}


const float WallsGeometry::WALLS_OFFSET_VALUE = 0.1f;


WallsGeometry::WallsGeometry(): m_useWallOffset(false) {}


WallsGeometry::WallsGeometry(const PolygonVector & verticeVec, bool offset /*= true*/) :
	m_verticeVector(verticeVec),
	m_useWallOffset(offset)
{
	// empty
}


void WallsGeometry::createGeometry()
{
	this->removeChild(m_wallsGeode);
	m_wallsGeode.release();

	m_wallsGeode = new osg::Geode();
	this->addChild(m_wallsGeode);

	for (int i = 0; i < m_verticeVector.size(); i++)
	{
		const Polygon & currVertex = m_verticeVector[i];

		if (currVertex.size() < 3)
			continue;

		const osg::Vec3 & a = currVertex[0];
		const osg::Vec3 & b = currVertex[1];
		const osg::Vec3 & c = currVertex[2];

		if (!m_useWallOffset) // normal
		{
			osg::ref_ptr<osg::Geometry> osgVert = createVertice(a, b, c, osg::Vec4(1, 0, 0, 1));
			m_wallsGeode->addDrawable(osgVert);
		}
		else
		{
			const float offset = WALLS_OFFSET_VALUE;

			// A B C + offset
			osg::ref_ptr<osg::Geometry> osgVertABC = createVerticeWithOffset(a, b, c, osg::Vec4(1, 0, 0, 1), offset, offset, offset);
			m_wallsGeode->addDrawable(osgVertABC);
			// A B C - offset
			osg::ref_ptr<osg::Geometry> osgVertABC2 = createVerticeWithOffset(a, b, c, osg::Vec4(1, 0, 0, 1), -offset, -offset, -offset);
			m_wallsGeode->addDrawable(osgVertABC2);
			// B B' C
			osg::ref_ptr<osg::Geometry> osgVertBBC = createVerticeWithOffset(b, b, c, osg::Vec4(1, 0, 0, 1), -offset, offset, -offset);
			m_wallsGeode->addDrawable(osgVertBBC);
			// B' B C'
			osg::ref_ptr<osg::Geometry> osgVertBBC2 = createVerticeWithOffset(b, b, c, osg::Vec4(1, 0, 0, 1), offset, -offset, offset);
			m_wallsGeode->addDrawable(osgVertBBC2);
			// A A' B
			osg::ref_ptr<osg::Geometry> osgVertAAB = createVerticeWithOffset(a, a, b, osg::Vec4(1, 0, 0, 1), -offset, offset, -offset);
			m_wallsGeode->addDrawable(osgVertAAB);
			// A' A B'
			osg::ref_ptr<osg::Geometry> osgVertAAB2 = createVerticeWithOffset(a, a, b, osg::Vec4(1, 0, 0, 1), offset, -offset, offset);
			m_wallsGeode->addDrawable(osgVertAAB2);
			// A A' C
			osg::ref_ptr<osg::Geometry> osgVertAAC = createVerticeWithOffset(a, a, c, osg::Vec4(1, 0, 0, 1), -offset, offset, -offset);
			m_wallsGeode->addDrawable(osgVertAAC);
			// A' A C'
			osg::ref_ptr<osg::Geometry> osgVertAAC2 = createVerticeWithOffset(a, a, c, osg::Vec4(1, 0, 0, 1), offset, -offset, offset);
			m_wallsGeode->addDrawable(osgVertAAC2);
		}
	}
}


void WallsGeometry::setOffset(bool offset)
{
	m_useWallOffset = offset;
}


void WallsGeometry::init(const PolygonVector & verticeVec, bool offset /*= true*/)
{
	m_verticeVector = verticeVec;
	m_useWallOffset = offset;
}


void WallsGeometry::setMaterial(osg::ref_ptr<osg::Material> material, bool lighting)
{
	if (lighting)
		getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	else
		getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	m_wallsGeode->getOrCreateStateSet()->setAttribute(material);
}


void WallsGeometry::setColor(osg::Vec4 color) const
{
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	for (int i = 0; i < 3; ++i)
	{
		colors->push_back(color);
	}

	for (unsigned int i = 0; i < m_wallsGeode->getNumChildren(); ++i)
	{
		osg::Geometry * g = dynamic_cast<osg::Geometry*>(m_wallsGeode->getChild(i));
		g->setColorArray(colors.get());
	}
}
