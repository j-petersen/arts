/* Copyright (C) 2002 Stefan Buehler <sbuehler@uni-bremen.de>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

/*!
  \file   interpolation.h
  \author Stefan Buehler <sbuehler@uni-bremen.de>
  \date   Fri May  3 08:54:45 2002
  
  \brief  Header file for interpolation.cc.
*/

#ifndef interpolation_h
#define interpolation_h

#include "arts.h"
#include "matpackVII.h"

//! Structure to store a grid position.
/*! 
  A grid position specifies, where an interpolation point is, relative
  to the original grid. It consists of three parts, an Index giving the
  original grid index below the interpolation point, a Numeric
  giving the fractional distance to the next original grid point, and a
  Numeric giving 1 minus this number. Of course, the last element is
  redundant. However, it is efficient to store this, since it is used
  many times over. We store the two Numerics in a plain C array of
  dimension 2. (No need to use fancy Array or Vector for this, since
  the dimension is fixed.)

  For example, idx=3 and fd=.5 means that this interpolation point is
  half-way between index 3 and 4 of the original grid.

  Note, that below in the first paragraph means "with a lower
  index". If the original grid is sorted in descending order, the
  value at the grid point below the interpolation point will be
  numerically higher than the interpolation point.

  In other words, grid positions and fractional distances are defined
  relative to the order of the original grid. Examples:

  old grid = 2 3
  new grid = 2.25
  idx      = 0
  fd[0]    = 0.25

  old grid = 3 2
  new grid = 2.25
  idx      = 0
  fd[0]    = 0.75

  Note that fd[0] is different in the second case, because the old grid
  is sorted in descending order. Note also that idx is the same in
  both cases.

  Grid positions for a whole new grid are stored in an Array<GridPos>
  (called ArrayOfGridPos). 
*/
struct GridPos {
   Index   idx;			/*!< Original grid index below interpolation point. */
   Numeric fd[2];		/*!< Fractional distance to next point
				  (0<=fd[0]<=1), fd[1] = 1-fd[0]. */ 
};

//! An Array of grid positions.
/*! 
  See \ref GridPos for details.
*/

typedef Array<GridPos> ArrayOfGridPos;

// Function headers (documentation is in .cc file):

std::ostream& operator<<(std::ostream& os, const GridPos& gp);

void gridpos( ArrayOfGridPos& gp,
              ConstVectorView old_grid,
              ConstVectorView new_grid );

////////////////////////////////////////////////////////////////////////////
//			Blue interpolation
////////////////////////////////////////////////////////////////////////////

void interpweights( MatrixView itw,
               	    const ArrayOfGridPos& cgp );

void interpweights( MatrixView itw,
               	    const ArrayOfGridPos& rgp,
               	    const ArrayOfGridPos& cgp );

void interpweights( MatrixView itw,
               	    const ArrayOfGridPos& pgp,
               	    const ArrayOfGridPos& rgp,
               	    const ArrayOfGridPos& cgp );

void interpweights( MatrixView itw,
               	    const ArrayOfGridPos& vgp,
               	    const ArrayOfGridPos& sgp,
               	    const ArrayOfGridPos& bgp,
               	    const ArrayOfGridPos& pgp,
               	    const ArrayOfGridPos& rgp,
               	    const ArrayOfGridPos& cgp );

void interp( VectorView      	   ia,
             ConstMatrixView 	   itw,
             ConstVectorView 	   a,    
             const ArrayOfGridPos& cgp);

void interp( VectorView      	   ia,
             ConstMatrixView 	   itw,
             ConstMatrixView 	   a,    
             const ArrayOfGridPos& rgp,
             const ArrayOfGridPos& cgp);

void interp( VectorView      	   ia,
             ConstMatrixView 	   itw,
             ConstTensor3View 	   a,    
	     const ArrayOfGridPos& pgp,
             const ArrayOfGridPos& rgp,
             const ArrayOfGridPos& cgp);

void interp( VectorView      	   ia,
             ConstMatrixView 	   itw,
             ConstTensor4View 	   a,    
	     const ArrayOfGridPos& bgp,
	     const ArrayOfGridPos& pgp,
             const ArrayOfGridPos& rgp,
             const ArrayOfGridPos& cgp);

void interp( VectorView      	   ia,
             ConstMatrixView 	   itw,
             ConstTensor5View 	   a,    
	     const ArrayOfGridPos& sgp,
	     const ArrayOfGridPos& bgp,
	     const ArrayOfGridPos& pgp,
             const ArrayOfGridPos& rgp,
             const ArrayOfGridPos& cgp);

void interp( VectorView      	   ia,
             ConstMatrixView 	   itw,
             ConstTensor6View 	   a,    
	     const ArrayOfGridPos& vgp,
	     const ArrayOfGridPos& sgp,
	     const ArrayOfGridPos& bgp,
	     const ArrayOfGridPos& pgp,
             const ArrayOfGridPos& rgp,
             const ArrayOfGridPos& cgp);

////////////////////////////////////////////////////////////////////////////
//			Green interpolation
////////////////////////////////////////////////////////////////////////////

void interpweights( Tensor3View itw,
               	    const ArrayOfGridPos& rgp,
               	    const ArrayOfGridPos& cgp );

void interp( MatrixView       	   ia,
             ConstTensor3View 	   itw,
             ConstMatrixView  	   a,   
	     const ArrayOfGridPos& rgp,
             const ArrayOfGridPos& cgp);


#endif // interpolation_h
