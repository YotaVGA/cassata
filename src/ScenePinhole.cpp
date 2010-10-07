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

#include "ScenePinhole.hpp"

ScenePinhole::ScenePinhole(const QDomNode &node, Scene &scene,
                           QSharedPointer<SceneElement> &object) :
    SceneCamera(node, scene, object), o(0, 0, 0)
{
    sceneptr = &scene;
}

void ScenePinhole::initialize()
{
    int w = sceneptr->width(),
        h = sceneptr->height();

    using namespace ifloat;

    if (w > h)
    {
        S  = div<IFloat>(2, h);
        Tx = div<IFloat>(w, h);
        Ty = IFloat(1);
    }
    else
    {
        S  = div<IFloat>(2, w);
        Ty = div<IFloat>(h, w);
        Tx = IFloat(1);
    }

    q = sceneptr->defaultQuality();
}

const QColor ScenePinhole::pixel(int x, int y)
{
    IFloat value = IFloat(-INFINITY, INFINITY);
    Quality quality;
    Float w;
    bool single = false;

    for (int i = 0; i <= q.maxsubdivisionindex(); i++)
    {
        quality = q;

        for (int qsteps = 0; qsteps < quality.quality_steps(); qsteps++)
        {
            IFloat tempval = iterate(IFloat(x, x + 1), IFloat(y, y + 1),
                                     quality.subdivisions(i), quality);

            using namespace ifloat;

            w = width(tempval);

            if (w >= width(value))
                continue;

            value = tempval;

            single = sceneptr->singleValuePixel(value);
            if (single)
                goto gotvalue;

            if (w < quality.tollerance())
                goto gotvalue;

            quality.increase();
        }
    }
gotvalue:

    sceneptr->newPixelStatistics(value, quality.tollerance(), w, single);

    using namespace std;

    int qvalue = max(min((int)round(median(value * IFloat(256))), 255), 0);
    return QColor(qvalue, qvalue, qvalue);
}  

const IFloat ScenePinhole::iterate(IFloat x, IFloat y, int steps,
                                   const Quality &quality)
{
    if (steps)
    {
        IFloat value;
        std::pair<IFloat, IFloat> xp = bisect(x),
                                  yp = bisect(y);
        IFloat wx1 = IFloat(xp.first.upper())  - IFloat(xp.first.lower()),
               wx2 = IFloat(xp.second.upper()) - IFloat(xp.second.lower()),
               wy1 = IFloat(yp.first.upper())  - IFloat(yp.first.lower()),
               wy2 = IFloat(yp.second.upper()) - IFloat(yp.second.lower());
        IFloat v1 = iterate(xp.first,  yp.first,  steps - 1, quality),
               v2 = iterate(xp.first,  yp.second, steps - 1, quality),
               v3 = iterate(xp.second, yp.first,  steps - 1, quality),
               v4 = iterate(xp.second, yp.second, steps - 1, quality);

        IFloat wv1 = v1 * wx1 * wy1,
               wv2 = v2 * wx1 * wy2,
               wv3 = v3 * wx2 * wy1,
               wv4 = v4 * wx2 * wy2;
        IFloat wx = IFloat(x.upper()) - IFloat(x.lower()),
               wy = IFloat(y.upper()) - IFloat(y.lower());

        return (wv1 + wv2 + wv3 + wv4) / (wx * wy);
    }

    IVector3 d(Tx - S * x, Ty - S * y, 1);
    d.normalize();
    return sceneptr->sample(Ray(ILine(o, d)), quality);
}
