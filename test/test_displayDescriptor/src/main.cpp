#include <QApplication>
#include <QTimer>

#include "QtOsgWidget.h"

#include "OsgGenericVisualizationWidget.h"
#include "FunctionalOsgDisplayDescriptor.h"

#include "DisplayableAreaLights.h"
#include "DisplayableCameras.h"
#include "DisplayableCoordinateSystem.h"
#include "DisplayableGrid.h"
#include "DisplayableWalls.h"

#include "osgCommonObjects_test.h"


auto areaLightsDescriptor = makeFunctionalOsgDisplayDescriptor
(
	"Area Lights",
	[](Parametrizable &) {},
	[](const Parametrizable &, const Roi3DF roi)
	{
		return std::make_unique<DisplayableAreaLight>(roi);
	}
);


auto coordinateSystemDescriptor = makeFunctionalOsgDisplayDescriptor
(
	"Coordinate System",
	[](Parametrizable & d)
	{
		d.addDoubleParameter("Pos X", -1000, 1000, 0, 1);
		d.addDoubleParameter("Pos Y", -1000, 1000, 0, 1);
		d.addDoubleParameter("Pos Z", -1000, 1000, 0, 1);
		
		d.addDoubleParameter("Size", 0, 100, 0.2, 1);
		
		d.addDoubleParameter("Beam Length", 0, 100, 10, 1);
	},
	[](const Parametrizable & d)
	{
		return std::make_unique<DisplayableCoordinateSystem>
		(
			osg::Vec3
			(
				d.getDoubleParameter("Pos X"),
				d.getDoubleParameter("Pos Y"),
				d.getDoubleParameter("Pos Z")
			),
			d.getDoubleParameter("Size"),
			d.getDoubleParameter("Beam Length")
		);
	}
);

auto gridDescriptor = makeFunctionalOsgDisplayDescriptor
(
	"Grid",
	[](Parametrizable & d)
	{
		d.addDoubleParameter("Dist X", 0.1, 10, 0.5);
		d.addDoubleParameter("Dist Y", 0.1, 10, 0.5);
		d.addDoubleParameter("Dist Z", 0.1, 10, 0.5);
	},
	[](const Parametrizable & d, const Roi3DF & roi)
	{
		return std::make_unique<DisplayableGrid>
		(
			roi,
			osg::Vec3
			(
				d.getDoubleParameter("Dist X"),
				d.getDoubleParameter("Dist Y"),
				d.getDoubleParameter("Dist Z")
			)
		);
	}
);

auto wallsDescriptor = makeFunctionalOsgDisplayDescriptor
(
	"Walls",
	[](Parametrizable &) {},
	[](const Parametrizable & d, const WallsGeometry & walls)
	{
		return std::make_unique<DisplayableWalls>(walls);
	}
);

auto flipDescriptor = makeFunctionalOsgDisplayDescriptor
(
	"Flip",
	[](Parametrizable & d) {},
	[](const Parametrizable & d, const WallsGeometry & walls)
	{
		return std::make_unique<DisplayableWalls>(walls);
	}
);


osg::Matrix rootTransformation(-1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1);

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	Roi3DF area(osg::Vec3(0, 0, 0), osg::Vec3(10, 10, 10));

	GenericVisualizationOsgWidget widget(nullptr);

	widget.getOsgWidget()->changeRootTransformation(rootTransformation);

	osg::StateSet * rootStateSet = widget.getOsgWidget()->getRoot()->getOrCreateStateSet();
	rootStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	rootStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	const auto id_lights = widget.registerVisualization(areaLightsDescriptor);
	const auto id_walls = widget.registerVisualization(wallsDescriptor);
	const auto id_grid = widget.registerVisualization(gridDescriptor);
	const auto id_coords = widget.registerVisualization(coordinateSystemDescriptor);

	auto id_flip = widget.registerVisualization(flipDescriptor);

	WallsGeometry walls(buildWalls());
	widget.setDataToVisualize(id_lights, area);
	widget.setDataToVisualize(id_walls, walls);
	widget.setDataToVisualize(id_grid, area);
	widget.setDataToVisualize(id_coords);


	WallsGeometry flip(buildFlip1());
	widget.setDataToVisualize(id_flip, flip);

	widget.updateUi();

	widget.getOsgWidget()->setLookAt(osg::Vec3(40, 30, 20), osg::Vec3(0, 0, 0));

	widget.show();

	QTimer t;
	bool state = true;
	QObject::connect(&t, &QTimer::timeout, [&widget, &id_flip, &state]
	                 {
		                 WallsGeometry w = state ? buildFlip1() : buildFlip2();
		                 widget.setDataToVisualize(id_flip, w );
		                 widget.updateUi();
		                 state = !state;
	                 });

	t.start(1000);

	return app.exec();
}
