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

#include "SceneDiffuse.hpp"

SceneDiffuse::SceneDiffuse() : emission(0), emission_set(false)
{
}

void SceneDiffuse::construct(const QDomNode &node, Scene &scene,
                             QSharedPointer<SceneElement> &object)
{
    SceneMaterial::construct(node, scene, object);

    for (QDomNode i = node.firstChild(); !i.isNull(); i = i.nextSibling())
    {
        if (!i.isElement())
            continue;

        QDomElement elem = i.toElement();
        QString name = elem.tagName();

        if (name == "emission")
        {
            if (emission_set)
                throw QString("Only an emission can be set").
                    arg(i.lineNumber()).arg(i.columnNumber());
            emission = elem.text().toDouble();
            emission_set = true;
        }
    }
}

const IFloat SceneDiffuse::value(const DifferentialSpace &ds,
                                 const Quality &quality) const
{
    return emission;
}
