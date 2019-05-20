#include "OsgGeometryManager.h"

#include <cassert>


const unsigned int GeometryManager::KEY_UNASSIGNED = 0;


GeometryManager::GeometryManager(GeometryDisplayer * pDisplayer) : mep_displayer(pDisplayer), m_currentKey(KEY_UNASSIGNED + 1)
{
	// TODO: Let's see if we even need this or if it is better, if the user of the widget sets this
	/*if (pDisplayer)
	{
		osg::StateSet * rootStateSet = pDisplayer->getRoot()->getOrCreateStateSet();
		rootStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
		rootStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	}
	*/
}


unsigned int GeometryManager::switchGeometry(std::unique_ptr<OsgDisplayable> && displayable, unsigned int key /*= KEY_UNASSIGNED*/)
{
	auto it = m_geometries.end();

	if(key != KEY_UNASSIGNED)
	{
		it = m_geometries.find(key);
	}
	
	if(it == m_geometries.end())
	{
		key = m_currentKey++;

		DisplayableWithActiveGeometry newEntry;
		newEntry.displayable = move(displayable);

		const auto inserted = m_geometries.insert(std::make_pair(key, std::move(newEntry)));
		assert(inserted.second == true);
		it = inserted.first;
	}
	else
	{	
		mep_displayer->removeNodeFromScene(it->second.activeGeometry);	
		it->second.displayable = move(displayable);
	}

	if(it->second.displayable->shouldBeDisplayed())
	{
		it->second.activeGeometry = it->second.displayable->getGeometry();
		mep_displayer->addNodeToScene(it->second.activeGeometry);
	}

	return key;
}


void GeometryManager::updateGeometry(unsigned key /*= KEY_UNASSIGNED*/)
{
	if (key == KEY_UNASSIGNED)
	{
		return;
	}

	auto it = m_geometries.find(key);

	if(it == m_geometries.end())
	{
		return;
	}
	
	mep_displayer->removeNodeFromScene(it->second.activeGeometry);
		
	if (it->second.displayable->shouldBeDisplayed())
	{
		it->second.activeGeometry = it->second.displayable->getGeometry();
		mep_displayer->addNodeToScene(it->second.activeGeometry);
	}
}


OsgDisplayable * GeometryManager::findGeometry(unsigned key) const
{
	auto it = m_geometries.find(key);
	return it == m_geometries.end() ? nullptr : it->second.displayable.get();
}


bool GeometryManager::removeGeometry(unsigned int key)
{
	auto it = m_geometries.find(key);

	if(it == m_geometries.end())
	{
		return false;
	}

	mep_displayer->removeNodeFromScene(it->second.activeGeometry);

	m_geometries.erase(it);

	return true;
}