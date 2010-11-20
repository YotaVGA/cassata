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
#include <crlibm.h>
#include <boost/logic/tribool.hpp>

typedef double Float;
class IFloat;

/***********************************************
 ***** CLASS FOR FLOATING ROUNDING CONTROL *****
 ***********************************************/
class FloatingStatus
{
    friend class IFloat;

    protected:
        static int  stdrounding;
        static bool prot;
        static bool first_init;

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

/*****************************************
 ***** CLASS FOR ARITHMETIC INTERVAL *****
 *****************************************/
class IFloat
{
    protected:
        static FloatingStatus round;

        Float a, b;
        bool isempty;

        friend Float lower     (const IFloat &ifloat);
        friend Float upper     (const IFloat &ifloat);
        friend Float median    (const IFloat &ifloat);
        friend Float medianup  (const IFloat &ifloat);
        friend Float mediandown(const IFloat &ifloat);
        friend Float width     (const IFloat &ifloat);
        friend Float widthdown (const IFloat &ifloat);
        friend Float norm      (const IFloat &ifloat);

        friend IFloat halfpoint(const IFloat &ifloat);
        friend IFloat radius   (const IFloat &ifloat);

        friend IFloat min(const IFloat &a, const IFloat &b);
        friend IFloat max(const IFloat &a, const IFloat &b);
        friend IFloat abs(const IFloat &ifloat);

        friend IFloat square(const IFloat &ifloat);
        //TODO: nroot
        friend IFloat cubic (const IFloat &ifloat);
        friend IFloat pow   (const IFloat &ifloat, int n);
        friend IFloat nroot (const IFloat &ifloat, int n);

        friend std::pair<IFloat, IFloat> splitdiv(const IFloat &a,
                                                  const IFloat &b);
        friend IFloat multiplicativeInverse(const IFloat &ifloat);

        friend IFloat intersect(const IFloat &a, const IFloat &b);
        friend IFloat hull     (const IFloat &a, const IFloat &b);
        friend IFloat overlap  (const IFloat &a, const IFloat &b);

        friend IFloat trunc (const IFloat &ifloat);
        friend IFloat ceil  (const IFloat &ifloat);
        friend IFloat floor (const IFloat &ifloat);
        friend IFloat round (const IFloat &ifloat);
        friend IFloat lround(const IFloat &ifloat);

        //TODO: fmod, exp, expm1, log, log1p, log2, log10, sin, cos, tan, asin,
        //      acos, atan, sinh, cosh, sinpi, cospi, tanpi, atanpi, sqrt,
        //      cbrt, pow
        friend IFloat fmod        (const IFloat &a, const IFloat &b);
        friend IFloat exp         (const IFloat &ifloat);
        friend IFloat expm1       (const IFloat &ifloat);
        friend IFloat log         (const IFloat &ifloat);
        friend IFloat log1p       (const IFloat &ifloat);
        friend IFloat log2        (const IFloat &ifloat);
        friend IFloat log10       (const IFloat &ifloat);
        friend IFloat sin         (const IFloat &ifloat);
        friend IFloat cos         (const IFloat &ifloat);
        friend IFloat tan         (const IFloat &ifloat);
        friend IFloat asin        (const IFloat &ifloat);
        friend IFloat acos        (const IFloat &ifloat);
        friend IFloat atan        (const IFloat &ifloat);
        friend IFloat sinh        (const IFloat &ifloat);
        friend IFloat cosh        (const IFloat &ifloat);
        friend IFloat sinpi       (const IFloat &ifloat);
        friend IFloat cospi       (const IFloat &ifloat);
        friend IFloat tanpi       (const IFloat &ifloat);
        friend IFloat atanpi      (const IFloat &ifloat);
        friend IFloat sqrt        (const IFloat &ifloat);
        friend IFloat cbrt        (const IFloat &ifloat);
        friend IFloat pow         (const IFloat &a, const IFloat &b);

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
        /* Constants */
        static const IFloat empty;
        static const IFloat zero, one;
        static const IFloat pos, neg;
        static const IFloat half_pi, pi, twice_pi;

        inline static FloatingStatus roundStatus()
        {
            return round;
        }

        inline IFloat() : a(std::numeric_limits<Float>::quiet_NaN()),
                          b(std::numeric_limits<Float>::quiet_NaN()),
                          isempty(true)
        {
        }

