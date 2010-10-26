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

#include "SceneMesh.hpp"

void SceneMesh::construct(const QDomNode &node, Scene &scene,
                          QSharedPointer<SceneElement> &object)
{
    SceneElement::construct(node, scene, object);
    sceneptr = &scene;

    start = stop = scene.element("geometry")["list"].length();

    for (QDomNode i = node.firstChild(); !i.isNull(); i = i.nextSibling())
    {
        if (!i.isElement())
            continue;

        QDomElement elem = i.toElement();
        QString name = elem.tagName();

        if (name == "polygon")
        {
            QList<IVector3> points;

            for (QDomNode j = i.firstChild(); !j.isNull(); j = j.nextSibling())
            {
                if (!j.isElement())
                    continue;

                QDomElement elem = j.toElement();
                QString name = elem.tagName();
                if (name == "point")
                {
                    QStringList coords =
                        elem.text().split(' ', QString::SkipEmptyParts);
                    if (coords.size() != 3)
                        throw QString(
                                "Error in %1.%2: the point must be 3D").
                            arg(j.lineNumber()).arg(j.columnNumber());

                    Float point[3];
                    for (int k = 0; k < 3; k++)
                    {
                        bool valid;
                        Float value = coords[k].toDouble(&valid);

                        if (!valid)
                            throw QString("Error in %1.%2: float error").
                                arg(j.lineNumber()).arg(j.columnNumber());

                        point[k] = value;
                    }

                    points << IVector3(point[0], point[1], point[2]);
                }
            }

            if (points.size() != 3)
                throw QString("Error in %1.%2: only tris are supported").
                    arg(i.lineNumber()).arg(i.columnNumber());

            scene.element("geometry")["list"] <<
                QSharedPointer<QObject>(new Triangle(scene, points[0],
                                                     points[1], points[2]));
            stop++;
        }
        else if (name == "material")
        {
            if (!materialid.isEmpty())
                throw QString(
                        "Error in %1.%2: only a material can be expressed").
                    arg(i.lineNumber()).arg(i.columnNumber());

            materialid = elem.attribute("id");
            if (materialid.isEmpty())
                throw QString("Error in %1.%2: material id must be expressed").
                    arg(i.lineNumber()).arg(i.columnNumber());

            materialline   = i.lineNumber();
            materialcolumn = i.columnNumber();
        }
    }

    if (materialid.isEmpty())
        throw QString(
                "Error in %1.%2: a material must be expressed").
            arg(node.lineNumber()).arg(node.columnNumber());
}

void SceneMesh::initialize()
{
    QSharedPointer<QObject> matptr =
        sceneptr->element("material")[materialid].value(0);
    if (!matptr)
        throw QString(
                "Error in %1.%2: material %3 does not exists").
            arg(materialline).arg(materialcolumn).arg(materialid);

    QSharedPointer<SceneMaterial> material =
        qSharedPointerCast<SceneMaterial>(matptr);
    if (!matptr)
        throw QString(
                "Error in %1.%2: id %3 is not a material").
            arg(materialline).arg(materialcolumn).arg(materialid);

    for (int i = start; i < stop; i++)
        qSharedPointerCast<Geometry>(
                sceneptr->element("geometry")["list"][i])->
            setMaterial(material);
}
