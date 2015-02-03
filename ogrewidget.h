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

#pragma once

// OGRE includes
#include <OGRE/Ogre.h>

// Qt includes
#include <QGLWidget>

#ifdef Q_OS_UNIX
#include <QX11Info>
#endif

class OgreWidget : public QGLWidget {
    Q_OBJECT
public:
    OgreWidget(QWidget *parent = 0);
    virtual ~OgreWidget();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int, int);
    virtual void paintGL();

    virtual Ogre::RenderSystem *chooseRenderer(const Ogre::RenderSystemList&);

    Ogre::Root *        _ogreRoot;
    Ogre::RenderWindow *_ogreWindow;
    Ogre::Camera *      _camera;
    Ogre::Viewport *    _viewport;
    Ogre::SceneManager *_sceneManager;
};
