#include "CameraGeometry.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LightSource>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Vec3>

#include "osg_utils.h"


CameraGeometry::CameraGeometry(const Info & info, const float radius, const float scaling, const Vec4 & color) :
	m_info(info),
	m_radius(radius),
	m_scaling(scaling),
	m_color(color)
{
	createGeometry();
}


void CameraGeometry::createGeometry()
{
	osg::ref_ptr<osg::Geode> camGeode = new osg::Geode();
	this->addChild(camGeode);

	osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(m_info.origin, m_radius);
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(sphere);

	sd->setColor(m_color);
	camGeode->addDrawable(sd);

	Vec3 end_topLeft, end_topRight, end_bottomLeft, end_bottomRight;

	camGeode->addChild(createViewRayGeometry(m_info.origin, m_info.dir_topLeft, m_scaling, &end_topLeft));
	camGeode->addChild(createViewRayGeometry(m_info.origin, m_info.dir_topRight, m_scaling, &end_topRight));
	camGeode->addChild(createViewRayGeometry(m_info.origin, m_info.dir_bottomLeft, m_scaling, &end_bottomLeft));
	camGeode->addChild(createViewRayGeometry(m_info.origin, m_info.dir_bottomRight, m_scaling, &end_bottomRight));

	camGeode->addChild(createBeam(end_topLeft, end_bottomLeft, Vec4(0, 1, 0, 1))); //left -> grün
	camGeode->addChild(createBeam(end_bottomLeft, end_bottomRight, Vec4(0, 1, 1, 1))); //bottom -> yellow
	camGeode->addChild(createBeam(end_bottomRight, end_topRight, Vec4(1, 0, 0, 1))); //right ->red
	camGeode->addChild(createBeam(end_topRight, end_topLeft, Vec4(1, 1, 1, 1))); //top -> white

	osg::ref_ptr<osg::ShapeDrawable> sd_topLeft = new osg::ShapeDrawable(osg::ref_ptr<osg::Sphere>(new osg::Sphere(end_topLeft, m_radius / 2)));
	osg::ref_ptr<osg::ShapeDrawable> sd_topRight = new osg::ShapeDrawable(osg::ref_ptr<osg::Sphere>(new osg::Sphere(end_topRight, m_radius / 2)));
	osg::ref_ptr<osg::ShapeDrawable> sd_bottomLeft = new osg::ShapeDrawable(osg::ref_ptr<osg::Sphere>(new osg::Sphere(end_bottomLeft, m_radius / 2)));
	osg::ref_ptr<osg::ShapeDrawable> sd_bottomRight = new osg::ShapeDrawable(osg::ref_ptr<osg::Sphere>(new osg::Sphere(end_bottomRight, m_radius / 2)));
	
	sd_topLeft->setColor(osg::Vec4(1, 0, 0, 1));
	sd_topRight->setColor(osg::Vec4(0, 1, 0, 1));
	sd_bottomLeft->setColor(osg::Vec4(0, 0, 1, 1));
	sd_bottomRight->setColor(osg::Vec4(1, 1, 1, 1));

	camGeode->addDrawable(sd_topLeft);
	camGeode->addDrawable(sd_topRight);
	camGeode->addDrawable(sd_bottomLeft);
	camGeode->addDrawable(sd_bottomRight);

	/*const osg::Vec3 x(100, 0, 0);
	const osg::Vec3 y(0, 100, 0);
	const osg::Vec3 z(0, 0, 100);

	Vector3f rx, ry, rz;
	rx << 50 , 0 , 0;
	ry << 0 , 50 , 0;
	rz << 0 , 0 , 50;

	rx = m_model.m_camRot.cast<float>() * rx;
	ry = m_model.m_camRot.cast<float>() * ry;
	rz = m_model.m_camRot.cast<float>() * rz;

	const osg::Vec3 vrx(rx(0), rx(1), rx(2));
	const osg::Vec3 vry(ry(0), ry(1), ry(2));
	const osg::Vec3 vrz(rz(0), rz(1), rz(2));

	osg::ref_ptr<osg::Sphere> sphereX = new osg::Sphere(center + vrx, 5);
	osg::ref_ptr<osg::ShapeDrawable> sdX = new osg::ShapeDrawable(sphereX);
	sdX->setColor(osg::Vec4(1, 1, 0, 1));
	camGeode->addDrawable(sdX);

	osg::ref_ptr<osg::Sphere> sphereY = new osg::Sphere(center + vry, 5);
	osg::ref_ptr<osg::ShapeDrawable> sdY = new osg::ShapeDrawable(sphereY);
	sdY->setColor(osg::Vec4(0, 1, 0, 1));
	camGeode->addDrawable(sdY);

	osg::ref_ptr<osg::Sphere> sphereZ = new osg::Sphere(center + vrz, 5);
	osg::ref_ptr<osg::ShapeDrawable> sdZ = new osg::ShapeDrawable(sphereZ);
	sdZ->setColor(osg::Vec4(0, 1, 1, 1));
	camGeode->addDrawable(sdZ);

	camGeode->addChild(createBeam(center, center + x, osg::Vec4(1, 1, 0, 1)));
	camGeode->addChild(createBeam(center, center + y, osg::Vec4(0, 1, 0, 1)));
	camGeode->addChild(createBeam(center, center + z, osg::Vec4(0, 1, 1, 1)));

	camGeode->addChild(createBeam(center, center + vrx, osg::Vec4(1, 1, 0, 1)));
	camGeode->addChild(createBeam(center, center + vry, osg::Vec4(0, 1, 0, 1)));
	camGeode->addChild(createBeam(center, center + vrz, osg::Vec4(0, 1, 1, 1)));*/

	setAttributesNonLightingBlendable(this);
}


osg::ref_ptr<osg::Geometry> CameraGeometry::createViewRayGeometry(const Vec3 & origin, const Vec3 & direction, float scaling, Vec3 * pEndPoint /*= nullptr*/)
{
	Vec3 beamEnd = origin + direction * scaling;
	
	osg::Vec4 color(1, 0, 0, 1);

	if (pEndPoint)
	{
		*pEndPoint = beamEnd;
	}

	return createBeam(origin, beamEnd, color);
}