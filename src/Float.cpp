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
    if (a.empty or b.empty)
        return IFloat();

    ProtectRounding pr;

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
    if (a.empty or b.empty)
        return IFloat();

    if (a <= 0 and !(b <= 0))
        return -(-a / b);
    
    if (!(a <= 0) and b <= 0)
        return -(a / -b);

    if (a <= 0 and b <= 0)
        return -a / -b;

    if (!(b >= 0))
        return IFloat(-std::numeric_limits<Float>::infinity(),
                       std::numeric_limits<Float>::infinity());

    ProtectRounding pr;

    Float r; // For rounding

    if (a >= 0)
    {
        r = a.a / -b.b; // For rounding
        return IFloat(-r, a.b / b.a);
    }

    r = a.a / -b.a; // For rounding
    return IFloat(-r, a.b / b.a);
}

std::pair<IFloat, IFloat> div(const IFloat &a, const IFloat &b)
{
    std::pair<IFloat, IFloat> r;

    if (a.empty or b.empty)
        return r;

    IFloat tf;

    if (a <= 0 and !(b <= 0))
    {
        r = div(-a, b);
        tf       =  r.first;
        r.first  = -r.second;
        r.second = -tf;
        return r;
    }
    
    if (!(a <= 0) and b <= 0)
    {
        r = div(a, -b);
        tf       =  r.first;
        r.first  = -r.second;
        r.second = -tf;
        return r;
    }

    if (a <= 0 and b <= 0)
        return div(-a, -b);

    ProtectRounding pr;

    Float t;

    if (b >= 0)
    {
        if (a >= 0)
        {
            t = a.a / -b.b; // For rounding
            r.first = IFloat(-t, a.b / b.a);
            return r;
        }

        t = a.a / -b.a; // For rounding
        r.first = IFloat(-t, a.b / b.a);
        return r;
    }

    Float inf = std::numeric_limits<Float>::infinity();

    if (a >= 0)
    {
        r.first = IFloat(-inf, a.a / b.a);

        t = a.a / -b.b; // For rounding
        r.second = IFloat(-t, inf);

        return r;
    }

    r.first = IFloat(-inf, std::max(a.a / b.b, a.b / b.a));

    t = std::max(a.a / -b.a, a.b / -b.b); // For rounding
    r.second = IFloat(-t, inf);

    return r;
}

IFloat multiplicativeInverse(const IFloat &ifloat)
{
    if (ifloat.empty)
        return IFloat();

    if (ifloat <= 0)
        return -multiplicativeInverse(-ifloat);

    if (!(ifloat >= 0))
        return IFloat(-std::numeric_limits<Float>::infinity(),
                       std::numeric_limits<Float>::infinity());

    ProtectRounding pr;

    Float r; // For rounding

    r = 1 / -ifloat.b; // For rounding
    return IFloat(-r, 1 / ifloat.a);
}
