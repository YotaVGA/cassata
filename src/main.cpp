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

using namespace std;

void render(Window *win, const Scene &scene)
{
    QElapsedTimer timer;
    timer.start();

    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
            win->draw(x, y, scene.pixel(x, y));
    }
    win->end();

    qint64 ms = timer.elapsed();
    qint64 s  = ms / 1000;
    qint64 m  = s  / 60;
    qint64 h  = m  / 60;
    qint64 d  = h  / 24;
    ms %= 1000;
    s  %= 60;
    m  %= 60;
    h  %= 24;

    QString time;

    if (!s and !m and !h and !d)
        time = QString("%1ms").arg(ms);
    else if (!m and !h and !d)
        time = QString("%1.%2s").arg(s).arg(ms, 3, 10, QChar('0'));
    else if (!h and !d)
        time = QString("%1:%2.%3").arg(m, 2, 10, QChar('0')).
            arg(s, 2, 10, QChar('0')).arg(ms, 3, 10, QChar('0'));
    else if (!d)
        time = QString("%1:%2:%3.%4").arg(h, 2, 10, QChar('0')).
            arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')).
            arg(ms, 3, 10, QChar('0'));
    else
        time = QString("%1d %2:%3:%4.%5").arg(d).arg(h, 2, 10, QChar('0')).
            arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')).
            arg(ms, 3, 10, QChar('0'));

    cout << "Rendered!\n";
    cout << "Rendering time: " << time.toStdString() << endl;
}

int main(int argc, char **argv)
{
    //Scene registrations
    SceneRegister<SceneImage> regsceneinfo("image");

    try
    {
        QApplication app(argc, argv);

        QStringList args = app.arguments();
        if (args.length() != 3)
            throw QString("Two arguments are required");


        Scene scene(args.at(1));

        Window win;
        win.setFileName(args.at(2));
        win.show();
        QtConcurrent::run(render, &win, scene);

        return app.exec();
    }
    catch (const QString &s)
    {
        cerr << s.toStdString() << endl;
        return EXIT_FAILURE;
    }
}
