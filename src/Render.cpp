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

#include <iostream>
#include "Render.hpp"
#include "Float.hpp"

using namespace std;

Render::Render() : bquit(false)
{
}

void Render::render(Window *win, Scene &scene)
{
    connect(this, SIGNAL(changeTitle(QString)),
            win,  SLOT(setWindowTitle(QString)));
    s = &scene;

    QElapsedTimer timer;
    timer.start();

    emit changeTitle(QString("Cassata prototype - Rendering - preparation"));

    UnprotectRounding pr;

    scene.firstSolution();
    scene.refineSolution();

    y = 0;
    connect(win, SIGNAL(timeout()), this, SLOT(updateRenderingTitleStep3()));
    updateRenderingTitleStep3();
    for (; y < scene.height(); y++)
    {
        for (int x = 0; x < scene.width(); x++)
        {
            QMutexLocker m(this);
            if (bquit)
                return;
            m.unlock();

            win->draw(x, y, scene.pixel(x, y));
        }

        updateRenderingTitleStep3();
    }
    win->end();
    win->disconnect(this);

    emit changeTitle(QString("Cassata prototype - Rendered - %1").
                     arg(statistics(scene)));

    cout << "Rendered!\n";
    cout << "Rendering time: " <<
        showTime(timer.elapsed()).toStdString() << endl;
}

QString Render::showTime(qint64 ms)
{
    QString time;

    qint64 s  = ms / 1000;
    qint64 m  = s  / 60;
    qint64 h  = m  / 60;
    qint64 d  = h  / 24;
    ms %= 1000;
    s  %= 60;
    m  %= 60;
    h  %= 24;

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

    return time;
}

void Render::quit()
{
    bquit = true;
}

void Render::updateRenderingTitleStep3()
{
    emit changeTitle(
            QString("Cassata prototype - Rendering - Line %1 - %2").
            arg(y).arg(statistics(*s)));
}
