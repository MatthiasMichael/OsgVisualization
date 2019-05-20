#pragma once

#include <QGlWidget>
#include <QKeyEvent>
#include <QMutex>

#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/RenderInfo>
#include <osg/Image>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

//#include "rtcvImage.h"

#include "GeometryDisplayer.h"


//class ViewCaptureCallback : public QObject, public osg::Camera::DrawCallback
//{
//	Q_OBJECT
//public:
//	ViewCaptureCallback() : flag( false)
//	{
//		// empty
//	}
//	
//	virtual void operator()(const osg::Camera & camera) const;
//	virtual void operator()(const osg::RenderInfo & info) const;
//
//	const rtcvImageRgba & getRenderedImage(){ return renderedImg;}
//
//	mutable bool flag;
//	osg::GraphicsContext* gc;
//	mutable rtcvImageRgba renderedImg;
//
//signals:
//	void imageRendered() const;
//};


class QtOsgWidget : public QGLWidget, public GeometryDisplayer
{
public:
	QtOsgWidget( QWidget* parent);

	bool event( QEvent* event ) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	
	void updateDisplay() override;
	bool addNodeToScene(osg::ref_ptr<osg::Node> node) override;
	bool removeNodeFromScene(osg::ref_ptr<osg::Node> node) override;

	void renderImage();
	void setTopView();
	void setPerspectiveView(const float openingAngle = 30.);
	void setTopViewScale( const float scale );
	void setLookAt(const osg::Vec3 & eye, const osg::Vec3 & center, const osg::Vec3 & up = osg::Vec3(0, 0, 1));
	void changeRootTransformation(const osg::Matrix & transformation);

	//rtcvImageRgba & getRenderedImage();
	osg::ref_ptr<osg::Group> getRoot() const { return m_root; }
	QTransform getWorld2ImgTrafo() const { return m_trafoWorld2Img; }

	//ViewCaptureCallback * mp_captureCallback;

private:
	osgGA::EventQueue* getEventQueue() const;

private:
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mp_graphicsWindow;
	osg::ref_ptr<osgViewer::CompositeViewer> mp_viewer;  

	osg::ref_ptr<osg::Group> m_root;
	osg::ref_ptr<osgViewer::View> mp_view;
	osg::ref_ptr<osg::Camera> mp_camera;
	osg::ref_ptr<osgGA::TrackballManipulator> mp_manipulator;

	QTransform m_trafoWorld2Img;
	float m_topViewScale;
	//rtcvImageRgba m_renderedImage;
	bool m_isTopView;
	osg::Matrixd m_topViewRotMat;

	QMutex m_mutex;
};