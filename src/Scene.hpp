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

class BaseSceneRegister
{
    public:
        virtual QSharedPointer<QObject> newClass(const QDomNode &node) const = 0;

        virtual ~BaseSceneRegister();
};

class Scene
{
    private:
        QDomDocument scenedoc;

    public:
        Scene(const QString &filename);

        const QColor pixel(int x, int y) const;
};

// HACK: The gcc initialize the global objects in link order. Scene.cpp should
// be linked after the Scene*.cpp files.
// A more clean solution should be found.
template <typename T> class SceneRegister : public BaseSceneRegister
{
    public:
        SceneRegister(const QString &name)
        {
            extern QMap<QString, BaseSceneRegister *> sceneregistrations;

            sceneregistrations[name] = this;
        }

        virtual QSharedPointer<QObject> newClass(const QDomNode &node) const
        {
            return QSharedPointer<QObject>(new T(node));
        }
};

#endif
