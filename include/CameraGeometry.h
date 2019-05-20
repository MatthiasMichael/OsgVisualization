#pragma once

#include <osg/Group>


class CameraGeometry : public osg::Group
{
public:
	using Vec3 = osg::Vec3;
	using Vec4 = osg::Vec4;

	struct Info
	{
		Vec3 origin;
		Vec3 dir_topLeft;
		Vec3 dir_topRight;
		Vec3 dir_bottomLeft;
		Vec3 dir_bottomRight;
	};

	CameraGeometry(const Info & info, const float radius, const float scaling, const Vec4 & color);

private:
	void createGeometry();

	static osg::ref_ptr<osg::Geometry> createViewRayGeometry(const Vec3 & origin, const Vec3 & direction, float scaling, Vec3 * pEndPoint = nullptr);

private:
	Info m_info;

	float m_radius;
	float m_scaling;
	osg::Vec4 m_color;
};
