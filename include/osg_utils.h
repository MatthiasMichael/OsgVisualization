#pragma once

#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Vec3i>

osg::Vec3i i_from_f_roundDown(const osg::Vec3f & v);
osg::Vec3i i_from_f_roundUp(const osg::Vec3f & v);

void setAttributesNonLightingBlendable(osg::Node * node);

osg::ref_ptr<osg::Geometry> createBeam(const osg::Vec3 & start, const osg::Vec3 & end, const osg::Vec4 & color);

std::vector<osg::ref_ptr<osg::Light>> createLights(osg::Vec4 color, const osg::Vec3 & start, const osg::Vec3 & end);
