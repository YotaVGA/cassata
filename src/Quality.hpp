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

#ifndef QUALITY_HPP
#define QUALITY_HPP

class Quality
{
    public:
        int pixelmaxsteps, pixelstep,
            depthmaxsteps, depthstep,
            matmaxsteps,   matstep;

        inline const Quality decreased() const
        {
            return Quality();
        }

        inline int maxsubdivisionindex() const
        {
            return 8;
        }

        inline int subdivisions(int subindex) const
        {
            return 2 * subindex;
        }

        inline Float steptollerance() const
        {
            return 0/*ifloat::div<IFloat>(1,
                    (1 << 2 * (totalsteps - steps)) * 2 * 256).lower()*/;
        }

        inline bool stopIteration() const
        {
            return false;
        }

        inline bool increase()
        {
            return false;
        }
        
        inline int quality_steps() const
        {
            return 1;
        }

        inline Float tollerance() const
        {
            return ifloat::div<IFloat>(1, 2 * 256).lower();
        }

        inline int maxdepth() const
        {
            return 1;
        }

        inline int reflectancemaxsteps() const
        {
            return 2 * (pixelmaxsteps - pixelstep);
        }


        inline Float reflectancetollerance() const
        {
            return 0;
            /*return ifloat::div<IFloat>(1,
                    (1 << (2 * (totalsteps - step))) * 2 * 256).lower();*/
        }
};

#endif
