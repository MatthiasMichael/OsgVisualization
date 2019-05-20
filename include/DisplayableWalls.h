#pragma once

#include <osg/ref_ptr>
#include <osg/Geode>

#include "OsgDisplayable.h"

#include "WallsGeometry.h"


class DisplayableWalls : public OsgDisplayable
{
public: 
	DisplayableWalls() : m_walls() { }
	DisplayableWalls(const WallsGeometry & walls) : m_walls(new WallsGeometry(walls)) { }

	osg::ref_ptr<osg::Group> getGeometry() const override;


	osg::ref_ptr<WallsGeometry> m_walls;
};
