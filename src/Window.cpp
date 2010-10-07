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
#include "Window.hpp"

using namespace std;

Window::Window(int x, int y) : lastupdate(false), lastdraw(0),
                               lastline(0), timer(this)
{
    setWindowTitle("Cassata prototype");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resize(x, y);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(500);
}

void Window::resize(int x, int y)
{
    QMutexLocker l(&mutex);

    QWidget::resize(x, y);
    image = (typeof image)(new QImage(x, y, QImage::Format_RGB32));
    image->fill(0);
}

void Window::draw(int x, int y, QColor color)
{
    QMutexLocker l(&mutex);

    image->setPixel(x, y, color.rgb());

    if (y < lastdraw)
        lastdraw = y;

    if (y > lastline)
        lastline = y;
}

void Window::end()
{
    QMutexLocker l(&mutex);

    lastupdate = true;
    timer.singleShot(0, this, SLOT(update()));
}

const QString &Window::fileName()
{
    QMutexLocker l(&mutex);

    return filename;
}

void Window::setFileName(const QString &str)
{
    QMutexLocker l(&mutex);

    filename = str;
}

void Window::paintEvent(QPaintEvent *event)
{
    QMutexLocker l(&mutex);

    QPainter p(this);

    p.drawImage(event->rect().topLeft(), *image, event->rect());
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R)
    {
        cout << "Refreshing!" << std::endl;
        refresh();
    }

    if (event->key() == Qt::Key_S)
    {
        cout << "Saving!" << std::endl;
        save();
    }
}

void Window::update()
{
    QMutexLocker l(&mutex);

    int ld = lastdraw;
    int w = width();
    int h = lastdraw - lastline + 1;

    lastdraw = lastline;

    l.unlock();
    repaint(0, ld, w, h);
    l.relock();

    if (lastupdate)
    {
        timer.stop();
        l.unlock();
        save();
    }
}

void Window::save()
{
    QMutexLocker l(&mutex);

    image->save(filename);
}

void Window::refresh()
{
    QMutexLocker l(&mutex);

    int w = width(), h = height();

    l.unlock();
    repaint(0, 0, w, h);
}
