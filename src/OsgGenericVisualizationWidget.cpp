#include "OsgGenericVisualizationWidget.h"

#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>


GenericVisualizationOsgWidget::GenericVisualizationOsgWidget(QWidget * parent) :
	QWidget(parent),
	m_geometryManager(nullptr)
{
	m_ui.setupUi(this);

	m_geometryManager = GeometryManager(m_ui.widget_osg);
}


void GenericVisualizationOsgWidget::updateUi() const
{
	m_geometryManager.updateUi();
}


size_t GenericVisualizationOsgWidget::registerVisualization(TypeErasedOsgDisplayDescriptor && descriptor)
{
	OsgDisplayDescriptorWidget * pWidget = new OsgDisplayDescriptorWidget(std::move(descriptor), &m_geometryManager, m_ui.widget_parameter);

	m_ui.layout_parameter->addWidget(pWidget);

	pWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

	m_displayWidgets.push_back(pWidget);

	return m_displayWidgets.size() - 1;
}


void GenericVisualizationOsgWidget::lookAtObjects() const
{
	const Roi3DF usableArea = getSizeHint();

	const osg::Vec3 center = usableArea.start<osg::Vec3>() + (usableArea.end<osg::Vec3>() - usableArea.start<osg::Vec3>()) / 2;
	osg::Vec3 eye = center;
	eye[0] += 2 * std::abs(usableArea.end<osg::Vec3>().x() - usableArea.start<osg::Vec3>().x());
	eye[1] += 1.5 * std::abs(usableArea.end<osg::Vec3>().y() - usableArea.start<osg::Vec3>().y());
	eye[2] += 10;

	m_ui.widget_osg->setLookAt(eye, center);
}


Roi3DF GenericVisualizationOsgWidget::getSizeHint() const
{
	Roi3DF unifiedSizeHint = Roi3DF::EmptyRoi;

	for (const auto d : m_displayWidgets)
	{
		unifiedSizeHint.unite(d->getSizeHint());
	}

	return unifiedSizeHint;
}