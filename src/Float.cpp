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

#include "Float.hpp"

bool FloatingStatus::prot        = true;
int  FloatingStatus::stdrounding = fegetround();

FloatingStatus::FloatingStatus()
{
    fesetround(FE_UPWARD);
}

IFloat operator*(const IFloat &a,const IFloat &b)
{
    ProtectRounding pr;

    if (a.empty or b.empty)
        return IFloat();

    bool posa = a.b >= 0,
         posb = b.b >= 0,
         unka = a.a <  0 and a.b > 0,
         unkb = b.a <  0 and b.b > 0;

    Float r; // For rounding
    if (posa)
    {
        if (posb)
        {
            r = a.a * -b.a; // For rounding
            return IFloat(-r, a.b * b.b);
        }

        if (unkb)
        {
            r = a.b * -b.a; // For rounding
            return IFloat(-r, a.b * b.b);
        }

        r = a.b * -b.a; // For rounding
        return IFloat(-r, a.a * b.b);
    }

    if (unka)
    {
        if (posb)
        {
            r = a.a * -b.b; // For rounding
            return IFloat(-r, a.b * b.b);
        }

        if (unkb)
        {
            r = std::max(a.a * -b.b, a.b * -b.a); // For rounding
            return IFloat(-r, std::max(a.a * b.a, a.b * b.b));
        }

        r = a.b * -b.b; // For rounding
        return IFloat(-r, a.a * b.b);
    }

    if (posb)
    {
        r = a.a * -b.b; // For rounding
        return IFloat(-r, a.b * b.a);
    }

    if (unkb)
    {
        r = a.b * - b.b; // For rounding
        return IFloat(-r, a.b * b.a);
    }

    r = a.b * b.b; // For rounding
    return IFloat(-r, a.a * b.a);
}

IFloat operator/(const IFloat &a,const IFloat &b)
{
    ProtectRounding pr;

    return IFloat();
}

