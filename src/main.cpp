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
#include <iostream>
#include <cstdlib>
#include "Window.hpp"

using namespace std;

void render(Window *win, const QString &scene)
{
    for (int y = 0; y < 600; y++)
    {
        for (int x = 0; x < 800; x++)
            win->draw(x, y, Qt::white);
    }
    win->end();
    cout << "Rendered!" << endl;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QStringList args = app.arguments();
    if (args.length() != 3)
    {
        cerr << "Two arguments are required" << endl;
        return EXIT_FAILURE;
    }

    QString scene = args.at(1);
    QString image = args.at(2);

    Window win;
    win.setFileName(image);
    win.show();
    QtConcurrent::run(render, &win, scene);

    return app.exec();
}
