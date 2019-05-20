#include "QtOsgWidget.h"
#include <osgGA/CameraViewSwitchManipulator>
#include <osg/Image>
#include <osgDB/WriteFile>
#include <QMutexLocker>


//void ViewCaptureCallback::operator()(const osg::Camera & camera) const
//{
//	if( !flag )
//		return;
//
//	glReadBuffer(camera.getDrawBuffer());
//	
//	int x,y,width,height;
//	x = camera.getViewport()->x();
//	y = camera.getViewport()->y();
//	width = camera.getViewport()->width();
//	height = camera.getViewport()->height(); 
//
//	if( renderedImg.getSx() != width || renderedImg.getSy() != height )
//		renderedImg.resize( width, height );
//
//	unsigned char * data = (unsigned char*)renderedImg.getDataPointer();
//	glReadPixels( x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );
//
//	flag = false;
//
//	Q_EMIT( imageRendered() );
//}
//
//
//void ViewCaptureCallback::operator()(const osg::RenderInfo & info) const
//{
//	if( !flag )
//		return;
//
//// 	glReadBuffer( GL_BACK ); 
//// 
//// 	const osg::Camera * camera = info.getCurrentCamera();
//// 
//// 	int x,y,width,height;
//// 	x = camera->getViewport()->x();
//// 	y = camera->getViewport()->y();
//// 	width = camera->getViewport()->width();
//// 	height = camera->getViewport()->height(); 
//// 
//// 	if( renderedImg.getSx() != width || renderedImg.getSy() != height )
//// 		renderedImg.resize( width, height );
//// 
//// 	unsigned char * data = (unsigned char*)renderedImg.getDataPointer();
//// 	glReadPixels( x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );
//// 
//// 	flag = false;
//// 
//// 	Q_EMIT( imageRendered() );
//}


QtOsgWidget::QtOsgWidget(QWidget* parent) : QGLWidget( parent ),
	mp_graphicsWindow( new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(), this->width(), this->height() ) ),
	mp_viewer( new osgViewer::CompositeViewer ),
	m_topViewScale(1.f), 
	m_isTopView( false )
{
	m_root = osg::ref_ptr<osg::Group>(new osg::Group());

	float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );

	mp_camera = new osg::Camera;
	mp_camera->setViewport( 0, 0, this->width(), this->height() );
	mp_camera->setClearColor( osg::Vec4( 0.f, 0.f, 1.f, 1.f ) );
	mp_camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
	mp_camera->setGraphicsContext( mp_graphicsWindow );

	//mp_captureCallback = new ViewCaptureCallback();

	//mp_camera->setFinalDrawCallback(mp_captureCallback);
	//mp_camera->setPostDrawCallback(mp_captureCallback);
	//mp_camera->setInitialDrawCallback(mp_captureCallback);
	//mp_viewer->renderingTraversals();

	mp_view = new osgViewer::View;
	mp_view->setCamera( mp_camera );
	mp_view->addEventHandler( new osgViewer::StatsHandler );

#ifdef WITH_PICK_HANDLER
	mp_view->addEventHandler( new PickHandler );
#endif

	mp_manipulator = new osgGA::TrackballManipulator;
	mp_manipulator->setAllowThrow( false );
	mp_view->setCameraManipulator( mp_manipulator );

	mp_viewer->addView( mp_view );
	mp_viewer->setThreadingModel( osgViewer::CompositeViewer::SingleThreaded );
	mp_viewer->realize();

	// This ensures that the widget will receive keyboard events. This focus
	// policy is not set by default. The default, Qt::NoFocus, will result in
	// keyboard events that are ignored.
	this->setFocusPolicy( Qt::StrongFocus );
	this->setMinimumSize( 100, 100 );

	// Ensures that the widget receives mouse move events even though no
	// mouse button has been pressed. We require this in order to let the
	// graphics window switch viewports properly.
	this->setMouseTracking( true );

	mp_viewer->getView(0)->setSceneData(m_root);
	//m_root->addChild( mp_camera );

	/*if ( m_renderedImage.getSx() != this->width() || m_renderedImage.getSy() != this->height() )
		m_renderedImage.resize( this->width(), this->height() );*/

