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

QHash<QString, BaseSceneRegister *> sceneregistrations;

BaseSceneRegister::~BaseSceneRegister()
{
}

Scene::Scene(const QString &filename) : scenedoc("scene")
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

        QSharedPointer<SceneElement> shader =
            shaderiterator.value()->newClass(i, *this);
        shaders << shader;
        QString name = elem.attribute("id");
        if (!name.isEmpty())
            element("names")[name] = shader;
    }

    for (int i = 0; i < shaders.size(); i++)
        shaders[i]->initialize();
}

SceneList &Scene::element(const QString &name)
{
    return lists[name];
}

const QColor Scene::pixel(int x, int y) const
{
    return Qt::white;
}
