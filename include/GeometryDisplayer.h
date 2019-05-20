#pragma once

#include <osg/Node>

#include <osg/ref_ptr>


class GeometryDisplayer
{
public:
	virtual void updateDisplay() = 0;

	virtual bool addNodeToScene(osg::ref_ptr<osg::Node> node) = 0;
	virtual bool removeNodeFromScene(osg::ref_ptr<osg::Node> node) = 0;
};