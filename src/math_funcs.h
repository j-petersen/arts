/* Copyright (C) 2000 Patrick Eriksson <patrick@rss.chalmers.se>

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



////////////////////////////////////////////////////////////////////////////
//   File description
////////////////////////////////////////////////////////////////////////////
/**
   \file   math_funcs.h

   Contains declerations of basic mathematical and vector/matrix functions.

   Example on types of functions:
   \begin{enumerate}
    \item Element-wise application of common scalar functions
    \item Boolean functions                         
    \item Creation of common vectors                
    \item Interpolation routines                    
    \item Integration routines                      
    \item Conversion between vector and matrix types
   \end{enumerate}

   \author Patrick Eriksson
   \date 2000-09-18 
*/



#ifndef math_funcs_h
#define math_funcs_h

////////////////////////////////////////////////////////////////////////////
//   External declarations
////////////////////////////////////////////////////////////////////////////

#include "vecmat.h"



////////////////////////////////////////////////////////////////////////////
//   Basic mathematical vector and vector functions
////////////////////////////////////////////////////////////////////////////

// //// sqrt //////////////////////////////////////////////////////////////////

// void sqrt( const VECTOR& x, VECTOR& y );

// VECTOR sqrt( const VECTOR& x );



// //// exp ///////////////////////////////////////////////////////////////////

// void exp( const VECTOR& x, VECTOR& y );

// VECTOR exp( const VECTOR& x );

// void exp( const MATRIX& X, MATRIX& Y );

// MATRIX exp( const MATRIX& X );



// //// log ////////////////////////////////////////////////////////////////////

// void log( const VECTOR& x, VECTOR& y );

// VECTOR log( const VECTOR& x );

// void log( const MATRIX& X, MATRIX& Y );

// MATRIX log( const MATRIX& X );

void log10( VECTOR& y, const VECTOR& x );

VECTOR log10( const VECTOR& x );

void log10( MATRIX& Y, const MATRIX& X );

MATRIX log10( const MATRIX& X );



//// mean and standard deviation ////////////////////////////////////////////

void mean_row( VECTOR& m, const MATRIX& x );

void std_row( VECTOR& s, const MATRIX& x, const VECTOR& m );



//// min and max ////////////////////////////////////////////////////////////

// Max and min are not needed due to MTL builtin functions.

// Numeric min( const VECTOR& x );

// Numeric min( const MATRIX& A );

// Numeric max( const VECTOR& x );

// Numeric max( const MATRIX& A );


/* Gives the maximum value of an array.

    Because this is a template function, the definition has to be also 
    in the header file, and not in file math_func.cc.

    \return      the maximum value of x
    \param   x   an array

    \author Stefan Buehler
    \date   2000-06-27
*/
// template<class T>
// T max( const ARRAY<T>& x )
// {
//   size_t n = x.size();
//   T y=x[0];
//   for ( size_t i=1; i<n; i++ )
//     {
//       if ( x[i] > y )
// 	y = x[i];
//     }
//   return y; 
// }

/* Gives the minimum value of an array.

    Because this is a template function, the definition has to be also 
    in the header file, and not in file math_func.cc.

    \return      the minimum value of x
    \param   x   an array

    \author Stefan Buehler
    \date   2000-06-27
*/
// template<class T>
// T min( const ARRAY<T>& x )
// {
//   size_t n = x.size();
//   T y=x(1);
//   for ( size_t i=2; i<=n; i++ )
//   {
//     if ( x(i) < y )
//       y = x(i);
//   }
//   return y; 
// }



//// first and last /////////////////////////////////////////////////////////

Numeric first( const VECTOR& x );

Numeric last( const VECTOR& x );



////////////////////////////////////////////////////////////////////////////
//   Logical functions
////////////////////////////////////////////////////////////////////////////

bool any( const ARRAYofsizet& x ); 



////////////////////////////////////////////////////////////////////////////
// Functions to generate vectors
////////////////////////////////////////////////////////////////////////////

void linspace(                      
              VECTOR&     x,           
        const Numeric  start,    
        const Numeric  stop,        
        const Numeric  step );

VECTOR linspace(             
        const Numeric  start, 
        const Numeric  stop,  
        const Numeric  step );

void nlinspace(         
              VECTOR&     x, 
        const Numeric     start,     
        const Numeric     stop,        
        const int         n );

VECTOR nlinspace(         
        const Numeric     start, 
        const Numeric     stop,  
        const int         n );

void nlogspace(         
              VECTOR&     x, 
        const Numeric     start,     
        const Numeric     stop,        
        const int         n );

