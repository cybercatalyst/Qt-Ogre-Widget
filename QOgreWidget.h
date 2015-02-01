#ifndef __OGREWIDGET_H__
#define __OGREWIDGET_H__

#include <OGRE/Ogre.h>
#include <QGLWidget>
#include <QX11Info>

class QOgreWidget : public QGLWidget
{
  //Q_OBJECT;

 public:
  QOgreWidget( QWidget *parent=0 ):
    QGLWidget( parent ),
    mOgreWindow(NULL)
    {
      init( "../bin/plugins.cfg", "../bin/ogre.cfg", "../bin/ogre.log" );
    }

  virtual ~QOgreWidget()
    {
      mOgreRoot->shutdown();
      delete mOgreRoot;
      destroy();
    }

 protected:
  virtual void initializeGL();
  virtual void resizeGL( int, int );
  virtual void paintGL();

  void init( std::string, std::string, std::string );

  virtual Ogre::RenderSystem *chooseRenderer(const Ogre::RenderSystemList& );

  Ogre::Root *mOgreRoot;
  Ogre::RenderWindow *mOgreWindow;
  Ogre::Camera *mCamera;
  Ogre::Viewport *mViewport;
  Ogre::SceneManager *mSceneMgr;
};

#endif
