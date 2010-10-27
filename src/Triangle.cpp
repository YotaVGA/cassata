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
    using namespace ifloat;

    Float w, wt;
    w = width(hull(hull(a.x(), b.x()), c.x()));
    i3 = 0;

    wt = width(hull(hull(a.y(), b.y()), c.y()));
    if (wt < w)
    {
        i3 = 1;
        w = wt;
    }

    wt = width(hull(hull(a.z(), b.z()), c.z()));
    if (wt < w)
    {
        i3 = 2;
        w = wt;
    }

    i1 = (i3 + 1) % 3;
    i2 = (i1 + 1) % 3;

    invmatrix(0, 0) = a[i1] - c[i1];
    invmatrix(0, 1) = b[i1] - c[i1];
    invmatrix(1, 0) = a[i2] - c[i2];
    invmatrix(1, 1) = b[i2] - c[i2];

    invmatrix = invmatrix.inverse();
}

const IFloat Triangle::hit(const Ray &ray, IFloat *distance,
        DifferentialSpace *ds) const
{
    bool certain = true;

    *distance = ILine(ray.line()).intersection(plane);

    using namespace ifloat;
    using namespace ilogic;
    using namespace compare::tribool;

    tribool ba = *distance < IFloat(0),
            bb = *distance > IFloat(ray.lenght());
    if (ba or bb)
        return 0;
    if (indeterminate(ba) or indeterminate(bb))
        certain = false;

    IVector3 p  = ray.line().origin() + *distance * ray.line().direction();
    IVector2 lp;
    lp[0] = p[i1] - c[i1];
    lp[1] = p[i2] - c[i2];
    IVector2 b  = invmatrix * lp;
    IVector3 barycentric;
    barycentric[i1] = b[0];
    barycentric[i2] = b[1];
    barycentric[i3] = IFloat(1) - b[0] - b[1];

    for (int i = 0; i < 3; i++)
    {
        ba = barycentric[i] <= IFloat(0);
        bb = barycentric[i] >= IFloat(1);
        if (ba or bb)
            return 0;
        if (indeterminate(ba) or indeterminate(bb))
            certain = false;
    }

    IVector3 normal    = plane.normal();
    tribool  invnormal = normal.dot(ray.line().direction()) < IFloat(0);
    if (invnormal)
        normal = -normal;
    else if (indeterminate(invnormal))
        normal = IVector3(hull(normal.x(), -normal.x()),
                          hull(normal.y(), -normal.y()),
                          hull(normal.z(), -normal.z()));

    *ds = DifferentialSpace(p, normal);

    if (certain)
        return 1;
    else
        return IFloat(0, 1);
}
