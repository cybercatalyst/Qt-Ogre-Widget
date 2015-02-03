////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of QtOgreWidget.                                         //
// Copyright (c) 2015 Jacob Dawid <jacob@omg-it.works>                        //
//                                                                            //
// QtOgreWidget is free software: you can redistribute it and/or              //
// modify it under the terms of the GNU Affero General Public License as      //
// published by the Free Software Foundation, either version 3 of the         //
// License, or (at your option) any later version.                            //
//                                                                            //
// QtOgreWidget is distributed in the hope that it will be useful,            //
// but WITHOUT ANY WARRANTY; without even the implied warranty of             //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              //
// GNU Affero General Public License for more details.                        //
//                                                                            //
// You should have received a copy of the GNU Affero General Public           //
// License along with QtOgreWidget.                                           //
// If not, see <http://www.gnu.org/licenses/>.                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// Own includes
#include "ogrewidget.h"

// Qt includes
#include <QDebug>

OgreWidget::OgreWidget(QWidget *parent)
    : QGLWidget(parent),
    _ogreWindow(0) {
}

OgreWidget::~OgreWidget() {
    _ogreRoot->shutdown();
    delete _ogreRoot;
}

void OgreWidget::initializeGL() {
    _ogreRoot = new Ogre::Root();

    const Ogre::RenderSystemList& renderers = _ogreRoot->getAvailableRenderers();
    if(renderers.empty()) {
        qDebug() << "No renderer available for OGRE.";
        return;
    }

    Ogre::RenderSystem* renderSystem = chooseRenderer(renderers);
    if(!renderSystem) {
        qDebug() << "Supplied a null renderer to OGRE.";
        return;
    }

    _ogreRoot->setRenderSystem(renderSystem);
    _ogreRoot->getRenderSystem()->setConfigOption("Video Mode",
                                                  QString("%1x%2")
                                                  .arg(width())
                                                  .arg(height())
                                                  .toStdString());
    _ogreRoot->getRenderSystem()->setConfigOption("Full Screen", "No");
    _ogreRoot->saveConfig();
    _ogreRoot->initialise(false);

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
#ifndef Q_OS_MAC
    params["parentWindowHandle"] = windowHandle;
    _ogreWindow = _ogreRoot->createRenderWindow("QOgreWidget_RenderWindow",
                                                width(), height(), false, &params);

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
    params["externalWindowHandle"] = windowHandle;
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
    _ogreWindow = _ogreRoot->createRenderWindow("QOgreWidget_RenderWindow",
                                                width(), height(), false, &params);
    _ogreWindow->setActive(true);
    makeCurrent();
#endif

    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_NoBackground);

    Ogre::SceneType sceneType = Ogre::ST_EXTERIOR_CLOSE;

    _sceneManager = _ogreRoot->createSceneManager(sceneType);
    _sceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));

    _camera = _sceneManager->createCamera("QOgreWidget_Cam");
    _camera->setPosition(Ogre::Vector3(0, 1, 0));
    _camera->lookAt(Ogre::Vector3(0, 0, 0));
    _camera->setNearClipDistance(1.0);

    _viewport = _ogreWindow->addViewport(_camera);
    _viewport->setBackgroundColour(Ogre::ColourValue(0.8, 0.8, 1));
}

void OgreWidget::paintGL() {
    if(_ogreRoot && _ogreWindow) {
        _ogreRoot->renderOneFrame();
    }
}

void OgreWidget::resizeGL(int width, int height) {
    Q_UNUSED(width);
    Q_UNUSED(height);
    if(_ogreWindow) {
        _ogreWindow->windowMovedOrResized();
    }
}

Ogre::RenderSystem* OgreWidget::chooseRenderer(const Ogre::RenderSystemList& renderers) {
    // Default implementation chooses first renderer.
    // Override to do something different.
    return *renderers.begin();
}
