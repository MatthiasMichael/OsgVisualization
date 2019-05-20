#include <QApplication>
#include <QTimer>

#include "QtOsgWidget.h"

#include "OsgGeometryManager.h"

#include "DisplayableGrid.h"
#include "DisplayableWalls.h"
#include "DisplayableCameras.h"

#include "osgCommonObjects_test.h"
#include "DisplayableAreaLights.h"
#include "DisplayableCoordinateSystem.h"


int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	Roi3DF area(osg::Vec3(0, 0, 0), osg::Vec3(10, 10, 10));

	QtOsgWidget widget(nullptr);

	osg::StateSet * rootStateSet = widget.getRoot()->getOrCreateStateSet();
	rootStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	rootStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	//widget.addNodeToScene(createLightsFromUsableArea(area));

	GeometryManager manager(&widget);

	manager.switchGeometry(std::make_unique<DisplayableAreaLight>(area));
	manager.switchGeometry(std::make_unique<DisplayableGrid>(area, osg::Vec3(0.5, 0.5, 0.5)));
	manager.switchGeometry(std::make_unique<DisplayableWalls>(WallsGeometry(buildWalls())));
	manager.switchGeometry(std::make_unique<DisplayableCoordinateSystem>(osg::Vec3(0, 0, 0), 2, 5));

	// TODO: Test Displayable Camera

	auto keyFlip = manager.switchGeometry(std::make_unique<DisplayableWalls>(WallsGeometry(buildFlip1())));

	widget.setLookAt(osg::Vec3(40, 30, 20), osg::Vec3(0, 0, 0));

	widget.show();

	QTimer t;
	bool state = true;
	QObject::connect(&t, &QTimer::timeout, [&manager, &keyFlip, &state]
	{
		WallsGeometry w = state ? buildFlip1() : buildFlip2();
		manager.switchGeometry(std::make_unique<DisplayableWalls>(w), keyFlip);
		manager.updateUi();
		state = !state;
	});

	t.start(1000);

	return app.exec();
}
