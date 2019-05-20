#pragma once

#include "ui_OsgGenericVisualizationWidget.h"

#include "Roi3DF.h"

#include "OsgGeometryManager.h"
//#include "OsgDisplayDescriptorGroupWidget.h"

#include "QtOsgWidget.h" // TODO: Replace with Pointer to GeometryDisplayer from OsgUtil
#include "OsgDisplayDescriptorWidget.h"


class GenericVisualizationOsgWidget : public QWidget
{
public:
	GenericVisualizationOsgWidget(QWidget * parent);

	size_t registerVisualization(TypeErasedOsgDisplayDescriptor && descriptor);

	template<typename... TArgs>
	void setDataToVisualize(size_t id, TArgs&&... data)
	{
		m_displayWidgets[id]->setData(std::forward<TArgs>(data)...);
		updateUi();
	}

	void clearDataToVisualize(size_t id)
	{
		m_displayWidgets[id]->clearData();
		updateUi();
	}

	void updateUi() const;
	void lookAtObjects() const;

	QtOsgWidget * getOsgWidget() const { return m_ui.widget_osg; }

private:
	Roi3DF getSizeHint() const;
	
private:
	Ui_OsgGenericVisualizationWidget m_ui;

	GeometryManager m_geometryManager;

	std::vector<OsgDisplayDescriptorWidget*> m_displayWidgets;
};
