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
#include "Ray.hpp"
#include "DifferentialSpace.hpp"
#include "Quality.hpp"

class Geometry;

class SceneCamera;

typedef QHash<QString, QList<QSharedPointer<QObject> > > SceneList;

class Scene
{
    protected:
        int w, h;
        IFloat lim;
        QDomDocument scenedoc;
        QHash<QString, SceneList> lists;
        QSharedPointer<SceneCamera> camera;
        QList<QSharedPointer<Geometry> > geometries;

    public:
        Scene(const QString &filename);

        SceneList &element(const QString &name);
        void firstSolution();
        void refineSolution();
        const QColor pixel(int x, int y);
        const IFloat hit(const Ray &ray, IFloat *distance,
                         DifferentialSpace *ds, qint64 *object,
                         qint64 skip = -1, qint64 start = 0);
        const IFloat value(const DifferentialSpace &ds,
                           const Quality &quality, qint64 object);
        const IFloat sample(const Ray &ray, const Quality &quality,
                            qint64 skip = -1);

        int width() const;
        int height() const;
        void setWidth(int width);
        void setHeight(int height);

        inline const IFloat limits(const Ray &ray) const
        {
            return lim;
        }

        inline const Quality defaultQuality() const
        {
            return Quality();
        }
};

class BaseSceneRegister
{
    public:
        virtual SceneElement *
            newClass(const QDomNode &node, Scene &scene,
                     QSharedPointer<SceneElement> &object) const = 0;

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

        virtual SceneElement *
            newClass(const QDomNode &node, Scene &scene,
                     QSharedPointer<SceneElement> &object) const
        {
            return new T(node, scene, object);
        }
};

#include "SceneCamera.hpp"
#include "Geometry.hpp"

#endif
