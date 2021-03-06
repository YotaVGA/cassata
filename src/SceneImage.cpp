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

#include "SceneImage.hpp"

void SceneImage::construct(const QDomNode &node, Scene &scene,
                           QSharedPointer<SceneElement> &object)
{
    SceneElement::construct(node, scene, object);
    
    for (QDomNode i = node.firstChild(); !i.isNull(); i = i.nextSibling())
    {
        if (!i.isElement())
            continue;

        QDomElement elem = i.toElement();
        QString name = elem.tagName();

        bool valid;
        int value = elem.text().toInt(&valid);

        if (name == "width")
        {
            if (!valid)
                throw QString("Error in %1.%2: Image width do not valid").
                    arg(i.lineNumber()).arg(i.columnNumber());
            scene.setWidth(value);
        }
        else if (name == "height")
        {
            if (!valid)
                throw QString("Error in %1.%2: Image height do not valid").
                    arg(i.lineNumber()).arg(i.columnNumber());
            scene.setHeight(value);
        }
    }
}
