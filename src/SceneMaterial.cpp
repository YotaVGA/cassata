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

#include "SceneMaterial.hpp"

void SceneMaterial::construct(const QDomNode &node, Scene &scene,
                              QSharedPointer<SceneElement> &object)
{
    SceneElement::construct(node, scene, object);
    sceneptr = &scene;
}

const IFloat SceneMaterial::value(const Ray &in, const DifferentialSpace &ds,
                                  Quality &quality) const
{
    IVector3 z(0, 0, 1);
    IVector3 normal = ds.normal();
    IVector3 axis = normal.cross(z);

    using namespace ifloat;
    IVector3 inv = -in.line().direction();
    IFloat v = emission(ds, inv);

    IFloat ref = 0;
    if (quality.depthstep)
    {
        /* TODO: Fix hul area weight */
        quality.depthstep--;
        IAngle angle(acos(normal.dot(z)), axis[0], axis[1], axis[2]);
        quality.matmaxsteps = quality.matstep = quality.reflectancemaxsteps();
        ref = itervalue(inv, ds, quality, angle, hull(IFloat(0), PI2),
                        hull(IFloat(0), PI_H));
        quality.depthstep++;
    }
    return v + ref;
}

const IFloat SceneMaterial::itervalue(const IVector3 &in,
                                      const DifferentialSpace &ds,
                                      Quality &quality, const IAngle &angle,
                                      const IFloat &phi,
                                      const IFloat &theta) const
{
    IFloat s = sin(theta);
    IVector3 out(s * cos(phi), s * sin(phi), cos(theta));
    out = angle * out;

    using namespace ifloat;
    using namespace compare::certain;
    IFloat r = reflection(ds, in, out);
    if (r == IFloat(0))
        return 0;

    IFloat v = r * IFloat(1) * -out.dot(ds.normal());
    //IFloat v = r * sceneptr->sample(Ray(ILine(ds.point(), out)), quality,
    //                                ds.object()) * -out.dot(ds.normal());

    if (!quality.matstep or width(v) <= quality.reflectancetollerance())
        return v;

    IFloat wphi   = IFloat(phi.upper())   - IFloat(phi.lower()),
           wtheta = IFloat(theta.upper()) - IFloat(theta.lower());
    using namespace ifloat::compare::certain;
    IFloat invmul = wphi * wtheta;
    if (invmul == IFloat(0))
        return v;

    IFloat value;
    std::pair<IFloat, IFloat> phip   = bisect(phi),
                              thetap = bisect(theta);
    IFloat wphi1   = IFloat(phip.first.upper())    -
                     IFloat(phip.first.lower()),
           wphi2   = IFloat(phip.second.upper())   -
                     IFloat(phip.second.lower()),
           wtheta1 = IFloat(thetap.first.upper())  -
                     IFloat(thetap.first.lower()),
           wtheta2 = IFloat(thetap.second.upper()) -
                     IFloat(thetap.second.lower());
    quality.matstep--;
    IFloat v1 = itervalue(in, ds, quality, angle, phip.first,  thetap.first),
           v2 = itervalue(in, ds, quality, angle, phip.first,  thetap.second),
           v3 = itervalue(in, ds, quality, angle, phip.second, thetap.first),
           v4 = itervalue(in, ds, quality, angle, phip.second, thetap.second);
    quality.matstep++;

    IFloat wv1 = v1 * wphi1 * wtheta1,
           wv2 = v2 * wphi1 * wtheta2,
           wv3 = v3 * wphi2 * wtheta1,
           wv4 = v4 * wphi2 * wtheta2;


    return (wv1 + wv2 + wv3 + wv4) / invmul;
}
