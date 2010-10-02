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
#include "SceneElement.hpp"

typedef QHash<QString, QSharedPointer<QObject> > SceneList;

class Scene
{
    private:
        QDomDocument scenedoc;
        QHash<QString, SceneList> lists;

    public:
        Scene(const QString &filename);

        SceneList &element(const QString &name);
        const QColor pixel(int x, int y) const;
};

class BaseSceneRegister
{
    public:
        virtual QSharedPointer<SceneElement>
            newClass(const QDomNode &node, Scene &scene) const = 0;

        virtual ~BaseSceneRegister();
};

template <typename T> class SceneRegister : public BaseSceneRegister
{
    public:
        SceneRegister(const QString &name)
        {
            extern QHash<QString, BaseSceneRegister *> sceneregistrations;

            sceneregistrations[name] = this;
        }

        virtual QSharedPointer<SceneElement>
            newClass(const QDomNode &node, Scene &scene) const
        {
            return QSharedPointer<SceneElement>(new T(node, scene));
        }
};

#endif