// 	m_topViewRotMat.makeRotate(-M_PI / 2., osg::Vec3d(0, 0, 1) );
// 	m_topViewRotMat.makeRotate(M_PI, osg::Vec3d(0, 1, 0) );

	m_topViewRotMat.makeRotate(
		osg::DegreesToRadians(-90.0), osg::Vec3(0,0,1), // heading 
		osg::DegreesToRadians(0.0), osg::Vec3(0,1,0), // roll
		osg::DegreesToRadians(180.0), osg::Vec3(1,0,0)  ); // pitch
}


void QtOsgWidget::setTopView()
{
	if( !m_isTopView)
	{
		m_isTopView = true;	
		osg::ref_ptr<osg::MatrixTransform> transnodeosg = new osg::MatrixTransform;
		transnodeosg->setMatrix(m_topViewRotMat);
		const int numChild = m_root->getNumChildren();

		for (int i = 0; i < numChild; ++i)
		{
			osg::ref_ptr<osg::Node> pNode = m_root->getChild(i);
			if (pNode)
			{
				m_root->removeChild(i);
				transnodeosg->addChild( pNode );
			}
		}
		m_root->addChild(transnodeosg);
	}
	m_trafoWorld2Img = QTransform(); //identity

	float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
	mp_camera->setProjectionMatrixAsOrtho2D(0, float(this->width()) * m_topViewScale, float(this->height()) * m_topViewScale, 0); //, 1.f, 1000.f);
	
	osgGA::CameraViewSwitchManipulator* mani = new osgGA::CameraViewSwitchManipulator;
	mp_view->setCameraManipulator(mani);
	
	// TODO: Rotation!

 	m_trafoWorld2Img.rotate(-90, Qt::ZAxis);
 	m_trafoWorld2Img.rotate(180, Qt::YAxis);
	m_trafoWorld2Img.scale(1./m_topViewScale, 1./m_topViewScale);
}


void QtOsgWidget::setPerspectiveView(const float openingAngle /*= 30.*/)
{
	float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );

	mp_camera->setProjectionMatrixAsPerspective( openingAngle, aspectRatio, 1.f, 1000.f );mp_manipulator = new osgGA::TrackballManipulator;
	mp_manipulator->setAllowThrow( false );

	mp_view->setCameraManipulator( mp_manipulator );

	const int numChild = m_root->getNumChildren();

	for (int i = 0; i < numChild; ++i)
	{
		osg::ref_ptr<osg::MatrixTransform> pTrafo = dynamic_cast<osg::MatrixTransform *>(m_root->getChild(i));
		if (pTrafo)
		{
			const int numTrafoChild = pTrafo->getNumChildren();
			for (int j = 0; j < numTrafoChild; ++j)
			{
				osg::ref_ptr<osg::Node> pNode = pTrafo->getChild(i);
				if (pNode)
				{
					pTrafo->removeChild( pNode );
					m_root->addChild(pNode);
				}
			}
			m_root->removeChild(i);
		}
	}
}


bool QtOsgWidget::event( QEvent* event )
{
	bool handled = QGLWidget::event( event );

	switch( event->type() )
	{
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
	case QEvent::MouseButtonDblClick:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseMove:
		update();
		break;

	default:
		break;
	}

	return handled;
}


void QtOsgWidget::mousePressEvent(QMouseEvent* event)
{
	// 1 = left mouse button
	// 2 = middle mouse button
	// 3 = right mouse button

	unsigned int button = 0;

	switch( event->button() )
	{
	case Qt::LeftButton:
		button = 1;
		break;

	case Qt::MiddleButton:
		button = 2;
		break;

	case Qt::RightButton:
		button = 3;
		break;

	default:
		break;
	}

	this->getEventQueue()->mouseButtonPress( 
		static_cast<float>( event->x() ),
		static_cast<float>( event->y() ),
		button 
	);

}


void QtOsgWidget::mouseMoveEvent(QMouseEvent* event)
{
	this->getEventQueue()->mouseMotion( 
		static_cast<float>( event->x() ),
		static_cast<float>( event->y() ) 
	);
}


