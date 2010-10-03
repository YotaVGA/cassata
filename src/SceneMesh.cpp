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

SceneMesh::SceneMesh(const QDomNode &node, Scene &scene)
{
    for (QDomNode i = node.firstChild(); !i.isNull(); i = i.nextSibling())
    {
        if (!i.isElement())
            continue;

        QDomElement elem = i.toElement();
        QString name = elem.tagName();

        if (name == "polygon")
        {
            for (QDomNode j = i.firstChild(); !j.isNull(); j = j.nextSibling())
            {
                if (!i.isElement())
                    continue;

                QDomElement elem = i.toElement();
                QString name = elem.tagName();

                if (name == "point")
                {
                    QStringList coords =
                        elem.text().split(' ', QString::SkipEmptyParts);
                    if (coords.size() != 3)
                        throw QString(
                                "Error in %1.%2: Only tris are supported").
                            arg(j.lineNumber()).arg(j.columnNumber());

                    QList<Float> points;
                    for (int k = 0; k < 3; k++)
                    {
                        bool valid;
                        Float value = elem.text().toDouble(&valid);

                        if (!valid)
                            throw QString("Error in %1.%2: float error").
                            arg(j.lineNumber()).arg(j.columnNumber());

                        points << value;
                    }

                    //TODO
                    //polygons << Triangle(scene, points[0], points[1],
                    //        points[2]);
                }
            }
        }
    }
}