VECTOR nlogspace(  
        const Numeric     start, 
        const Numeric     stop,  
        const int         n );



////////////////////////////////////////////////////////////////////////////
//   Interpolation routines
////////////////////////////////////////////////////////////////////////////

/** Local help function to check input grids.

    \date   2000-06-29
    \author Patrick Eriksson 

    Made a templat function out of this. Adapted to MTL.
    \date   2001-01-05
    \author Stefan Buehler
*/
template< class X, class XI >
int interp_check( const X&  x,        
		  const XI&  xi,
		  const size_t   n_y )
{
  size_t  n  = x.size();
  size_t  ni = xi.size();
  int     order=1;          // flag for pos. or neg. order, assume pos.

  // Determine the order, -1=decreasing and 1=increasing
  if ( x[0] > x[n-1] )
    order = -1;

  if ( n < 2 )
    throw runtime_error("Vector length for interpolation must be >= 2");

  if ( n != n_y ) 
    throw runtime_error("Sizes of input data to interpolation do not match");

  if ( (order*xi[0]<order*x[0]) || (order*xi[ni-1]>order*x[n-1]) ) 
    {
      ostringstream os;
      os << "Interpolation points must be inside the original range.\n"
	 << "Int.:  xi[0] = " << xi[0] << ", xi[ni] = " << xi[ni] << '\n'
	 << "Orig.: x[0]  = " << x[0]  << ", x[n]   = " << x[n];
      throw runtime_error(os.str());
    }

  for (size_t i=0; i<n-1; i++ )
  {
    if ( order*x[i+1] < order*x[i] ) 
      throw runtime_error("Original interpolation grid must be ordered");
  }

  for (size_t i=0; i<ni-1; i++ )
  {
    if ( order*xi[i+1] < order*xi[i] ) 
      throw runtime_error("Interpolation points must be ordered");
  }

  return order;
}

/** Multiple linear interpolation of a vector. This template version
    of the function should work for any vector-like type. It is based
    on Patrick's original interp_lin() function. This has to have a
    unique name, othervise there are conflicts with the other interp
    functions. All arguments can belong to different vector-like
    types. This should make it possible to use matrix rows or columns,
    or vector sub-ranges with this algorithm.

    The vector x specifies the points at which the data y is given. 

    The size of yi has to be the same as for xi.

    For technical reasons, the arguments can not be passed as
    references. Anyway, since copies in MTL are shallow, there is no
    penalty for passing by value.

    \retval  yi      interpolated values 
    \param   x       the x grid
    \param   y       the function to interpolate
    \param   xi      interpolation points

    \date   2001-01-05
    \author Stefan Buehler
*/
template <class YI, class X, class Y, class XI>
void interp_lin_vector( YI       yi,
			const X  x, 
			const Y  y, 
			const XI xi )
{
//    out3 << "interp_lin_vector:\n"
//         << "x, " << x.size() << ": ";
//    print_vector(out3,x);
//    out3 << "\ny, " << y.size() << ": ";
//    print_vector(out3,y);
//    out3 << "\nxi, " << xi.size() << ": ";
//    print_vector(out3,xi);
//    out3 << "\n";

  // Check grids and get order of grids
  int order = interp_check( x, xi, y.size() ); 

  size_t        i, j=0, n=xi.size();
  Numeric       w;

  assert( n==yi.size() ); 

  for ( i=0; i<n; i++ )
  {
    for( ;  order*x[j+1] < order*xi[i]; j++ ) {}
    w = (xi[i]-x[j]) / (x[j+1]-x[j]);
    yi[i] = y[j] + w * (y[j+1]-y[j]); 
  }

//    out3 << "yi, " << yi.size() << ": ";
//    print_vector(out3,yi);
//    out3 << "\n";
}      

