#include "OsgDisplayDescriptorWidget.h"

#include <QVBoxLayout>

#include "GenericParameterWidget.h"


OsgDisplayDescriptorWidget::OsgDisplayDescriptorWidget(TypeErasedOsgDisplayDescriptor && descriptor, GeometryManager * pManager, QWidget* pParent /*= nullptr*/) :
	QWidget(pParent), 
	m_descriptor(std::move(descriptor)), 
	mep_manager(pManager), 
	m_displayData(), 
	m_managerId(GeometryManager::KEY_UNASSIGNED),
	mep_parameterWidget(nullptr)
{
	m_ui.setupUi(this);

	m_ui.groupBox->setTitle(m_descriptor.getName());
	m_ui.groupBox->setFlat(true);

	if (m_descriptor.getPolymorphicDescriptor().hasParameters())
	{
		mep_parameterWidget = new GenericParameterWidget(this, m_descriptor.getPolymorphicDescriptor());
		m_ui.layout_inner->addWidget(mep_parameterWidget);
	}
		
	connect(m_ui.groupBox, &QGroupBox::clicked, this, &OsgDisplayDescriptorWidget::switchActive);

	m_descriptor.notifyOnAnyParameterChange(this, SLOT(updateDisplay()));
}


void OsgDisplayDescriptorWidget::updateDisplay()
{
	if (!m_displayData.has_value())
	{
		m_managerId = mep_manager->switchGeometry(std::make_unique<EmptyDisplayable>(), m_managerId);
		return;
	}

	auto d = m_descriptor.makeDisplayable(*m_displayData);
	d->setShouldBeDisplayed(m_ui.groupBox->isChecked());
	
	m_managerId = mep_manager->switchGeometry(std::move(d), m_managerId);

	mep_manager->updateUi();
}


void OsgDisplayDescriptorWidget::setData(std::vector<std::any> && arguments)
{
	m_displayData = std::move(arguments);
	updateDisplay();
}


void OsgDisplayDescriptorWidget::clearData()
{
	m_displayData = std::nullopt;
	updateDisplay();
}


Roi3DF OsgDisplayDescriptorWidget::getSizeHint() const { return m_descriptor.getSizeHint(); }


void OsgDisplayDescriptorWidget::switchActive(bool active)
{
	if (mep_parameterWidget)
	{
		mep_parameterWidget->setEnabled(active);
	}

	OsgDisplayable * pDisplayable = mep_manager->findGeometry(m_managerId);
	
	if (!pDisplayable)
		return;

	pDisplayable->setShouldBeDisplayed(m_ui.groupBox->isChecked());
	mep_manager->updateGeometry(m_managerId);
	mep_manager->updateUi();
}

