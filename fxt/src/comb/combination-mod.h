#if !defined  HAVE_COMBINATION_MOD_H__
#define       HAVE_COMBINATION_MOD_H__
// This file is part of the FXT library.
// Copyright (C) 2010, 2012, 2014, 2019 Joerg Arndt
// License: GNU General Public License version 3 or later,
// see the file COPYING.txt in the main directory.

#include "fxttypes.h"


class combination_mod
// Combinations in strong minimal-change order.
// The set (as opposed to delta set) is generated.
// Generation via modulo steps counting.
// Obtained by a slight modification of the Eades-McKay sequence.
{
public:
    ulong *x_;  // combination: k elements 0<=x[j]<k in increasing order
    ulong *s_;  // aux: start of range for moves
    ulong *a_;  // aux: actual start position of moves
    ulong n_, k_;  // Combination (n choose k)

    combination_mod(const combination_mod&) = delete;
    combination_mod & operator = (const combination_mod&) = delete;

public:
    explicit combination_mod(ulong n, ulong k)
    {
        n_ = n;
        k_ = k;
        x_ = new ulong[k_+1];  // incl. high sentinel
        s_ = new ulong[k_+1];  // incl. high sentinel
        a_ = new ulong[k_];
        x_[k_] = n_;
        first();
    }

    ~combination_mod()
    {
        delete [] x_;
        delete [] s_;
        delete [] a_;
    }

    const ulong * data()  const  { return x_; }

    void first()
    {
        for (ulong j=0; j<k_; ++j)  x_[j] = j;
        for (ulong j=0; j<k_; ++j)  s_[j] = j;
        for (ulong j=0; j<k_; ++j)  a_[j] = x_[j];
    }

    ulong next()
    // Return position where track changed, return k with last combination
    {
        ulong j = k_;
        while ( j-- )  // loop over tracks
        {
            const ulong sj = s_[j];
            const ulong m = x_[j+1] - sj - 1;

            if ( 0!=m )  // unless range empty
            {
                ulong u = x_[j] - sj;

                // modulo moves:
//                if ( 0==(j&1) )  // EMK
                if ( 0!=(j&1) )  // mod
//                if ( (k_&1)==(j&1) )  // Fred Lunnon's mset-gray with combinations
                {
                    ++u;
                    if ( u>m )  u = 0;
                }
                else
                {
                    --u;
                    if ( u>m )  u = m;
                }
                u += sj;

                if ( u != a_[j] )  // next position != start position
                {
                    x_[j] = u;
                    s_[j+1] = u+1;
                    return j;
                }
            }
            a_[j] = x_[j];
        }

        return  k_;  // current combination is last
    }
};
// -------------------------


#endif  // !defined HAVE_COMBINATION_MOD_H__
