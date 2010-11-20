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
        
        friend std::pair<IFloat, IFloat> bisect(IFloat &ifloat);
        friend std::pair<std::pair<IFloat, IFloat>, std::pair<IFloat, IFloat> >
                                         weighted_bisect(IFloat &ifloat);
        friend IFloat weighted_sum(IFloat &wa, IFloat &a,
                                   IFloat &wb, IFloat &b);

        friend IFloat fmod        (const IFloat &a, const IFloat &b);
        friend IFloat exp         (const IFloat &ifloat);
        friend IFloat expm1       (const IFloat &ifloat);
        friend IFloat log         (const IFloat &ifloat);
        friend IFloat log1p       (const IFloat &ifloat);
        friend IFloat log2        (const IFloat &ifloat);
        friend IFloat log10       (const IFloat &ifloat);
        friend IFloat sin         (const IFloat &ifloat);
        friend IFloat cos         (const IFloat &ifloat);
        friend std::pair<IFloat, IFloat>
                      sincos      (const IFloat &ifloat);
        friend IFloat tan         (const IFloat &ifloat);
        friend IFloat asin        (const IFloat &ifloat);
        friend IFloat acos        (const IFloat &ifloat);
        friend IFloat atan        (const IFloat &ifloat);
        friend IFloat sinh        (const IFloat &ifloat);
        friend IFloat cosh        (const IFloat &ifloat);
        friend IFloat sinpi       (const IFloat &ifloat);
        friend IFloat cospi       (const IFloat &ifloat);
        friend std::pair<IFloat, IFloat>
                      sincospi    (const IFloat &ifloat);
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
        static const IFloat pos, neg, all;
        static const IFloat half_pi, pi, twice_pi;
        static const IFloat e;

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
        boost::logic::tribool contains(const IFloat &ifloat) const;
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

