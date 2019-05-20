#include "DisplayableAreaLights.h"

#include <algorithm>

#include <osg/Light>
#include <osg/LightSource>


DisplayableAreaLight::DisplayableAreaLight(Roi3DF area): m_area(area)
{
	// empty
}


osg::ref_ptr<osg::Group> DisplayableAreaLight::getGeometry() const
{
	const osg::Vec3 center = m_area.center<osg::Vec3>();
	const osg::Vec4 lightPos(center[0], center[1], std::max(m_area.z1, m_area.z2), 1.);

	const double dx = m_area.x2 - m_area.x1;
	const double dy = m_area.y2 - m_area.y1;
	const double modelSizeSquared = dx * dx + dy * dy;

	osg::Light* myLight = new osg::Light;
	myLight->setLightNum(0);
	myLight->setPosition(lightPos);
	myLight->setDiffuse(osg::Vec4(0.8f, 0.8f, 1.0f, 1.0));
	myLight->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	myLight->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	myLight->setAmbient(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	myLight->setConstantAttenuation(1.0f);
	myLight->setLinearAttenuation(2.0f / sqrt(modelSizeSquared));
	myLight->setQuadraticAttenuation(2.0f / modelSizeSquared);

	osg::LightSource* lightSource = new osg::LightSource;
	lightSource->setLight(myLight);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);

	osg::Group* lightGroup = new osg::Group;
	lightGroup->addChild(lightSource);

	return lightGroup;
}
