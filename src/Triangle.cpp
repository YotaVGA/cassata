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

#include "Triangle.hpp"

Triangle::Triangle(Scene &scene, IVector3 pa, IVector3 pb, IVector3 pc) :
    Geometry(scene), a(pa), b(pb), c(pc), plane(IPlane::Through(pa, pb, pc))
{
    matinv.col(0) = a;
    matinv.col(1) = b;
    matinv.col(2) = c;

    matinv = matinv.inverse();
}

const IFloat Triangle::hit(const Ray &ray, IFloat *distance,
        DifferentialSpace *ds) const
{
    bool certain = true;

    *distance = ILine(ray.line()).intersection(plane);

    using namespace ifloat;
    using namespace ilogic;
    using namespace compare::tribool;

    tribool ba = *distance < 0,
            bb = *distance > ray.lenght();
    if (ba or bb)
        return 0;
    if (indeterminate(ba) or indeterminate(bb))
        certain = false;

    IVector3 p = ray.line().origin() + *distance * ray.line().direction();
    IVector3 b;
    IVector3 barycentric = matinv * p;

    for (int i = 0; i < 3; i++)
    {
        ba = barycentric[i] <= IFloat(0);
        bb = barycentric[i] >= IFloat(1);
        if (ba or bb)
            return 0;
        if (indeterminate(ba) or indeterminate(bb))
            certain = false;
    }

    *ds = DifferentialSpace(p);

    if (certain)
        return 1;
    else
        return IFloat(0, 1);
}

const IFloat Triangle::emission(const DifferentialSpace &ds,
                                const Quality &quality) const
{
    return 1;
}

const IFloat Triangle::directreflection(const DifferentialSpace &ds,
                                        const Quality &quality) const
{
    return 1;
}

const IFloat Triangle::indirectvalue(const DifferentialSpace &ds,
                                     const Quality &quality) const
{
    return 0;
}

const IFloat Triangle::value(const DifferentialSpace &ds,
                             const Quality &quality) const
{
    return emission(ds, quality);
}