inline IFloat nroot(const IFloat &ifloat, int n)
{
    if (ifloat.isempty)
        return ifloat;

    if (n == 0)
        return IFloat::one;

    if (n < 0)
        return 1 / nroot(ifloat, -n);

    ProtectRounding pr;

    if (n % 2)
    {
        if (ifloat >= 0)
            return pow(ifloat, 1. / n);

        if (ifloat <= 0)
            return -pow(ifloat, 1. / n);

        return hull(-pow(IFloat(+0., -lower(ifloat)), 1. / n),
                     pow(IFloat(+0.,  upper(ifloat)), 1. / n));
    }

    IFloat range = intersect(ifloat, IFloat::pos);

    return pow(range, 1 / 2.);
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

inline std::pair<IFloat, IFloat> bisect(IFloat &ifloat)
{
    if (ifloat.isempty)
        return std::pair<IFloat, IFloat>(IFloat::empty, IFloat::empty);

    Float m = median(ifloat);
    return std::pair<IFloat, IFloat>(IFloat(lower(ifloat), m),
                                     IFloat(m, upper(ifloat)));
}

inline std::pair<std::pair<IFloat, IFloat>, std::pair<IFloat, IFloat> >
                                 weighted_bisect(IFloat &ifloat)
{
    if (ifloat.isempty)
        return std::pair<std::pair<IFloat, IFloat>, std::pair<IFloat, IFloat> >
                    (std::pair<IFloat, IFloat>(IFloat::empty, IFloat::empty),
                     std::pair<IFloat, IFloat>(IFloat::empty, IFloat::empty));

    ProtectRounding pr;

    Float m   = medianup(ifloat);
    IFloat a  = IFloat(lower(ifloat), m),
           b  = IFloat(m, upper(ifloat));
    IFloat wa = IFloat(upper(a)) - IFloat(lower(a)),
           wb = IFloat(upper(b)) - IFloat(lower(b));
    Float  w  = upper(wa + wb);

    return std::pair<std::pair<IFloat, IFloat>, std::pair<IFloat, IFloat> >
        (std::pair<IFloat, IFloat>(a, wa / w),
         std::pair<IFloat, IFloat>(b, wb / w));

}

inline IFloat weighted_sum(IFloat &wa, IFloat &a,
                           IFloat &wb, IFloat &b)
{
    ProtectRounding pr;

    return wa * a + wb * b;
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

inline IFloat exp(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(exp_rd(lower(ifloat)), exp_ru(upper(ifloat)));
}

inline IFloat expm1(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(expm1_rd(lower(ifloat)), expm1_ru(upper(ifloat)));
}

inline IFloat log(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat domain = intersect(IFloat::pos, ifloat);

    if (domain.isempty)
        return IFloat::empty;

    return IFloat(log_rd(lower(domain)), log_ru(upper(domain)));
}

inline IFloat log1p(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat domain = intersect(IFloat(-1, upper(IFloat::pos)), ifloat);

    if (domain.isempty)
        return IFloat::empty;

    return IFloat(log1p_rd(lower(domain)), log1p_ru(upper(domain)));
}

inline IFloat log2(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat domain = intersect(IFloat::pos, ifloat);

    if (domain.isempty)
        return IFloat::empty;

    return IFloat(log2_rd(lower(domain)), log2_ru(upper(domain)));
}

inline IFloat log10(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat domain = intersect(IFloat::pos, ifloat);

    if (domain.isempty)
        return IFloat::empty;

    return IFloat(log10_rd(lower(domain)), log10_ru(upper(domain)));
}

inline IFloat sin(const IFloat &ifloat)
{
    /* This can be a little more fast and precise with a direct
     * implementation
     */

    return cos(IFloat::half_pi - ifloat);
}

IFloat cos(const IFloat &ifloat);

inline std::pair<IFloat, IFloat> sincos(const IFloat &ifloat)
{
    /* This can be a lot more faster with a direct implementation */
    return std::pair<IFloat, IFloat>(sin(ifloat), cos(ifloat));
}

IFloat tan(const IFloat &ifloat);

inline IFloat asin(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat range = intersect(ifloat, IFloat(-1, 1));

    if (range.isempty)
        return IFloat::empty;

    return IFloat(asin_rd(lower(range)), asin_ru(upper(range)));
}

inline IFloat acos(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat range = intersect(ifloat, IFloat(-1, 1));

    if (range.isempty)
        return IFloat::empty;

    return IFloat(acos_rd(upper(range)), acos_ru(lower(range)));
}

inline IFloat atan(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(atan_rd(upper(ifloat)), atan_ru(lower(ifloat)));
}

inline IFloat sinh(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(sinh_rd(lower(ifloat)), sinh_ru(upper(ifloat)));
}

inline IFloat cosh(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    IFloat param = abs(ifloat);

    return IFloat(cosh_rd(lower(param)), cosh_ru(upper(param)));
}

inline IFloat sinpi(const IFloat &ifloat)
{
    /* This can be a little more fast and precise with a direct
     * implementation
     */

    return cospi(1 / 2. - ifloat);
}

IFloat cospi(const IFloat &ifloat);

inline std::pair<IFloat, IFloat> sincospi(const IFloat &ifloat)
{
    /* This can be a lot more faster with a direct implementation */
    return std::pair<IFloat, IFloat>(sinpi(ifloat), cospi(ifloat));
}

IFloat tanpi(const IFloat &ifloat);

inline IFloat atanpi(const IFloat &ifloat)
{
    if (ifloat.isempty)
        return ifloat;

    return IFloat(atanpi_rd(upper(ifloat)), atanpi_ru(lower(ifloat)));
}

inline IFloat sqrt(const IFloat &ifloat)
{
    /* This is ad imprecise and slow working stub */

    if (ifloat.isempty)
        return ifloat;

    IFloat range = intersect(ifloat, IFloat::pos);

    return pow(range, 1 / 2.);
}

inline IFloat cbrt(const IFloat &ifloat)
{
    /* This is ad imprecise and slow working stub */

    if (ifloat.isempty)
        return ifloat;

    if (ifloat >= 0)
        return pow(ifloat, 1 / 2.);

    if (ifloat <= 0)
        return -pow(ifloat, 1 / 2.);

    return hull( pow(IFloat(+0., -lower(ifloat)), 1 / 2.),
                -pow(IFloat(+0.,  upper(ifloat)), 1 / 2.));
}

/* a must be positive.
 * If you want to use a negative and b integer, you must to use the other pow
 */
inline IFloat pow(const IFloat &a, const IFloat &b)
{
    /* This is a working stub. The implementation is imprecise and slow */

    if (a == IFloat::one)
        return IFloat::one;

    if (b == IFloat::zero)
        return IFloat::one;

    if (a == -IFloat::one and abs(b) == upper(IFloat::pos))
        return IFloat::one;

    if (abs(a) < 1 and b == lower(IFloat::neg))
        return IFloat::one;

    if (abs(a) > 1 and b == lower(IFloat::neg))
        return IFloat::zero;

    if (abs(a) < 1 and b == upper(IFloat::pos))
        return IFloat::zero;

    if (abs(a) > 1 and b == upper(IFloat::pos))
        return IFloat::one;

    if (a == upper(IFloat::pos) and b < 0)
        return abs(IFloat(0));

    if (a == upper(IFloat::pos) and b > 0)
        return -abs(IFloat(0));

    if (a.isempty or b.isempty)
        return IFloat::empty;

    ProtectRounding pr;

    IFloat range = intersect(a, IFloat::pos);

    return exp(b * log(range));
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

inline boost::logic::tribool IFloat::contains(const IFloat &ifloat) const
{
    if (isempty)
        return false;

    if (b < ifloat.a or a > ifloat.b)
        return false;

    if (a > ifloat.a or b < ifloat.b)
        return boost::logic::indeterminate;

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
