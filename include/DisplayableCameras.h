#pragma once

#include <vector>

#include "OsgDisplayable.h"

#include "CameraGeometry.h"


class DisplayableCameras : public OsgDisplayable
{
public:
	DisplayableCameras(const std::vector<CameraGeometry::Info> & cameras, float size = 10.f, float scaling = 50.f);
	
	osg::ref_ptr<osg::Group> getGeometry() const override;

private:
	std::vector<CameraGeometry::Info> m_cameras;

	const float m_size;
	const float m_scaling;
};