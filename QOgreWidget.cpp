// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// Own includes
#include "QOgreWidget.h"

// Qt includes
#include <QX11Info>
#include <QDebug>

bool QOgreWidget::initializeOgre(std::string plugins_file,
                        std::string ogre_cfg_file,
                        std::string ogre_log) {
    // create the main ogre object
    _ogreRoot = new Ogre::Root( plugins_file, ogre_cfg_file, ogre_log );


    // setup a renderer
    const Ogre::RenderSystemList& renderers = _ogreRoot->getAvailableRenderers();
    if(renderers.empty()) {
        qDebug() << "No renderer available for OGRE.";
        return false;
    }

    // initialize without creating window
    Ogre::RenderSystem* renderSystem = chooseRenderer(renderers);
    if(!renderSystem) {
        qDebug() << "Supplied a null render system to OGRE.";
        return false;
    }

    _ogreRoot->setRenderSystem(renderSystem);
    _ogreRoot->getRenderSystem()->setConfigOption("Video Mode", QString("%1x%2").arg(width()).arg(height()).toStdString());
    _ogreRoot->getRenderSystem()->setConfigOption("Full Screen", "No");
    _ogreRoot->saveConfig();

     // don't create a window
    _ogreRoot->initialise(false);
    return true;
}

void QOgreWidget::initializeGL() {
    if(!initializeOgre("../bin/plugins.cfg", "../bin/ogre.cfg", "../bin/ogre.log")) {
        return;
    }

    //== Creating and Acquiring Ogre Window ==//

    // Get the parameters of the window QT created
    Ogre::String windowHandle;
#ifdef WIN32
    // Windows code
    windowHandle += Ogre::StringConverter::toString((unsigned long)(winId()));
#elif MACOS
    // Mac code, tested on Mac OSX 10.6 using Qt 4.7.4 and Ogre 1.7.3
    Ogre::String windowHandle  = Ogre::StringConverter::toString(winId());
#else
    // Unix code
    windowHandle  = Ogre::StringConverter::toString((unsigned long)(QX11Info::display()));
    windowHandle += ":";
    windowHandle += Ogre::StringConverter::toString((unsigned int)(QX11Info::appScreen()));
    windowHandle += ":";
    windowHandle += Ogre::StringConverter::toString((unsigned long)(winId()));
#endif

    Ogre::NameValuePairList params;
#ifndef MACOS
    // code for Windows and Linux
    params["parentWindowHandle"] = windowHandle;
    _ogreWindow = _ogreRoot->createRenderWindow( "QOgreWidget_RenderWindow",
                                                 this->width(),
                                                 this->height(),
                                                 false,
                                                 &params );

    _ogreWindow->setActive(true);
    WId ogreWinId = 0x0;
    _ogreWindow->getCustomAttribute( "WINDOW", &ogreWinId );

    if(!ogreWinId) {
        qDebug() << "Ogre window id is null.";
    }

    // bug fix, extract geometry
    QRect geometry = this->frameGeometry();
    // create new window
    create(ogreWinId);
    // set geometrie infos to new window
    setGeometry(geometry);

#else
    // code for Mac
    params["externalWindowHandle"] = windowHandle;
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
    _ogreWindow = _ogreRoot->createRenderWindow("QOgreWidget_RenderWindow",
                                                width(), height(), false, &params);
    _ogreWindow->setActive(true);
    makeCurrent();
#endif

    setAttribute( Qt::WA_PaintOnScreen, true );
    setAttribute( Qt::WA_NoBackground );

    //== Ogre Initialization ==//
    Ogre::SceneType sceneType = Ogre::ST_EXTERIOR_CLOSE;

    _sceneManager = _ogreRoot->createSceneManager( sceneType );
    _sceneManager->setAmbientLight( Ogre::ColourValue(1,1,1) );

    _camera = _sceneManager->createCamera( "QOgreWidget_Cam" );
    _camera->setPosition( Ogre::Vector3(0,1,0) );
    _camera->lookAt( Ogre::Vector3(0,0,0) );
    _camera->setNearClipDistance( 1.0 );

    _viewport = _ogreWindow->addViewport(_camera);
    _viewport->setBackgroundColour(Ogre::ColourValue( 0.8,0.8,1 ));
}

void QOgreWidget::paintGL() {
    // Be sure to call "OgreWidget->repaint();" to call paintGL
    if(_ogreRoot && _ogreWindow) {
        _ogreRoot->renderOneFrame();
    }
}

void QOgreWidget::resizeGL( int width, int height ) {
    Q_UNUSED(width);
    Q_UNUSED(height);
    if(_ogreWindow) {
        _ogreWindow->windowMovedOrResized();
    }
}

Ogre::RenderSystem* QOgreWidget::chooseRenderer(const Ogre::RenderSystemList& renderers) {
    // Default implementation chooses first renderer.
    // Override to do something different.
    return *renderers.begin();
}
