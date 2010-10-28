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

ScenePinhole::ScenePinhole() : o(0, 0, 0)
{
}

void ScenePinhole::initialize()
{
    int w = refscene->width(),
        h = refscene->height();

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

    q = refscene->defaultQuality();
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
            quality.pixelstep = quality.pixelmaxsteps =
                quality.subdivisions(i);
            IFloat tempval = iterate(IFloat(x, x + 1), IFloat(y, y + 1),
                                     quality);

            using namespace ifloat;

            w = width(tempval);

            if (w >= width(value))
                continue;

            value = tempval;

            single = refscene->singleValuePixel(value);
            if (single)
                goto gotvalue;

            if (w < quality.tollerance())
                goto gotvalue;

            quality.increase();
        }
    }
gotvalue:

    refscene->newPixelStatistics(value, quality.tollerance(), w, single);

    using namespace std;

    Float t = median(value * IFloat(256));
    swapRounding();
    int qvalue = max(min((int)round(t), 255), 0);
    swapRounding();
    return QColor(qvalue, qvalue, qvalue);
}

const IFloat ScenePinhole::iterate(IFloat x, IFloat y, Quality &quality)
{
    IVector3 d(Tx - S * x, Ty - S * y, -1);
    d.normalize();
    quality.depthstep = quality.depthmaxsteps = quality.maxdepth();
    IFloat s = refscene->sample(Ray(ILine(o, d)), quality);

    if (!quality.pixelstep or width(s) <= quality.steptollerance())
    {
        return s;
    }

    IFloat wx = IFloat(x.upper()) - IFloat(x.lower()),
           wy = IFloat(y.upper()) - IFloat(y.lower());

    using namespace ifloat::compare::certain;
    IFloat invmul = wx * wy;
    if (invmul == IFloat(0))
        return s;


    IFloat value;
    std::pair<IFloat, IFloat> xp = bisect(x),
                              yp = bisect(y);
    IFloat wx1 = IFloat(xp.first.upper())  - IFloat(xp.first.lower()),
           wx2 = IFloat(xp.second.upper()) - IFloat(xp.second.lower()),
           wy1 = IFloat(yp.first.upper())  - IFloat(yp.first.lower()),
           wy2 = IFloat(yp.second.upper()) - IFloat(yp.second.lower());
    quality.pixelstep--;
    IFloat v1 = iterate(xp.first,  yp.first,  quality),
           v2 = iterate(xp.first,  yp.second, quality),
           v3 = iterate(xp.second, yp.first,  quality),
           v4 = iterate(xp.second, yp.second, quality);
    quality.pixelstep++;

    IFloat wv1 = v1 * wx1 * wy1,
           wv2 = v2 * wx1 * wy2,
           wv3 = v3 * wx2 * wy1,
           wv4 = v4 * wx2 * wy2;

    return (wv1 + wv2 + wv3 + wv4) / invmul;
}
