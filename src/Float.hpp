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

#ifndef FLOAT_HPP
#define FLOAT_HPP

#include <limits>
#include <algorithm>
#include <cfenv>
#include <boost/logic/tribool.hpp>

typedef double Float;
class IFloat;

class FloatingStatus
{
    friend class IFloat;

    protected:
        static bool prot;
        static int  stdrounding;

        FloatingStatus();

    public:
        inline void protect()
        {
            if (!prot)
            {
                prot = true;
                fesetround(stdrounding);
            }
        }

        inline void unprotect()
        {
            if (prot)
            {
                prot = false;
                fesetround(FE_UPWARD);
            }
        }

        inline bool isProtected() const
        {
            return prot;
        }
};

class IFloat
{
    protected:
        static FloatingStatus round;

        Float a, b;
        bool empty;

        friend Float lower(const IFloat &ifloat);
        friend Float upper(const IFloat &ifloat);
        friend IFloat hull(const IFloat &a, const IFloat &b);

        friend boost::logic::tribool operator< (const IFloat &a,
                                                const IFloat &b);
        friend boost::logic::tribool operator<=(const IFloat &a,
                                                const IFloat &b);
        friend boost::logic::tribool operator> (const IFloat &a,
                                                const IFloat &b);
        friend boost::logic::tribool operator>=(const IFloat &a,
                                                const IFloat &b);
        friend boost::logic::tribool operator==(const IFloat &a,
                                                const IFloat &b);
        friend boost::logic::tribool operator!=(const IFloat &a,
                                                const IFloat &b);

        friend IFloat operator+(const IFloat &a,const IFloat &b);
        friend IFloat operator-(const IFloat &a,const IFloat &b);
        friend IFloat operator*(const IFloat &a,const IFloat &b);
        friend IFloat operator/(const IFloat &a,const IFloat &b);

    public:
        inline static FloatingStatus roundStatus()
        {
            return round;
        }

        inline IFloat() : a(std::numeric_limits<Float>::quiet_NaN()),
                          b(std::numeric_limits<Float>::quiet_NaN()),
                          empty(true)
        {
        }

        inline IFloat(const Float v) : a(v), b(v), empty(false)
        {
        }

        inline IFloat(const Float m, const Float M) : a(m), b(M), empty(false)
        {
        }

        inline IFloat operator-() const
        {
            if (empty)
                return IFloat();

            return IFloat(-b, -a);
        }

};

class ProtectRounding
{
    protected:
        bool prot;

    public:
        inline ProtectRounding() : prot(IFloat::roundStatus().isProtected())
        {
            if (!prot)
                IFloat::roundStatus().protect();
        }

        inline ~ProtectRounding()
        {
            if (!prot)
                IFloat::roundStatus().unprotect();
        }
};

class Unprotectrounding
{
    protected:
        bool prot;

    public:
        inline Unprotectrounding() : prot(IFloat::roundStatus().isProtected())
        {
            if (prot)
                IFloat::roundStatus().unprotect();
        }

        inline ~Unprotectrounding()
        {
            if (prot)
                IFloat::roundStatus().protect();
        }
};

inline Float lower(const IFloat &ifloat)
{
    return ifloat.a;
}

inline Float upper(const IFloat &ifloat)
{
    return ifloat.b;
}

inline IFloat hull(const IFloat &a, const IFloat &b)
{
    if (a.empty)
        return b;
    if (b.empty)
        return a;

    return IFloat(std::min(a.a, b.a), std::max(a.b, b.b));
}

inline boost::logic::tribool operator<(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return boost::logic::indeterminate;

    if (a.b < b.a)
        return true;
    if (a.a >= b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator<=(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return boost::logic::indeterminate;

    if (a.b <= b.a)
        return true;
    if (a.a > b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator>(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return boost::logic::indeterminate;

    if (a.b > b.a)
        return true;
    if (a.a <= b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator>=(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return boost::logic::indeterminate;

    if (a.b >= b.a)
        return true;
    if (a.a < b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator==(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return boost::logic::indeterminate;

    if (a.a == a.b and a.a == b.a and b.a == b.b)
        return true;

    if (a.a > b.b or a.b < b.a)
        return false;

    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator!=(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return boost::logic::indeterminate;

    if (a.a > b.b or a.b < b.a)
        return true;

    if (a.a == a.b and a.a != b.a and b.a == b.b)
        return false;

    return boost::logic::indeterminate;
}

inline IFloat operator+(const IFloat &a,const IFloat &b)
{
    ProtectRounding pr;

    if (a.empty or b.empty)
        return IFloat();

    Float r = -a.a - b.a; // For rounding
    return IFloat(-r, a.b + b.b);
}

inline IFloat operator-(const IFloat &a,const IFloat &b)
{
    ProtectRounding pr;

    if (a.empty or b.empty)
        return IFloat();

    Float r = -a.a + b.a; // For rounding
    return IFloat(-r, a.b - b.a);
}

IFloat operator*(const IFloat &a,const IFloat &b);
IFloat operator/(const IFloat &a,const IFloat &b);

#endif
