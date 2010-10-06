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

/* Many controls and compless intervals require more attention, but
 * this is only a (working) stub, for a rapid write of a Cassata prototype
 */

#include <QtGui>
#include <cmath>
#include <boost/numeric/interval.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <crlibm.h>

typedef double Float;

class RoundingIFloat :
    public boost::numeric::interval_lib::rounded_arith_exact<Float>
{
public:
#define ROUNDINGIFLOATFUNC(f)                           \
    inline Float f##_down(Float x) {return f##_rd(x);}  \
    inline Float f##_up  (Float x) {return f##_ru(x);}  \

    ROUNDINGIFLOATFUNC(exp);
    ROUNDINGIFLOATFUNC(log);
    ROUNDINGIFLOATFUNC(sin);
    ROUNDINGIFLOATFUNC(cos);
    ROUNDINGIFLOATFUNC(tan);
    ROUNDINGIFLOATFUNC(asin);
    ROUNDINGIFLOATFUNC(acos);
    ROUNDINGIFLOATFUNC(atan);
    ROUNDINGIFLOATFUNC(sinh);
    ROUNDINGIFLOATFUNC(cosh);

#undef ROUNDINGIFLOATFUNC
};

typedef boost::numeric::interval<Float, boost::numeric::interval_lib::
                                 policies<boost::numeric::interval_lib::
                                 save_state<RoundingIFloat>,
                                 boost::numeric::interval_lib::
                                          checking_strict<Float> > > IFloat;

#define PI   (pi<IFloat>())
#define PI2  (pi_twice<IFloat>())
#define PI_H (pi_half<IFloat>())
#define INF  (std::numeric_limits<Float>::infinity())

namespace ifloat         = boost::numeric::interval_lib;

// NOTE: This work well, but for optimizzation reasons it isn't exactly
// conformant to C99, but it can return an equivalent angle
inline IFloat atan2(const IFloat &x, const IFloat &y)
{

    IFloat angle = atan(y / x);

    using namespace ifloat;
    using namespace ifloat::compare::certain;
    
    if (x > 0.)
        return angle;
    
    if (x < 0.)
        return IFloat(angle) + PI;

    return angle + IFloat(0, PI.upper());
}

namespace Eigen
{

template<> struct NumTraits<IFloat>
{
    typedef IFloat Real;
    typedef IFloat FloatingPoint;
    enum
    {
        IsComplex = 0,
        HasFloatingPoint = 1,
        ReadCost = 1,
        AddCost = 1,
        MulCost = 1
    };
};

}

namespace boost
{
namespace numeric
{
namespace interval_lib
{

inline const IFloat &ei_conj(const IFloat &x) {return x;}
inline const IFloat &ei_real(const IFloat &x) {return x;}
inline const IFloat  ei_imag(const IFloat &x) {return 0;}
inline const IFloat  ei_abs (const IFloat &x) {return abs(x);}
inline const IFloat  ei_abs2(const IFloat &x) {return x * x;}
inline const IFloat  ei_sqrt(const IFloat &x) {return sqrt(x);}
inline const IFloat  ei_exp (const IFloat &x) {return exp(x);}
inline const IFloat  ei_log (const IFloat &x) {return log(x);}
inline const IFloat  ei_sin (const IFloat &x) {return sin(x);}
inline const IFloat  ei_cos (const IFloat &x) {return cos(x);}

}
}
}

typedef Eigen::Matrix<IFloat, 2, 1>        IVector2;
typedef Eigen::Matrix<IFloat, 3, 1>        IVector3;
typedef Eigen::Matrix<IFloat, 4, 1>        IVector4;
typedef Eigen::Matrix<IFloat, 4, 4>        IMatrix4;
typedef Eigen::ParametrizedLine<IFloat, 3> ILine;
typedef Eigen::Quaternion<IFloat>          IAngle;

#endif