void QtOsgWidget::mouseReleaseEvent(QMouseEvent* event)
{
	// 1 = left mouse button
	// 2 = middle mouse button
	// 3 = right mouse button

	unsigned int button = 0;

	switch( event->button() )
	{
	case Qt::LeftButton:
		button = 1;
		break;

	case Qt::MiddleButton:
		button = 2;
		break;

	case Qt::RightButton:
		button = 3;
		break;

	default:
		break;
	}

	this->getEventQueue()->mouseButtonRelease( 
		static_cast<float>( event->x() ),
		static_cast<float>( event->y() ),
		button 
	);
}


void QtOsgWidget::wheelEvent(QWheelEvent* event)
{
	event->accept();
	int delta = event->delta();

	osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   
		osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;

	this->getEventQueue()->mouseScroll( motion );
}


void QtOsgWidget::paintEvent(QPaintEvent* event)
{
	this->makeCurrent();

	QPainter painter( this );
	painter.setRenderHint( QPainter::Antialiasing );

	this->paintGL();

	painter.end();

	this->doneCurrent();
}


void QtOsgWidget::paintGL()
{
	QMutexLocker lock(&m_mutex);
	mp_viewer->frame();
}


void QtOsgWidget::resizeGL(int width, int height)
{
	this->getEventQueue()->windowResize( this->x(), this->y(), width, height );
	mp_graphicsWindow->resized( this->x(), this->y(), width, height );

	std::vector<osg::Camera*> cameras;
	mp_viewer->getCameras( cameras );

	//assert( cameras.size() == 1 );

	cameras.front()->setViewport( 0, 0, this->width(), this->height() );
}


void QtOsgWidget::keyReleaseEvent(QKeyEvent* event)
{
	QString keyString   = event->text();
	const char* keyData = keyString.toLocal8Bit().data();

	this->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}


void QtOsgWidget::updateDisplay()
{
	update();
}


osgGA::EventQueue* QtOsgWidget::getEventQueue() const
{
	osgGA::EventQueue* eventQueue = mp_graphicsWindow->getEventQueue();

	if( eventQueue )
		return eventQueue;
	else
		throw std::runtime_error( "Unable to obtain valid event queue");
}

void QtOsgWidget::renderImage()
{
	//mp_captureCallback->flag = true;
	mp_viewer->updateTraversal();
}


//rtcvImageRgba & QtOsgWidget::getRenderedImage()
//{
//	m_renderedImage = mp_captureCallback->getRenderedImage();
//	m_renderedImage.swapRB();
//	m_renderedImage.flipUpDown();
//	return m_renderedImage;
//}


void QtOsgWidget::setLookAt(const osg::Vec3 & eye, const osg::Vec3 & center, const osg::Vec3 & up /*= osg::Vec3(0, 0, 1)*/)
{
	mp_manipulator->setAutoComputeHomePosition(false);
	mp_manipulator->setHomePosition(eye, center, up);
	mp_manipulator->home(0.0);
}


void QtOsgWidget::changeRootTransformation(const osg::Matrix & transformation)
{
	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform(transformation);
	trans->addChild(m_root);
	mp_viewer->getView(0)->setSceneData(trans);
}


bool QtOsgWidget::addNodeToScene(osg::ref_ptr<osg::Node> node)
{
	QMutexLocker lock(&m_mutex);

	if (m_isTopView)
	{
		const int numChild = m_root->getNumChildren();

		bool added = false;
		for (int i = 0; i < numChild; ++i)
		{
			osg::ref_ptr<osg::MatrixTransform> pTrafo = dynamic_cast<osg::MatrixTransform *>(m_root->getChild(i));
			if (pTrafo)
			{
				added |= pTrafo->addChild( node );
			}
		}
		return added;
	}
	else
	{
		return m_root->addChild(node);
	}
}


bool QtOsgWidget::removeNodeFromScene(osg::ref_ptr<osg::Node> node)
{
	QMutexLocker lock(&m_mutex);
	return m_root->removeChild(node);
}


void QtOsgWidget::setTopViewScale(const float scale)
{
	m_topViewScale = scale; 
	this->setTopView();
}