#pragma once

#include <map>
#include <memory>

#include <osg/Group>
#include <osg/ref_ptr>

#include "OsgDisplayable.h"
#include "GeometryDisplayer.h"


class GeometryManager
{
private:
	struct DisplayableWithActiveGeometry
	{
		DisplayableWithActiveGeometry() : displayable(nullptr), activeGeometry(nullptr) { }

		DisplayableWithActiveGeometry(const DisplayableWithActiveGeometry &) = delete;
		DisplayableWithActiveGeometry(DisplayableWithActiveGeometry &&) = default;

		std::unique_ptr<OsgDisplayable> displayable;
		osg::ref_ptr<osg::Group> activeGeometry;
	};

public:
	GeometryManager(GeometryDisplayer * pDisplayer);

	unsigned int switchGeometry(std::unique_ptr<OsgDisplayable> && displayable, unsigned int key = KEY_UNASSIGNED);
	void updateGeometry(unsigned int key = KEY_UNASSIGNED);
	
	OsgDisplayable * findGeometry(unsigned int key) const;
	bool removeGeometry(unsigned int key);

	void updateUi() const { mep_displayer->updateDisplay(); }

	GeometryDisplayer * getWidget() const { return mep_displayer; }

private:
	GeometryDisplayer * mep_displayer;

	std::map<unsigned int, DisplayableWithActiveGeometry> m_geometries;

	unsigned int m_currentKey;

public:
	static const unsigned int KEY_UNASSIGNED;
};