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
#include "Ray.hpp"
#include "Geometry.hpp"
#include "DifferentialSpace.hpp"
#include "Quality.hpp"

class SceneCamera;

typedef QHash<QString, QList<QSharedPointer<QObject> > > SceneList;

class Scene
{
    protected:
        int correct, incert, incorrect;
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
        const IFloat value(const Ray &in, const DifferentialSpace &ds,
                           const Quality &quality, qint64 object,
                           int totaldepth, int depth = 0);
        const IFloat sample(const Ray &ray, const Quality &quality,
                            int totaldepth, int depth = 0, qint64 skip = -1);

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

        inline bool singleValuePixel(const IFloat &value)
        {
            using namespace std;

            int a = max(min(
                        (int)round((value * IFloat(256)).upper()), 255), 0),
                b = max(min(
                        (int)round((value * IFloat(256)).lower()), 255), 0);
            if (a != b)
                return false;
            return true;
        }

        inline void newPixelStatistics(const IFloat &value,
                                       const Float &tollerance,
                                       const Float &w, bool single)
        {
            if (single)
                correct++;
            else if (w > tollerance)
                incorrect++;
            else
                incert++;
        }
};

#endif