/** Multiple linear interpolation of matrix rows. This is the template
    version of Patrick's original matrix interpolation function
    interp_lin_row. Should work with any kind of matrix / vector
    arguments. Use the trans() adapter if you want to interpolate
    matrix columns instead of rows.

    The vector x specifies the points at which the data y is given. 

    For technical reasons, the arguments can not be passed as
    references. Anyway, since copies in MTL are shallow, there is no
    penalty for passing by value.

    \retval  Yi      interpolated values (matrix)
    \param   x       the x grid (vector)
    \param   Y       the function to interpolate (matrix)
    \param   xi      interpolation points (vector)

    \date   2001-01-06
    \author Stefan Buehler
*/
template< class cYi, class cx, class cY, class cxi >
void interp_lin_matrix(    
		    cYi        Yi,
		    const cx   x, 
		    const cY   Y, 
		    const cxi  xi )
{
  // Check grids and get order of grids
  int order = interp_check( x, xi, Y.ncols() ); 

  INDEX      j=0, n=xi.size(), nrow=Y.nrows();
  Numeric    w;

  assert( nrow == Yi.nrows() );
  assert( n    == Yi.ncols() );

  for (INDEX i=0; i<n; i++ )
  {
    for( ;  order*x[j+1] < order*xi[i]; j++ ) {}
    w = (xi[i]-x[j]) / (x[j+1]-x[j]);
    for( INDEX k=0; k<nrow; k++ )
      {
	Yi(k,i) = Y(k,j) + w * (Y(k,j+1)-Y(k,j));
	// Caution: [][] indexing does not work here, because Y and Yi can
	// not only be simple matrices A but also for example
	// trans(A).

// 	out3 << "Y(" << k << "," << i << ") = " << Y(k,i) << " -- ";
// 	out3 << "YI(" << k << "," << i << ") = " << Yi(k,i) << "\n";
      }
  }

//   out3 << "interp_lin_matrix:\n"
//        << "x, " << x.size() << ": ";
//   print_vector(out3,x);
//   out3 << "\nY, " << Y.nrows() << "x" << Y.ncols() << ": ";
//   print_all_matrix(out3,Y);
//   out3 << "\nxi, " << xi.size() << ": ";
//   print_vector(out3,xi);
//   out3 << "\nYi, " << Yi.nrows() << "x" << Yi.ncols() << ": ";
//   print_all_matrix(out3,Yi);
//   out3 << "\n";
}        

void interp_lin(          
	VECTOR&        yi,
	const VECTOR&  x, 
	const VECTOR&  y, 
	const VECTOR&  xi );

VECTOR interp_lin(          
        const VECTOR&  x, 
        const VECTOR&  y, 
        const VECTOR&  xi );

Numeric interp_lin(         
        const VECTOR&  x, 
        const VECTOR&  y, 
        const Numeric  xi );

void interp_lin_row(    
              MATRIX&  Yi,
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi );

MATRIX interp_lin_row(      
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi );

void interp_lin_col(    
              MATRIX&  Yi,
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi );

MATRIX interp_lin_col(      
        const VECTOR&  x, 
        const MATRIX&  Y, 
        const VECTOR&  xi );



/////////////////////////////////////////////////////////////////////////////
//   Integration functions for vectors and matrices
//     These functions are not used for the moment
/////////////////////////////////////////////////////////////////////////////
/*
Numeric integr_lin(        
        const VECTOR&  x,  
        const VECTOR&  y );
void integr_lin(            
              Numeric&  w,
        const VECTOR&   x,  
        const VECTOR&   y ); 
void integr_lin(           
              MATRIX&  W,  
        const VECTOR&  x,  
        const MATRIX&  M );   
MATRIX integr_lin(         
        const VECTOR&  x,  
        const MATRIX&  M ); */



/////////////////////////////////////////////////////////////////////////////
//   Conversions between VECTOR and MATRIX types
/////////////////////////////////////////////////////////////////////////////

void to_matrix(MATRIX& W, const VECTOR& x);

MATRIX to_matrix(const VECTOR& x);



void to_vector(VECTOR& x, const MATRIX& W);

VECTOR to_vector(const MATRIX& W);



/////////////////////////////////////////////////////////////////////////////
//   Extraction of matrix columns and rows
//
//     x = row(i,A), row(x,i,A)
//             Generates a vector which contains row i of A.
//
//     x = col(i,A), col(x,i,A)
//             Generates a vector which contains column i of A.
//
/////////////////////////////////////////////////////////////////////////////

// Not needed anymore, this can be done easier and more efficiently
// using standard MTL functionality. (Use the copy() algorithm.)
// Example:
// MATRIX A,B;
// VECTOR x;
// copy( A[i], x );    // Copies row i to x.
// copy( columns(A)[i], x );    // Copies column i to x.
// copy( A.submatrix(0,A.nrows(),0,3), B );    // Copies columns 1 to 3 to B.
//
// Note that you have to make sure yourself that the dimensions
// match. You can also use copy in the other direction, hence
// put_in_col is also obsolete.

// void row(VECTOR& x,
// 	 size_t i,
// 	 const MATRIX& A);

// VECTOR row(size_t i,
// 	   const MATRIX& A);

// void col(VECTOR& x,
// 	 size_t i,
// 	 const MATRIX& A);

// VECTOR col(size_t i,
// 	   const MATRIX& A);

