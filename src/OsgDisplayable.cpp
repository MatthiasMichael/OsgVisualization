#include "OsgDisplayable.h"


OsgDisplayable::OsgDisplayable(bool shouldBeDisplayed /*= true*/) : m_shouldBeDisplayed(shouldBeDisplayed)
{
	// empty
}


osg::ref_ptr<osg::Group> EmptyDisplayable::getGeometry() const
{
	return new osg::Group();
}