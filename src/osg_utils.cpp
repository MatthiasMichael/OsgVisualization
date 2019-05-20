#include "osg_utils.h"

#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/Material>


osg::Vec3i i_from_f_roundDown(const osg::Vec3f & v)
{
	return osg::Vec3i( static_cast<int>(v.x()), static_cast<int>(v.y()), static_cast<int>(v.z()) );
}


osg::Vec3i i_from_f_roundUp(const osg::Vec3f & v)
{
	return osg::Vec3i( static_cast<int>(std::ceil(v.x())), static_cast<int>(std::ceil(v.y())), static_cast<int>(std::ceil(v.z())) );
}


void setAttributesNonLightingBlendable(osg::Node * node)
{
	osg::ref_ptr<osg::StateSet> stateSet = node->getOrCreateStateSet();
	osg::ref_ptr<osg::Material> material( new osg::Material );

	osg::ref_ptr<osg::AlphaFunc> alphaFunc( new osg::AlphaFunc );
	alphaFunc->setFunction(osg::AlphaFunc::GEQUAL,0.05f);

	material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
	material->setAlpha(osg::Material::FRONT_AND_BACK, 0.5);

	stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
	stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	stateSet->setAttributeAndModes(new osg::BlendFunc, osg::StateAttribute::ON );
	stateSet->setAttributeAndModes( alphaFunc, osg::StateAttribute::ON );
}


osg::ref_ptr<osg::Geometry> createBeam(const osg::Vec3 & start, const osg::Vec3 & end, const osg::Vec4 & color)
{
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(color);

	osg::ref_ptr<osg::Geometry> beam = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array();
	points->push_back(start);
	points->push_back(end);

	beam->setVertexArray(points.get());
	beam->setColorArray(colors.get());
	beam->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
	beam->addPrimitiveSet(new osg::DrawArrays(GL_LINES,0,2)); 

	return beam;
}


std::vector<osg::ref_ptr<osg::Light>> createLights(osg::Vec4 color, const osg::Vec3 & start, const osg::Vec3 & end)
{
	std::vector<osg::ref_ptr<osg::Light>> lights;
	std::vector<osg::Vec4> positions;

	const osg::Vec3 center = start + (end - start) / 2;
	const float height = end[2] + 1;

	positions.push_back(osg::Vec4(start[0], center[1] + 1, height, 1.0));
	positions.push_back(osg::Vec4(end[0], center[1] + 1, height, 1.0));
	positions.push_back(osg::Vec4(center[0], start[1] + 1, height, 1.0));
	positions.push_back(osg::Vec4(center[0], end[1] + 1, height, 1.0));

	for (int i = 0; i < 4; ++i)
	{
		osg::ref_ptr<osg::Light> light = new osg::Light();

		light->setLightNum(i);// each light must have a unique number
		light->setPosition(positions[i]);
		light->setDiffuse(color);
		light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
		light->setAmbient(osg::Vec4(1.0, 1.0, 1.0, 1.0));

		lights.push_back(light);
	}

	return lights;
}
