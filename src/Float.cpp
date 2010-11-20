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

class setupConstants
{
    public:
        setupConstants()
        {
            crlibm_init();
        }

        inline IFloat half_pi() const
        {
            return IFloat(-(Float)(-acos_rd(0)), acos_ru(0));
        }

        inline IFloat pi() const
        {
            return IFloat(-(Float)(-acos_rd(-1)), acos_ru(-1));
        }

        /* The twice_pi is a working stub: that is not with the best
         * precision
         */
        inline IFloat twice_pi() const
        {
            return IFloat(-(Float)(-2 * acos_rd(-1)), 2 * acos_ru(-1));
        }

        inline IFloat e() const
        {
            return IFloat(-(Float)(-exp_rd(1)), exp_ru(1));
        }
};
static setupConstants constants;

bool FloatingStatus::prot        = true;
bool FloatingStatus::first_init  = true;
int  FloatingStatus::stdrounding = fegetround();

const IFloat IFloat::empty = IFloat();
const IFloat IFloat::zero  = 0;
const IFloat IFloat::one   = 1;

const IFloat IFloat::pos = IFloat(0, std::numeric_limits<Float>::infinity());
const IFloat IFloat::neg = IFloat(-std::numeric_limits<Float>::infinity(), 0);
const IFloat IFloat::all = IFloat(-std::numeric_limits<Float>::infinity(),
                                   std::numeric_limits<Float>::infinity());

const IFloat IFloat::half_pi  = constants.half_pi();
const IFloat IFloat::pi       = constants.pi();
const IFloat IFloat::twice_pi = constants.twice_pi();

const IFloat IFloat::e = constants.e();

FloatingStatus::FloatingStatus()
{
    if (first_init)
    {
        stdrounding = fegetround();
        fesetround(FE_UPWARD);
        first_init = false;
    }
}

IFloat operator*(const IFloat &a,const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

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
    if (a.isempty or b.isempty)
        return IFloat::empty;

    if (a <= 0 and !(b <= 0))
        return -(-a / b);
    
    if (!(a <= 0) and b <= 0)
        return -(a / -b);

    if (a <= 0 and b <= 0)
        return -a / -b;

    if (!(b >= 0))
        return IFloat(-std::numeric_limits<Float>::infinity(),
                       std::numeric_limits<Float>::infinity());

    if (b == IFloat::zero)
        return IFloat::empty;

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

std::pair<IFloat, IFloat> splitdiv(const IFloat &a, const IFloat &b)
{
    std::pair<IFloat, IFloat> r;

    if (a.isempty or b.isempty)
        return r;

    IFloat tf;

    if (a <= 0 and !(b <= 0))
    {
        r = splitdiv(-a, b);
        tf       =  r.first;
        r.first  = -r.second;
        r.second = -tf;
        return r;
    }
    
    if (!(a <= 0) and b <= 0)
    {
        r = splitdiv(a, -b);
        tf       =  r.first;
        r.first  = -r.second;
        r.second = -tf;
        return r;
    }

    if (a <= 0 and b <= 0)
        return splitdiv(-a, -b);

    if (b == IFloat::zero)
        return r;

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
    if (ifloat.isempty)
        return IFloat::empty;

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

IFloat cos(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat range = fmod(ifloat, IFloat::twice_pi);

    if (width(range) >= lower(IFloat::twice_pi))
        return IFloat(-1, 1);

    bool up;
    bool set = false;

    if (ifloat.contains_zero())
    {
        set = true;
        up = true;
    }

    if (range.contains(IFloat::pi) != false)
    {
        if (set)
            return IFloat(-1, 1);

        set = true;
        up = false;
    }

    if (range.contains(IFloat::twice_pi) != false)
    {
        if (set)
            return IFloat(-1, 1);

        set = true;
        up = true;
    }

    /* This can be more precise */
    if (range.contains(IFloat::pi + IFloat::twice_pi) != false)
        return IFloat(-1, 1);

    UnprotectRounding upr;

    if (!set)
    {
        if (upper(range) < lower(IFloat::pi))
            return IFloat(cos_rd(upper(range)), cos_ru(lower(range)));

        if (upper(range) < lower(IFloat::twice_pi))
            return IFloat(cos_rd(lower(range)), cos_ru(upper(range)));

        return IFloat(cos_rd(upper(range)), cos_ru(lower(range)));
    }

    /* Probably this part can be more fast, whit only a cosine */
    if (up)
        return IFloat(std::min(cos_rd(lower(range)), cos_rd(upper(range))), 1);

    return IFloat(-1, std::max(cos_ru(lower(range)), cos_ru(upper(range))));
}

IFloat tan(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat range = fmod(ifloat, IFloat::pi);

    if (width(range) >= lower(IFloat::pi))
        return IFloat::all;

    bool set;

    if (range.contains(IFloat::half_pi) != false)
        set = true;

    if (range.contains(IFloat::half_pi + IFloat::pi) != false)
    {
        if (set)
            return IFloat::all;
        
        set = true;
    }

    if (range.contains(IFloat::half_pi + IFloat::twice_pi) != false)
        return IFloat::all;

    UnprotectRounding upr;

    if (!set)
        return IFloat(tan_rd(lower(range)), tan_ru(upper(range)));

    /* This can be more precise if it is checked if a peak is only touched but
     * it is not passed. In this case we have only an extreme that contains an
     * infinite
     */
    return IFloat::all;
}

IFloat sinpi (const IFloat &ifloat);
IFloat cospi (const IFloat &ifloat);
IFloat tanpi (const IFloat &ifloat);
IFloat atanpi(const IFloat &ifloat);
