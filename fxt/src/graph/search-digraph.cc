// This file is part of the FXT library.
// Copyright (C) 2010, 2012 Joerg Arndt
// License: GNU General Public License version 3 or later,
// see the file COPYING.txt in the main directory.

#include "graph/digraph-paths.h"

#include "fxttypes.h"
#include "fxtio.h"

//#include "jjassert.h"


//#define TRACE_FUNCALL  // define to print out calls


ulong
digraph_paths::all_paths(ulong (*pfunc)(const digraph_paths &),
                         ulong ns/*=0*/, ulong p/*=0*/, ulong maxnp/*=0*/)
// pfunc: function to visit (process) paths
// ns: start at node index ns (for fixing start of path)
// p: start at node value p   (for fixing start of path)
// maxnp: stop if maxnp paths were found
{
#ifdef TRACE_FUNCALL  // debug
    cout << "AP:  ns=" << ns << endl;
    cout << "AP:  p=" << p << endl;
    cout << "AP:  maxnp=" << maxnp << endl;

    cout << "rv[] :";
    for (ulong k=0; k<ng_; ++k)  cout << " " << setw(2) << rv_[k];
    cout << endl;
    cout << "   k :";
    for (ulong k=0; k<ng_; ++k)  cout << " " << setw(2) << k;
    cout << endl;
    cout << "qq[] :";
    for (ulong k=0; k<ng_; ++k)  cout << " " << setw(2) << qq_[k];
    cout << endl;
#endif

    pct_ = 0;
    cct_ = 0;
    pfct_ = 0;
    pfunc_ = pfunc;
    pfdone_ = 0;
    maxnp_ = maxnp;
    next_path(ns, p);
    return pfct_;  // Number of paths where pfunc() returned true
}
// -------------------------


void
digraph_paths::next_path(ulong ns, ulong p)
// ns+1 == how many nodes seen
// p == position (node we are on)
{
    if ( pfdone_ )  return;

    rv_[ns] = p;  // record position
    ++ns;

#ifdef TRACE_FUNCALL  // debug
    for (ulong k=0; k<ns; ++k)  cout << rv_[k] << " ";
    cout << endl;
#endif

    if ( ns==ng_ )  // all nodes seen ?
    {
        ++pct_;
        cq_ = path_is_cycle();
        if ( cq_ )  ++cct_;
        ulong pq = pfunc_(*this);
        if ( pq )
        {
            ++pfct_;
            if ( maxnp_ && ( pfct_>=maxnp_ ) )  pfdone_ = true;
        }
    }
    else
    {
        qq_[p] = 1;  // mark position as seen (else loops lead to errors)
        ulong fe, en;
        g_.get_edge_idx(p, fe, en);
        ulong fct = 0;  // count free reachable nodes
        for (ulong ep=fe; ep<en; ++ep)
        {
            ulong t = g_.e_[ep];  // next node
            if ( 0==qq_[t] )  // node free?
            {
                ++fct;
                qq_[p] = fct;  // mark position as seen: record turns
//                jjassert( fct>=1 );
                next_path(ns, t);
            }
        }
        // if ( 0==fct )  { "dead end: this is a U-turn"; }

        qq_[p] = 0;  // unmark position
    }
}
// -------------------------

