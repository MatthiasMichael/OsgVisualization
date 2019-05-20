#pragma once

#include <any>
#include <optional>
#include <vector>

#include "OsgGeometryManager.h"

#include "TypeErasedOsgDisplayDescriptor.h"

#include "ui_OsgDisplayDescriptorWidget.h"


class OsgDisplayDescriptorWidget : public QWidget
{
	Q_OBJECT

public:
	OsgDisplayDescriptorWidget(TypeErasedOsgDisplayDescriptor && descriptor, GeometryManager * pManager, QWidget * pParent = nullptr);

	template<typename... TArgs>
	void setData(TArgs &&... displayData);
	void setData(std::vector<std::any> && arguments);

	void clearData();

	Roi3DF getSizeHint() const;

public slots:
	void updateDisplay();
	void switchActive(bool active);

private:
	TypeErasedOsgDisplayDescriptor m_descriptor;
	GeometryManager * mep_manager;

	std::optional<std::vector<std::any>> m_displayData;
	unsigned int m_managerId;

	Ui::OsgDisplayDescriptorWidget m_ui;
	GenericParameterWidget * mep_parameterWidget;
};


template <typename ... TArgs>
void OsgDisplayDescriptorWidget::setData(TArgs &&... displayData)
{
	if (m_displayData.has_value())
	{
		m_displayData->clear();
	}
	else
	{
		m_displayData = std::vector<std::any>{ };
	}

	(m_displayData->push_back(std::forward<TArgs>(displayData)), ...);
	updateDisplay();
}
