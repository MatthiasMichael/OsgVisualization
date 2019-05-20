#pragma once

#include <osg/Group>
#include <osg/ref_ptr>


class OsgDisplayable
{
public:
	OsgDisplayable(bool shouldBeDisplayed = true);
	virtual ~OsgDisplayable() = default;

	OsgDisplayable(const OsgDisplayable &) = delete;
	OsgDisplayable(const OsgDisplayable &&) = delete;

	virtual osg::ref_ptr<osg::Group> getGeometry() const = 0;

	bool shouldBeDisplayed() const { return m_shouldBeDisplayed; }
	void setShouldBeDisplayed(bool v) { m_shouldBeDisplayed = v; }

protected:
	bool m_shouldBeDisplayed;
};


class EmptyDisplayable : public OsgDisplayable
{
public:
	osg::ref_ptr<osg::Group> getGeometry() const override;
};