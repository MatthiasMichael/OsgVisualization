#include "DisplayableCameras.h"

DisplayableCameras::DisplayableCameras(const std::vector<CameraGeometry::Info> & cameras, float size /*= 10.f*/, float scaling /*= 50.f*/) :
	m_cameras(cameras),
	m_size(size),
	m_scaling(scaling)
{
	// empty
}


osg::ref_ptr<osg::Group> DisplayableCameras::getGeometry() const
{
	osg::ref_ptr<osg::Group> camGroup = new osg::Group();

	for (const auto & cam : m_cameras)
	{
		osg::ref_ptr<CameraGeometry> g = new CameraGeometry(cam, m_size, m_scaling, osg::Vec4(1, 0, 0, 1));
		camGroup->addChild(g);
	}

	return camGroup;
}
