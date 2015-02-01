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

#pragma once

// OGRE includes
#include <OGRE/Ogre.h>

// Qt includes
#include <QGLWidget>
#include <QX11Info>

class QOgreWidget : public QGLWidget {
    Q_OBJECT
public:
    QOgreWidget(QWidget *parent = 0):
        QGLWidget(parent),
        _ogreWindow(0) {
    }

    virtual ~QOgreWidget() {
        _ogreRoot->shutdown();
        delete _ogreRoot;
        destroy();
    }

protected:
    virtual void initializeGL();
    virtual void resizeGL(int, int);
    virtual void paintGL();

    bool initializeOgre();

    virtual Ogre::RenderSystem *chooseRenderer(const Ogre::RenderSystemList& );

    Ogre::Root *        _ogreRoot;
    Ogre::RenderWindow *_ogreWindow;
    Ogre::Camera *      _camera;
    Ogre::Viewport *    _viewport;
    Ogre::SceneManager *_sceneManager;
};
