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

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtGui>

class Window : public QWidget
{
    Q_OBJECT

    private:
        bool lastupdate;
        int lastdraw, lastline;
        QString filename;
        QSharedPointer<QImage> image;
        QTimer timer;
        QMutex mutex;

    public:
        Window(int x = 800, int y = 600);

        void resize(int x, int y);
        void draw(int x, int y, QColor color);
        void end();
        const QString &fileName();
        void setFileName(const QString &str);

    protected:
        virtual void paintEvent(QPaintEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);

    public slots:
        void updateImage();
        void save();
        void refresh();

    signals:
        void timeout();
};

#endif
