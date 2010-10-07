// Copyright ® 2010 Fulvio Satta
// If you want contact me, send an email to Yota_VGA@users.sf.net
// This file is part of Cassata
//
// Cassata is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Cassata is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
// 02110-1301  USA

#include <QtGui>
#include <QtXml>
#include <iostream>
#include <cstdlib>
#include "Window.hpp"
#include "Scene.hpp"
#include "SceneImage.hpp"
#include "SceneMesh.hpp"
#include "ScenePinhole.hpp"
#include "Render.hpp"

using namespace std;

void render(Render *render, Window *win, Scene *scene)
{
    render->render(win, *scene);
}

int main(int argc, char **argv)
{
    //Scene registrations
    SceneRegister<SceneImage>   regsceneimage ("image");
    SceneRegister<SceneMesh>    regscenemesh  ("mesh");
    SceneRegister<ScenePinhole> regscenecamera("pinhole");

    QLocale::setDefault(QLocale::C);

    try
    {
        QApplication app(argc, argv);

        QStringList args = app.arguments();
        if (args.length() != 3)
            throw QString("Two arguments are required");

        Render r;

        QElapsedTimer timer;
        timer.start();

        Scene scene(args.at(1));

        cout << "Parsing time: " <<
            r.showTime(timer.elapsed()).toStdString() << endl;

        Window win;
        win.resize(scene.width(), scene.height());
        win.show();
        win.setFileName(args.at(2));
        QFuture<void> retthread = QtConcurrent::run(render, &r, &win, &scene);

        int ret = app.exec();
        r.lock();
        r.quit();
        r.unlock();
        retthread.waitForFinished();
        return ret;
    }
    catch (const QString &s)
    {
        cerr << s.toStdString() << endl;
        return EXIT_FAILURE;
    }
}
