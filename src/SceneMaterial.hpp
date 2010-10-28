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

#ifndef SCENEMATERIAL_HPP
#define SCENEMATERIAL_HPP

#include "SceneElement.hpp"

class SceneMaterial : public SceneElement
{
    protected:
        Scene *sceneptr;

    public:
        virtual void construct(const QDomNode &node, Scene &scene,
                               QSharedPointer<SceneElement> &object);

        virtual const IFloat emission(const DifferentialSpace &ds,
                                      const Quality &quality,
                                      const IVector3 &out) const = 0;
        virtual const IFloat reflection(const DifferentialSpace &ds,
                                        const Quality &quality,
                                        const IVector3 &in,
                                        const IVector3 &out) const = 0;

        virtual const IFloat value(const Ray &in, const DifferentialSpace &ds,
                                   const Quality &quality, int totalsteps, int step,
                                   int totaldepth, int depth) const;

    protected:
        virtual const IFloat itervalue(const IVector3 &in,
                                       const DifferentialSpace &ds,
                                       const Quality &quality,
                                       const IAngle &angle, const IFloat &phi,
                                       const IFloat &theta, int totalsteps,
                                       int step, int totaldepth, int depth,
                                       int totalsubdiv, int matstep) const;
};

#endif
