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

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Geometry.hpp"

class Triangle : public Geometry
{
    protected:
        IVector3 a, b, c;

    public:
        Triangle(Scene &scene, IVector3 pa, IVector3 pb, IVector3 pc);

        virtual const IFloat hit(const Ray &ray, IFloat *distance,
                DifferentialSpace *ds) const;
        virtual const IFloat value(const DifferentialSpace &ds) const;
};

#endif
