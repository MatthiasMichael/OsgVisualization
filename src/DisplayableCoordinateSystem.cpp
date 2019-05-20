#include "DisplayableCoordinateSystem.h"
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/LineWidth>

#include "osg_utils.h"
#include <osgText/Text>


DisplayableCoordinateSystem::DisplayableCoordinateSystem(const osg::Vec3 & position, float size, float beamLength) :
	m_position(position),
	m_size(size),
	m_beamLength(beamLength)
{
	// empty
}


osg::ref_ptr<osg::Group> DisplayableCoordinateSystem::getGeometry() const
{
	osg::ref_ptr<osg::Geode> coordinateSystemGroup = new osg::Geode();

	osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(m_position, m_size);
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(sphere);

	sd->setColor(osg::Vec4(1, 0, 0, 1));
	coordinateSystemGroup->addDrawable(sd);

	const float l = m_size * m_beamLength;

	const osg::Vec3 x(l, 0, 0);
	const osg::Vec3 y(0, l, 0);
	const osg::Vec3 z(0, 0, l);

	osg::ref_ptr<osg::Geometry> beam_x = createBeam(m_position, m_position + x, osg::Vec4(1, 0, 0, 1));
	osg::ref_ptr<osg::Geometry> beam_y = createBeam(m_position, m_position + y, osg::Vec4(0, 1, 0, 1));
	osg::ref_ptr<osg::Geometry> beam_z = createBeam(m_position, m_position + z, osg::Vec4(0, 1, 1, 1));

	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth();
	linewidth->setWidth(3.0f);

	beam_x->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
	beam_y->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
	beam_z->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);

	coordinateSystemGroup->addChild(beam_x);
	coordinateSystemGroup->addChild(beam_y);
	coordinateSystemGroup->addChild(beam_z);

	osg::ref_ptr<osgText::Text> text_x = new osgText::Text();
	text_x->setPosition(m_position + x * 1.1);
	text_x->setText("X");
	text_x->setCharacterSize(0.35);
	text_x->setAutoRotateToScreen(true);
	coordinateSystemGroup->addDrawable(text_x);

	osg::ref_ptr<osgText::Text> text_y = new osgText::Text();
	text_y->setPosition(m_position + y * 1.1);
	text_y->setText("Y");
	text_y->setCharacterSize(0.35);
	text_y->setAutoRotateToScreen(true);
	coordinateSystemGroup->addDrawable(text_y);

	osg::ref_ptr<osgText::Text> text_z = new osgText::Text();
	text_z->setPosition(m_position + z * 1.1);
	text_z->setText("Z");
	text_z->setCharacterSize(0.35);
	text_z->setAutoRotateToScreen(true);
	coordinateSystemGroup->addDrawable(text_z);

	setAttributesNonLightingBlendable(coordinateSystemGroup);

	return coordinateSystemGroup;
}
