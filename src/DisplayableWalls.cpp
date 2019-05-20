#include "DisplayableWalls.h"

osg::ref_ptr<osg::Group> DisplayableWalls::getGeometry() const
{
	if(m_walls.valid())
	{
		m_walls->setOffset(false);
		m_walls->createGeometry();

		osg::ref_ptr<osg::Material> material = new osg::Material();
		material->setDiffuse(osg::Material::FRONT,  osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		material->setSpecular(osg::Material::FRONT, osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));
		material->setAmbient(osg::Material::FRONT,  osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		material->setEmission(osg::Material::FRONT, osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		material->setShininess(osg::Material::FRONT, 25.0);

		m_walls->setColor(osg::Vec4(0.5, 0.5, 0.5, 0.5));
		m_walls->setMaterial(material, true);		
	}

	return m_walls;
}