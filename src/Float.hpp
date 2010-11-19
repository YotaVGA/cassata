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
        bool empty;

        friend Float lower     (const IFloat &ifloat);
        friend Float upper     (const IFloat &ifloat);
        friend Float median    (const IFloat &ifloat);
        friend Float medianup  (const IFloat &ifloat);
        friend Float mediandown(const IFloat &ifloat);
        friend Float width     (const IFloat &ifloat);
        friend Float norm      (const IFloat &ifloat);

        friend IFloat halfpoint(const IFloat &ifloat);
        friend IFloat radius   (const IFloat &ifloat);

        friend IFloat min(const IFloat &a, const IFloat &b);
        friend IFloat max(const IFloat &a, const IFloat &b);
        friend IFloat abs(const IFloat &ifloat);

        friend IFloat square(const IFloat &ifloat);
        /*friend IFloat pow   (const IFloat &ifloat, int n);
        friend IFloat nroot (const IFloat &ifloat, int n);*/

        friend std::pair<IFloat, IFloat> div(const IFloat &a, const IFloat &b);
        friend IFloat multiplicativeInverse(const IFloat &ifloat);

        friend IFloat intersect(const IFloat &a, const IFloat &b);
        friend IFloat hull     (const IFloat &a, const IFloat &b);
        friend IFloat overlap  (const IFloat &a, const IFloat &b);

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
        static const IFloat zero, one;
        static const IFloat half_pi, pi, twice_pi;

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

/******************************************************
 ***** INLINE ARITHMETIC INTERVAL IMPLEMENTATIONS *****
 ******************************************************/
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
    if (ifloat.empty)
        return 0;

    return ifloat.b - ifloat.a;
}

inline Float norm(const IFloat &ifloat)
{
    return std::max(fabs(ifloat.a), fabs(ifloat.b));
}

inline IFloat halfpoint(const IFloat &ifloat)
{
    if (ifloat.empty)
        return ifloat;

    ProtectRounding pr;

    return IFloat(mediandown(ifloat), medianup(ifloat));
}

inline IFloat radius(const IFloat &ifloat)
{
    if (ifloat.empty)
        return 0;

    Float r = -(-ifloat.b + ifloat.a); // For rounding

    return IFloat(-r, width(ifloat));
}

inline IFloat min(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return IFloat();

    return IFloat(std::min(a.a, b.a), std::min(a.b, b.b));
}

inline IFloat max(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return IFloat();

    return IFloat(std::max(a.a, b.a), std::max(a.b, b.b));
}

inline IFloat abs(const IFloat &ifloat)
{
    if (ifloat.empty)
        return IFloat();

    Float a = fabs(ifloat.a),
          b = fabs(ifloat.b);
    return IFloat(std::min(a, b), std::max(a, b));
}

inline IFloat square(const IFloat &ifloat)
{
    IFloat r = abs(ifloat);
    return r * r;
}

inline IFloat intersect(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return IFloat();

    if (a < b or b < a)
        return IFloat();

    return IFloat(std::max(a.a, b.a), std::min(a.b, b.b));
}

inline IFloat hull(const IFloat &a, const IFloat &b)
{
    if (a.empty)
        return b;
    if (b.empty)
        return a;

    return IFloat(std::min(a.a, b.a), std::max(a.b, b.b));
}

inline IFloat overlap(const IFloat &a, const IFloat &b)
{
    if (a.empty or b.empty)
        return false;

    if (a < b or b < a)
        return false;

    return true;
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
    if (a.empty or b.empty)
        return IFloat();

    ProtectRounding pr;

    Float r = -a.a - b.a; // For rounding
    return IFloat(-r, a.b + b.b);
}

inline IFloat operator-(const IFloat &a,const IFloat &b)
{
    if (a.empty or b.empty)
        return IFloat();

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
    if (empty)
        return false;

    if (a > val or b < val)
        return false;

    return true;
}

inline bool IFloat::contains(const IFloat &ifloat) const
{
    if (empty)
        return false;

    if (a > ifloat.a or b < ifloat.b)
        return false;

    return true;
}

inline bool IFloat::contains_zero() const
{
    if (empty)
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
    return empty;
}

inline bool IFloat::isASingleton() const
{
    return !empty and a == b;
}

#endif
