#include "AbstractOsgDisplayDescriptor.h"


AbstractOsgDisplayDescriptor::AbstractOsgDisplayDescriptor(const QString & name, Roi3DF sizeHint /*=Roi3DF::EmptyRoi*/) :
	Parametrizable(),
	m_name(name),
	m_sizeHint(sizeHint)
{
	// empty
}


void AbstractOsgDisplayDescriptor::notifyOnAnyParameterChange(QObject * receiver, const char * slot) const
{
	for (const auto p : m_parameters)
	{
		notifyOnParameterChanged(p->getName(), receiver, slot);
	}
}