// void row(MATRIX& X,
// 	 size_t i,
// 	 size_t k,
// 	 const MATRIX& A);

// MATRIX row(size_t i,
// 	   size_t k,
// 	   const MATRIX& A);

// void col(MATRIX& X,
// 	 size_t i,
// 	 size_t k,
// 	 const MATRIX& A);

// MATRIX col(size_t i,
// 	   size_t k,
// 	   const MATRIX& A);



/////////////////////////////////////////////////////////////////////////////
//   Putting data in a matrix column or and row
/////////////////////////////////////////////////////////////////////////////

// Obsolete (see comment above).

// void put_in_col(
//               MATRIX& A,
// 	      size_t  i, 
//         const VECTOR& x );



/////////////////////////////////////////////////////////////////////////////
//   Random data
/////////////////////////////////////////////////////////////////////////////


// Template below.
// void rand_uniform(
//               VECTOR&    r,
//         const Numeric&   x_low,
//         const Numeric&   x_high );

// Template below.
// void rand_gaussian(
//               VECTOR&    r,
//         const Numeric&   s );

void rand_matrix_uniform(
              MATRIX&    m,
        const Numeric&   x_low,
        const Numeric&   x_high );

void rand_matrix_gaussian(
              MATRIX&    r,
        const Numeric&   s );

void rand_data_gaussian(
              MATRIX&    z,
        const VECTOR&    z0,
        const MATRIX&    s );


/**
   Creates a vector with random data uniformerly distributed between
   the lower and higher limit given.

   The random data is uncorrelated. The length of the random vector is
   taken from r.size().

   \retval   r          random vector
   \param    x_low      lower limit for the random values
   \param    x_high     upper limit for the random data

   \author Patrick Eriksson
   \date   2000-11-26

   Adapted to MTL. Gone from 1-based to 0-based. No resize for
   non-return versions. Now a template function, should work for any
   vector-like type (e.g., matrix rows).
   \date 2000-12-25
   \author Stefan Buehler
*/
template<class T>
void rand_uniform(
		  T     r,
		  const Numeric&   x_low,
		  const Numeric&   x_high )
{
  const Numeric dx = x_high-x_low;

  //  for (INDEX i=0; i<1e6; ++i)

  // Init seed in a "random" way
  //  srand( (unsigned int) time( NULL ) );
  // FIXME: This is no good, because if you call rand_uniform
  // repeatedly in quick succession, the random seed is always
  // initialized to the same value. You can verify this if you
  // uncomment the delay loop above. With the delay loop you get
  // random data for rand_matrix_uniform, but without the delay loop,
  // every row gets the same random data. :-(
  // I think the random seem should be set only once at the start of
  // ARTS. Or maybe there should be a WS method to set the seed.

  for ( size_t i=0; i<r.size(); i++ )
    {
      r[i] = x_low + dx* (double(rand())/double(RAND_MAX));
      // FIXME: Why is double used explicitly here?
      //      cout << "r[" << i << "]: " << r[i] << "\n";
    }
}

/**
   Creates a gaussian random vector with zero mean and 
   the standard deviation given.

   The random data is uncorrelated. The length of the random vector to
   generate is taken from r.size().

   The algorith is taken from Numerical Recipies, Section 7.2. 
   See www.nr.com.

   \retval   r          random vector
   \param    s          standard deviation

   \author Patrick Eriksson
   \date   2000-11-27

   Adapted to MTL. Gone from 1-based to 0-based. No resize for
   non-return versions. Now a template function, should work for any
   vector-like type (e.g., matrix rows).
   \date 2000-12-25
   \author Stefan Buehler
*/
template<class T>
void rand_gaussian(
		   T     r,
		   const Numeric&   s )
{
  VECTOR  z(2);    // A vector of length 2 with uniform PDF between -1 and 1
  Numeric rad;  // The radius cooresponding to z
  Numeric fac;  // Normalisation factor
 
  const INDEX n = r.size();

  for ( size_t i=0; i<n; )
  {
    rand_uniform( z, -1, 1 );
    rad = z[0]*z[0] + z[1]*z[1];
    // FIXME: I don't understand this algorithm. Shouldn't there be a
    // sqrt here? Otherwise I don't understand the if clause in the
    // next line. -- SAB
    if ( (rad<1) && (rad>0) )
    {
      fac = sqrt( -2*log(rad)/rad );
      i++;
      r[i] = s*fac*z[0];
      i++;
      if ( i < n )
        r[i] = s*fac*z[1];        
    }
  }
}


#endif  // math_funcs_h
