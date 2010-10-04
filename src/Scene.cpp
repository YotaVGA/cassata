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

#include "Scene.hpp"
#include "Geometry.hpp"

QHash<QString, BaseSceneRegister *> sceneregistrations;

BaseSceneRegister::~BaseSceneRegister()
{
}

Scene::Scene(const QString &filename) : scenedoc("scene"), w(800), h(600)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        throw QString("It is impossible to open the scene");

    QString error;
    int errorline, errorcolumn;
    if (!scenedoc.setContent(&file, &error, &errorline, &errorcolumn))
        throw QString("Error in the scene file: %2.%3: %4").
            arg(errorline).arg(errorcolumn).arg(error);

    QList<QSharedPointer<SceneElement> > shaders;
    for (QDomNode i = scenedoc.firstChildElement().firstChild(); !i.isNull();
            i = i.nextSibling())
    {
        if (!i.isElement())
            continue;

        QDomElement elem = i.toElement();
        QHash<QString, BaseSceneRegister *>::const_iterator shaderiterator =
            sceneregistrations.find(elem.tagName());
        if (shaderiterator == sceneregistrations.end())
            throw QString("Error in %1.%2: Shader %3 do not found").
                arg(i.lineNumber()).arg(i.columnNumber()).
                arg(elem.tagName());

        QSharedPointer<SceneElement> shader;
        shaderiterator.value()->newClass(i, *this, shader);
        shaders << shader;
        QString name = elem.attribute("id");
        if (!name.isEmpty())
            element("names")[name] << shader;
    }

    for (int i = 0; i < shaders.size(); i++)
        shaders[i]->initialize();

    element("names").clear();

    if (!element("camera")["shader"].size())
        throw QString("Error in scenefile: Camera missing");

    if (element("camera")["shader"].size() > 1)
        throw QString("Error in scenefile: More than a camera");
    
    camera = qSharedPointerDynamicCast<SceneCamera>(
            element("camera")["shader"][0]);

    for (int i = 0; i < element("geometry")["list"].size(); i++)
        geometries << qSharedPointerDynamicCast<Geometry>(
                element("geometry")["list"][0]);
}

SceneList &Scene::element(const QString &name)
{
    return lists[name];
}

void Scene::firstSolution()
{
    lim = IFloat(0, 1000);
}

void Scene::refineSolution()
{
}

const QColor Scene::pixel(int x, int y)
{
    return camera->pixel(x, y);
}

const IFloat Scene::hit(const Ray &ray, IFloat *distance,
                        DifferentialSpace *ds, qint64 *object,
                        qint64 skip, qint64 start)
{
    for (qint64 i = start; i < geometries.size(); i++)
    {
        if (i == skip)
            continue;

        using namespace boost::numeric::interval_lib;
        using namespace compare::certain;

        IFloat h = geometries[i]->hit(ray, distance, ds);
        if (h == 0)
            continue;

        *object = i;
        return h;
    }

    return 0;
}

const IFloat Scene::value(const DifferentialSpace &ds, const Quality &quality,
                          qint64 object)
{
    return geometries[object]->value(ds, quality);
}

const IFloat Scene::sample(const Ray &ray, const Quality &quality,
                           qint64 skip)
{
    if (quality.stopIteration())
        return limits(ray);

    IFloat hitp = 0;
    IFloat distance = INFINITY;
    IFloat val = 0;

    IFloat temphit;
    IFloat tempdistance;
    qint64 i = 0;
    DifferentialSpace ds;
    while (temphit = hit(ray, &tempdistance, &ds, &i, skip, i),
           i < geometries.size())
    {
        IFloat tempvalue = value(ds, quality, i);

        using namespace boost::numeric::interval_lib;
        using namespace compare::certain;

        if (tempdistance < distance)
        {
            if (temphit == 1)
            {
                distance = tempdistance;
                val = tempvalue;
            }
            else
            {
                distance = hull(distance, tempdistance);
                val = tempvalue * temphit + hull(max(hitp - temphit, IFloat(0)),
                        min(hitp, IFloat(1) - temphit)) * val;
            }
        }
        else if (tempdistance > distance)
        {
            if (hitp == 1)
                continue;

            distance = hull(distance, tempdistance);
            val = val * hitp + hull(max(temphit - hitp, IFloat(0)),
                    min(temphit, IFloat(1) - hitp)) * tempvalue;
        }
        else
        {
            distance = hull(distance, tempdistance);
            val = hull(temphit, max(hitp - temphit, IFloat(0))) * tempvalue +
                  hull(hitp,    max(temphit - hitp, IFloat(0))) * val;
        }

        hitp = max(hitp, temphit);
    }

    return hitp * val;
}

int Scene::width() const
{
    return w;
}

int Scene::height() const
{
    return h;
}

void Scene::setWidth(int width)
{
    w = width;
}

void Scene::setHeight(int heigth)
{
    h = heigth;
}