        inline IFloat(const Float v) : a(v), b(v), isempty(false)
        {
        }

        inline IFloat(const Float m, const Float M) : a(m), b(M), isempty(false)
        {
        }

        inline IFloat operator-() const
        {
            if (isempty)
                return *this;

            return IFloat(-b, -a);
        }

        inline IFloat operator+() const
        {
            return *this;
        }

        IFloat &operator+=(IFloat &b);
        IFloat &operator-=(IFloat &b);
        IFloat &operator*=(IFloat &b);
        IFloat &operator/=(IFloat &b);

        bool contains(const Float  &val) const;
        bool contains(const IFloat &ifloat) const;
        bool contains_zero() const;
        bool properContains(const IFloat &ifloat) const;
        bool isEmpty() const;
        bool isASingleton() const;
};

/************************************************
 ***** CLASSES FOR FAST ROUNDING MANAGEMENT *****
 ************************************************/
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

class UnprotectRounding
{
    protected:
        bool prot;

    public:
        inline UnprotectRounding() : prot(IFloat::roundStatus().isProtected())
        {
            if (prot)
                IFloat::roundStatus().unprotect();
        }

        inline ~UnprotectRounding()
        {
            if (prot)
                IFloat::roundStatus().protect();
        }
};

/*********************************************************************
 ***** INLINE ARITHMETIC INTERVAL IMPLEMENTATIONS AND PROTOTYPES *****
 *********************************************************************/
inline Float lower(const IFloat &ifloat)
{
    return ifloat.a;
}

inline Float upper(const IFloat &ifloat)
{
    return ifloat.b;
}

inline Float median(const IFloat &ifloat)
{
    UnprotectRounding upr;

    return (ifloat.b - ifloat.a) / 2;
}

inline Float medianup(const IFloat &ifloat)
{
    ProtectRounding pr;

    return (ifloat.b - ifloat.a) / 2;
}

inline Float mediandown(const IFloat &ifloat)
{
    ProtectRounding pr;

    Float r = (-ifloat.b + ifloat.a) / 2; // For rounding

    return -r;
}

inline Float width(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return 0;

    return ifloat.b - ifloat.a;
}

inline Float widthdown (const IFloat &ifloat)
{
    if (ifloat.isempty)
        return 0;

    Float r = -ifloat.b + ifloat.a; // For rounding

    return -r;
}

inline Float norm(const IFloat &ifloat)
{
    return std::max(fabs(ifloat.a), fabs(ifloat.b));
}

inline IFloat halfpoint(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    ProtectRounding pr;

    return IFloat(mediandown(ifloat), medianup(ifloat));
}

inline IFloat radius(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return 0;

    return IFloat(widthdown(ifloat), width(ifloat));
}

inline IFloat min(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

    return IFloat(std::min(a.a, b.a), std::min(a.b, b.b));
}

inline IFloat max(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

    return IFloat(std::max(a.a, b.a), std::max(a.b, b.b));
}

inline IFloat abs(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    Float a = fabs(ifloat.a),
          b = fabs(ifloat.b);
    return IFloat(std::min(a, b), std::max(a, b));
}

inline IFloat square(const IFloat &ifloat)
{
    IFloat r = abs(ifloat);
    return r * r;
}

inline IFloat cubic(const IFloat &ifloat)
{
    return square(ifloat) * ifloat;
}

inline IFloat pow(const IFloat &ifloat, int n)
{
    if (ifloat.isempty)
        return ifloat;

    if (n == 0)
        return IFloat::one;

    if (n < 0)
        return 1 / pow(ifloat, -n);

    IFloat r = IFloat::zero;
    IFloat m = ifloat;
    while (n > 0)
    {
        if (n & 1)
            r += m;
        m = square(m);

        n >>= 1;
    }

    return r;
}

std::pair<IFloat, IFloat> splitdiv(const IFloat &a, const IFloat &b);
IFloat multiplicativeInverse(const IFloat &ifloat);

inline IFloat intersect(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

    if (a < b or b < a)
        return IFloat();

    return IFloat(std::max(a.a, b.a), std::min(a.b, b.b));
}

inline IFloat hull(const IFloat &a, const IFloat &b)
{
    if (a.isempty)
        return b;

    if (b.isempty)
        return a;

    return IFloat(std::min(a.a, b.a), std::max(a.b, b.b));
}

inline IFloat overlap(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return false;

    if (a < b or b < a)
        return false;

    return true;
}

inline IFloat trunc(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(trunc(ifloat.a), trunc(ifloat.b));
}

inline IFloat ceil(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(ceil(ifloat.a), ceil(ifloat.b));
}

inline IFloat floor(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(floor(ifloat.a), floor(ifloat.b));
}

inline IFloat round(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(round(ifloat.a), round(ifloat.b));
}

inline IFloat lround(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(lround(ifloat.a), lround(ifloat.b));
}

inline IFloat fmod(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

    if (b == 0)
        return IFloat::empty;

    if (b < 0)
        return -fmod(-a, -b);

    if (!(b <= 0) and !(b >= 0))
        return hull(fmod(a, intersect(b, IFloat::pos)),
                   -fmod(-a, intersect(b, IFloat::neg)));

    Float num   = lower(a);
    Float denom = lower(num) < 0 ? lower(b) : upper(b);
    Float t = lower(num) / -denom;

    Float n1 = trunc(-t);
    Float n2 = trunc(nextafter(-t, std::numeric_limits<Float>::infinity()));

    if (n1 == n2)
        return a - n1 * b;
    return a - hull(n1, n2) * b;
}

inline boost::logic::tribool operator<(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return boost::logic::indeterminate;

    if (a.b < b.a)
        return true;
    if (a.a >= b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator<=(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return boost::logic::indeterminate;

    if (a.b <= b.a)
        return true;
    if (a.a > b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator>(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return boost::logic::indeterminate;

    if (a.b > b.a)
        return true;
    if (a.a <= b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator>=(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return boost::logic::indeterminate;

    if (a.b >= b.a)
        return true;
    if (a.a < b.b)
        return false;
    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator==(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return boost::logic::indeterminate;

    if (a.a == a.b and a.a == b.a and b.a == b.b)
        return true;

    if (a.a > b.b or a.b < b.a)
        return false;

    return boost::logic::indeterminate;
}

inline boost::logic::tribool operator!=(const IFloat &a, const IFloat &b)
{
    if (a.isempty or b.isempty)
        return boost::logic::indeterminate;

    if (a.a > b.b or a.b < b.a)
        return true;

    if (a.a == a.b and a.a != b.a and b.a == b.b)
        return false;

    return boost::logic::indeterminate;
}

inline IFloat operator+(const IFloat &a,const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

    ProtectRounding pr;

    Float r = -a.a - b.a; // For rounding
    return IFloat(-r, a.b + b.b);
}

inline IFloat operator-(const IFloat &a,const IFloat &b)
{
    if (a.isempty or b.isempty)
        return IFloat::empty;

    ProtectRounding pr;

    Float r = -a.a + b.a; // For rounding
    return IFloat(-r, a.b - b.a);
}

IFloat operator*(const IFloat &a,const IFloat &b);
IFloat operator/(const IFloat &a,const IFloat &b);

inline IFloat &IFloat::operator+=(IFloat &b)
{
    *this = *this + b;
    return *this;
}

inline IFloat &IFloat::operator-=(IFloat &b)
{
    *this = *this - b;
    return *this;
}

inline IFloat &IFloat::operator*=(IFloat &b)
{
    *this = *this * b;
    return *this;
}

inline IFloat &IFloat::operator/=(IFloat &b)
{
    *this = *this / b;
    return *this;
}

inline bool IFloat::contains(const Float  &val) const
{
    if (isempty)
        return false;

    if (a > val or b < val)
        return false;

    return true;
}

inline bool IFloat::contains(const IFloat &ifloat) const
{
    if (isempty)
        return false;

    if (a > ifloat.a or b < ifloat.b)
        return false;

    return true;
}

inline bool IFloat::contains_zero() const
{
    if (isempty)
        return false;

    if (a > 0 or b < 0)
        return false;

    return true;
}

inline bool IFloat::properContains(const IFloat &ifloat) const
{
    return contains(ifloat) and *this != ifloat;
}

inline bool IFloat::isEmpty() const
{
    return isempty;
}

inline bool IFloat::isASingleton() const
{
    return !isempty and a == b;
}

#endif
