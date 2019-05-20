#pragma once

#include "Roi3DF_osg.h"

#include "Parametrizable.h"


class AbstractOsgDisplayDescriptor : public Parametrizable
{
public:
	AbstractOsgDisplayDescriptor(const QString & name, Roi3DF sizeHint = Roi3DF::EmptyRoi);

	void notifyOnAnyParameterChange(QObject* receiver, const char * slot) const;

	void setSizeHint(Roi3DF area) { m_sizeHint = area; }

	const QString & getName() const { return m_name; }
	Roi3DF getSizeHint() const { return m_sizeHint; }

private:
	QString m_name;

	Roi3DF m_sizeHint;
};