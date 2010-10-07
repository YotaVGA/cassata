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

#ifndef RAY_HPP
#define RAY_HPP

#include "Float.hpp"

class Ray
{
    protected:
        ILine  l;
        IFloat len;

    public:
        inline Ray(const ILine &line, const IFloat &lenght = INF) : l(line),
                                                                    len(lenght)
        {
        }

        inline const ILine &line() const
        {
            return l;
        }

        inline const IFloat &lenght() const
        {
            return len;
        }

        inline void setLenght(const IFloat &lenght)
        {
            len = lenght;
        }
};

#endif
