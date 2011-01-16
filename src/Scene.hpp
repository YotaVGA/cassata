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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <QtXml>
#include <QtGui>

typedef QHash<QString, QList<QSharedPointer<QObject> > > SceneList;

class Scene
{
    protected:
        int correct, incert, incorrect;
        int w, h;
        QDomDocument scenedoc;
        QHash<QString, SceneList> lists;

    public:
        Scene(const QString &filename);

        SceneList &element(const QString &name);
        void firstSolution();
        void refineSolution();
        const QColor pixel(int x, int y);

        int width() const;
        int height() const;
        void setWidth(int width);
        void setHeight(int height);

        inline int numberCorrectPixels() const
        {
            return correct;
        }

        inline int numberIncertPixels() const
        {
            return incert;
        }

        inline int numberIncorrectPixels() const
        {
            return incorrect;
        }
};

#endif
