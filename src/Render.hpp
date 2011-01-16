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

#ifndef RENDER_H
#define RENDER_H

#include "Window.hpp"
#include "Scene.hpp"

class Render : public QObject, public QMutex
{
    Q_OBJECT

    protected:
        bool bquit;
        int y;
        Scene *s;

        inline QString statistics(const Scene &scene)
        {
            return QString("Correct: %1, Incert: %2, Incorrect %3").
                   arg(scene.numberCorrectPixels()).
                   arg(scene.numberIncertPixels()).
                   arg(scene.numberIncorrectPixels());
        }

    public:
        Render();

        void render(Window *win, Scene &scene);
        QString showTime(qint64 ms);
        void quit();

    signals:
        void changeTitle(const QString &string);

    public slots:
        void updateRenderingTitleStep3();
};

#endif
