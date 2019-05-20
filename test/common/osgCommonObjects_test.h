#pragma once

#include <osg/LightSource>

#include "osg_utils.h"

#include "Roi3DF_osg.h"


inline std::vector<std::vector<osg::Vec3>> buildWalls()
{
	return
	{
		{
			osg::Vec3(0, 0, 0),
			osg::Vec3(10, 0, 0),
			osg::Vec3(0, 0, 10)
		},
		{
			osg::Vec3(10, 0, 0),
			osg::Vec3(10, 0, 10),
			osg::Vec3(0, 0, 10)
		},
		{
			osg::Vec3(0, 0, 0),
			osg::Vec3(0, 10, 0),
			osg::Vec3(0, 0, 5)
		},
		{
			osg::Vec3(0, 10, 0),
			osg::Vec3(0, 10, 5),
			osg::Vec3(0, 0, 5)
		},
		{
			osg::Vec3(0, 10, 0),
			osg::Vec3(10, 10, 0),
			osg::Vec3(0, 10, 5)
		}
	};
}

inline std::vector<std::vector<osg::Vec3>> buildFlip1()
{
	return
	{
		{
			osg::Vec3(4, 4, 0),
			osg::Vec3(6, 6, 0),
			osg::Vec3(4, 4, 10)
		},
		{
			osg::Vec3(6, 6, 0),
			osg::Vec3(6, 6, 10),
			osg::Vec3(4, 4, 10)
		}
	};
}

inline std::vector<std::vector<osg::Vec3>> buildFlip2()
{
	return
	{
		{
			osg::Vec3(4, 6, 0),
			osg::Vec3(6, 4, 0),
			osg::Vec3(4, 6, 10)
		},
		{
			osg::Vec3(6, 4, 0),
			osg::Vec3(6, 4, 10),
			osg::Vec3(4, 6, 10)
		}
	};
}

inline osg::ref_ptr<osg::Group> createLightsFromUsableArea(const Roi3DF & usableArea)
{
	const osg::Vec3 center = usableArea.center<osg::Vec3>();
	const osg::Vec4 lightPos(center[0], center[1], center[2], 1.);

	const double dx = usableArea.x2 - usableArea.x1;
	const double dy = usableArea.y2 - usableArea.y1;
	const double modelSizeSquared = dx * dx + dy * dy;

	osg::Light * myLight = new osg::Light;
	myLight->setLightNum(0);
	myLight->setPosition(lightPos);
	myLight->setDiffuse(osg::Vec4(0.8f, 0.8f, 1.0f, 1.0));
	myLight->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	myLight->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	myLight->setAmbient(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	myLight->setConstantAttenuation(1.0f);
	myLight->setLinearAttenuation(2.0f / sqrt(modelSizeSquared));
	myLight->setQuadraticAttenuation(2.0f / modelSizeSquared);

	osg::ref_ptr<osg::LightSource> lightSource(new osg::LightSource);
	lightSource->setLight(myLight);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);

	osg::ref_ptr<osg::Group> lightGroup(new osg::Group);
	lightGroup->addChild(lightSource);

	return lightGroup;
}