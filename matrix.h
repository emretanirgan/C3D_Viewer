////////////////////////////////
// Aline N - 
// Extensions for creating rotation, scale, and translation matrices
// Extensions for multiplying vec3

////////////////////////////////
// Matrix TCL Lite v1.13
// Copyright (c) 1997-2002 Techsoft Pvt. Ltd. (See License.Txt file.)
//
// Matrix.h: Matrix C++ template class include file 
// Web: http://www.techsoftpl.com/matrix/
// Email: matrix@techsoftpl.com
//

//////////////////////////////
// Note: This matrix template class defines majority of the matrix
// operations as overloaded operators or methods. It is assumed that
// users of this class is familiar with matrix algebra. We have not
// defined any specialization of this template here, so all the instances
// of matrix will be created implicitly by the compiler. The data types
// tested with this class are float, double, long double, complex<float>,
// complex<double> and complex<long double>. Note that this class is not 
// optimized for performance.
//
// Since implementation of exception, namespace and template are still
// not standardized among the various (mainly old) compilers, you may 
// encounter compilation error with some compilers. In that case remove 
// any of the above three features by defining the following macros:
//
//  _NO_NAMESPACE:  Define this macro to remove namespace support.
//
//  _NO_EXCEPTION:  Define this macro to remove exception handling
//                  and use old style of error handling using function.
//
//  _NO_TEMPLATE:   If this macro is defined matrix class of double
//                  type will be generated by default. You can also
//                  generate a different type of matrix like float.
//
//  _SGI_BROKEN_STL: For SGI C++ v.7.2.1 compiler.
//
//  Since all the definitions are also included in this header file as
//  inline function, some compiler may give warning "inline function
//  can't be expanded". You may ignore/disable this warning using compiler
//  switches. All the operators/methods defined in this class have their
//  natural meaning except the followings:
//
//  Operator/Method                          Description
//  ---------------                          -----------
//   operator ()   :   This function operator can be used as a
//                     two-dimensional subscript operator to get/set
//                     individual matrix elements.
//
//   operator !    :   This operator has been used to calculate inversion
//                     of matrix.
//
//   operator ~    :   This operator has been used to return transpose of
//                     a matrix.
//
//   operator ^    :   It is used calculate power (by a scalar) of a matrix.
//                     When using this operator in a matrix equation, care
//                     must be taken by parenthesizing it because it has
//                     lower precedence than addition, subtraction,
//                     multiplication and division operators.
//
//   operator >>   :   It is used to read matrix from input stream as per
//                     standard C++ stream operators.
//
//   operator <<   :   It is used to write matrix to output stream as per
//                     standard C++ stream operators.
//
// Note that professional version of this package, Matrix TCL Pro 2.11
// is optimized for performance and supports many more matrix operations.
// It is available from our web site at <http://www.techsoftpl.com/matrix/>.
//

#ifndef __cplusplus
#error Must use C++ for the type matrix.
#endif

#pragma warning( disable: 4290)

#if !defined(__STD_MATRIX_H)
#define __STD_MATRIX_H

//////////////////////////////
// First deal with various shortcomings and incompatibilities of
// various (mainly old) versions of popular compilers available.
//

#if defined(__BORLANDC__)
#pragma option -w-inl -w-pch
#endif

#if ( defined(__BORLANDC__) || _MSC_VER <= 1000 ) && !defined( __GNUG__ )
#  include <stdio.h>
#  include <stdlib.h>
#  include <math.h>
#  include <iostream.h>
#  include <string.h>
#else
#  include <cmath>
#  include <cstdio>
#  include <cstdlib>
#  include <string>
#  include <iostream>
#endif

#include "Transformation.h"

#if defined(_MSC_VER) && _MSC_VER <= 1000
#  define _NO_EXCEPTION        // stdexception is not fully supported in MSVC++ 4.0
typedef int bool;
#  if !defined(false)
#    define false  0
#  endif
#  if !defined(true)
#    define true   1
#  endif
#endif

#if defined(__BORLANDC__) && !defined(__WIN32__)
#  define _NO_EXCEPTION        // std exception and namespace are not fully
#  define _NO_NAMESPACE        // supported in 16-bit compiler
#endif

#if defined(_MSC_VER) && !defined(_WIN32)
#  define _NO_EXCEPTION
#endif

#if defined(_NO_EXCEPTION)
#  define _NO_THROW
#  define _THROW_MATRIX_ERROR
#else
#  if defined(_MSC_VER)
#    if _MSC_VER >= 1020
#      include <stdexcept>
#    else
#      include <stdexcpt.h>
#    endif
#  elif defined(__MWERKS__)
#      include <stdexcept>
#  elif (__GNUC__ >= 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 8))
#     include <stdexcept>
#  else
#     include <stdexcep>
#  endif
#  define _NO_THROW               throw ()
#  define _THROW_MATRIX_ERROR     throw (matrix_error)
#endif

#ifndef __MINMAX_DEFINED
#  define max(a,b)    (((a) > (b)) ? (a) : (b))
#  define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#if defined(_MSC_VER)
#undef _MSC_EXTENSIONS      // To include overloaded abs function definitions!
#endif

//#if ( defined(__BORLANDC__) || _MSC_VER ) && !defined( __GNUG__ ) 
//inline float abs (float v) { return (float)fabs( v); } 
//inline double abs (double v) { return fabs( v); } 
//inline long double abs (long double v) { return fabsl( v); }
//#endif

#if defined(__GNUG__) || defined(__MWERKS__) || (defined(__BORLANDC__) && (__BORLANDC__ >= 0x540))
#define FRIEND_FUN_TEMPLATE <>
#else
#define FRIEND_FUN_TEMPLATE
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1020   // MSVC++ 4.0/4.2 does not
#  define _NO_NAMESPACE                     // support "std" namespace
#endif

#if !defined(_NO_NAMESPACE)
#if defined( _SGI_BROKEN_STL )              // For SGI C++ v.7.2.1 compiler
namespace std { }
#endif
using namespace std;
#endif

#ifndef _NO_NAMESPACE
namespace math {
#endif

#if !defined(_NO_EXCEPTION)
class matrix_error : public logic_error
{
    public:
	matrix_error (const string& what_arg) : logic_error( what_arg) {}
};
#define REPORT_ERROR(ErrormMsg)  throw matrix_error( ErrormMsg);
#else
inline void _matrix_error (const char* pErrMsg)
{
    cout << pErrMsg << endl;
    exit(1);
}
#define REPORT_ERROR(ErrormMsg)  _matrix_error( ErrormMsg);
#endif

#if !defined(_NO_TEMPLATE)
#  define MAT_TEMPLATE  template <class T>
#  define matrixT  matrix<T>
#else
#  define MAT_TEMPLATE
#  define matrixT  matrix
#  ifdef MATRIX_TYPE
     typedef MATRIX_TYPE T;
#  else
     typedef double T;
#  endif
#endif


MAT_TEMPLATE
class matrix
{
public:
   // Constructors
   matrix (const matrixT& m);
   matrix (size_t row = 6, size_t col = 6);

   // Destructor
   ~matrix ();

   // Assignment operators
   matrixT& operator = (const matrixT& m) _NO_THROW;

   // Value extraction method
   size_t RowNo () const { return _m->Row; }
   size_t ColNo () const { return _m->Col; }

   // Subscript operator
   T& operator () (size_t row, size_t col) _THROW_MATRIX_ERROR;
   T  operator () (size_t row, size_t col) const _THROW_MATRIX_ERROR;

   // Unary operators
   matrixT operator + () _NO_THROW { return *this; }
   matrixT operator - () _NO_THROW;

   // Combined assignment - calculation operators
   matrixT& operator += (const matrixT& m) _THROW_MATRIX_ERROR;
   matrixT& operator -= (const matrixT& m) _THROW_MATRIX_ERROR;
   matrixT& operator *= (const matrixT& m) _THROW_MATRIX_ERROR;
   matrixT& operator *= (const T& c) _NO_THROW;
   matrixT& operator /= (const T& c) _NO_THROW;
   matrixT& operator ^= (const size_t& pow) _THROW_MATRIX_ERROR;

   // Miscellaneous -methods
   void Null (const size_t& row, const size_t& col) _NO_THROW;
   void Null () _NO_THROW;
   void Unit (const size_t& row) _NO_THROW;
   void Unit () _NO_THROW;
   void SetSize (size_t row, size_t col) _NO_THROW;
   void Set (size_t row, size_t col, const T* data) _NO_THROW;

   // Utility methods
   matrixT Solve (const matrixT& v) const _THROW_MATRIX_ERROR;
   matrixT Adj () _THROW_MATRIX_ERROR;
   matrixT Inv () _THROW_MATRIX_ERROR;
   matrixT Transpose () _THROW_MATRIX_ERROR;
   T Det () const _THROW_MATRIX_ERROR;
   T Norm () _NO_THROW;
   T Cofact (size_t row, size_t col) _THROW_MATRIX_ERROR;
   T Cond () _NO_THROW;   


   // Type of matrices
   bool IsSquare () _NO_THROW { return (_m->Row == _m->Col); } 
   bool IsSingular () _NO_THROW;
   bool IsDiagonal () _NO_THROW;
   bool IsScalar () _NO_THROW;
   bool IsUnit () _NO_THROW;
   bool IsNull () _NO_THROW;
   bool IsSymmetric () _NO_THROW;
   bool IsSkewSymmetric () _NO_THROW;
   bool IsUpperTriangular () _NO_THROW;
   bool IsLowerTriangular () _NO_THROW;

protected:
    struct base_mat
    {
	T **Val;
	size_t Row, Col, RowSiz, ColSiz;
	int Refcnt;

	base_mat (size_t row, size_t col, T** v)
	{
	    Row = row; RowSiz = row;
	    Col = col; ColSiz = col;
	    Refcnt = 1;

	    Val = new T* [row];
	    size_t rowlen = col * sizeof(T);

	    for (size_t i=0; i < row; i++)
	    {
		Val[i] = new T [col];
		if (v) memcpy( Val[i], v[i], rowlen);
	    }
	}
	~base_mat ()
	{
	    for (size_t i=0; i < RowSiz; i++)
		delete [] Val[i];
	    delete [] Val;
	}
    };
    base_mat *_m;

    void clone ();
    void realloc (size_t row, size_t col);
    int pivot (size_t row);
};

MAT_TEMPLATE class TranslationMatrix : public matrixT
{
public:
    TranslationMatrix(T x, T y, T z);  
};

MAT_TEMPLATE class RotationMatrix : public matrixT
{
public:
    RotationMatrix(int axis, double angle);  // angle in radians
};

MAT_TEMPLATE class ScaleMatrix : public matrixT
{
public:
    ScaleMatrix(T x, T y, T z);  // angle in radians
};



#if defined(_MSC_VER) && _MSC_VER <= 1020
#  undef  _NO_THROW               // MSVC++ 4.0/4.2 does not support 
#  undef  _THROW_MATRIX_ERROR     // exception specification in definition
#  define _NO_THROW
#  define _THROW_MATRIX_ERROR
#endif

// constructor
MAT_TEMPLATE inline
matrixT::matrix (size_t row, size_t col)
{
  _m = new base_mat( row, col, 0);
}

// copy constructor
MAT_TEMPLATE inline
matrixT::matrix (const matrixT& m)
{
    _m = m._m;
    _m->Refcnt++;
}

// Internal copy constructor
MAT_TEMPLATE inline void
matrixT::clone ()
{
    _m->Refcnt--;
    _m = new base_mat( _m->Row, _m->Col, _m->Val);
}

// destructor
MAT_TEMPLATE inline
matrixT::~matrix ()
{
   if (--_m->Refcnt == 0) delete _m;
}

// assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator = (const matrixT& m) _NO_THROW
{
    m._m->Refcnt++;
    if (--_m->Refcnt == 0) delete _m;
    _m = m._m;
    return *this;
}

//  reallocation method
MAT_TEMPLATE inline void 
matrixT::realloc (size_t row, size_t col)
{
   if (row == _m->RowSiz && col == _m->ColSiz)
   {
      _m->Row = _m->RowSiz;
      _m->Col = _m->ColSiz;
      return;
   }

   base_mat *m1 = new base_mat( row, col, NULL);
   size_t colSize = min(_m->Col,col) * sizeof(T);
   size_t minRow = min(_m->Row,row);

   for (size_t i=0; i < minRow; i++)
      memcpy( m1->Val[i], _m->Val[i], colSize);

   if (--_m->Refcnt == 0) 
       delete _m;
   _m = m1;

   return;
}

// public method for resizing matrix
MAT_TEMPLATE inline void
matrixT::SetSize (size_t row, size_t col) _NO_THROW
{
   size_t i,j;
   size_t oldRow = _m->Row;
   size_t oldCol = _m->Col;

   if (row != _m->RowSiz || col != _m->ColSiz)
      realloc( row, col);

   for (i=oldRow; i < row; i++)
      for (j=0; j < col; j++)
	 _m->Val[i][j] = T(0);

   for (i=0; i < row; i++)                      
      for (j=oldCol; j < col; j++)
	 _m->Val[i][j] = T(0);

   return;
}

MAT_TEMPLATE inline void matrixT::Set(size_t row, size_t col, const T* data) _NO_THROW
{
   SetSize(row, col);
   for (size_t i = 0; i < row; i++)
   {
	   for (size_t j = 0; j < col; j++)
	   {
		   _m->Val[i][j] = data[j*col+i];
	   }
   }
}

// subscript operator to get/set individual elements
MAT_TEMPLATE inline T&
matrixT::operator () (size_t row, size_t col) _THROW_MATRIX_ERROR
{
   if (row >= _m->Row || col >= _m->Col)
      REPORT_ERROR( "matrixT::operator(): Index out of range!");
   if (_m->Refcnt > 1) clone();
   return _m->Val[row][col];
}

// subscript operator to get/set individual elements
MAT_TEMPLATE inline T
matrixT::operator () (size_t row, size_t col) const _THROW_MATRIX_ERROR
{
   if (row >= _m->Row || col >= _m->Col)
      REPORT_ERROR( "matrixT::operator(): Index out of range!");
   return _m->Val[row][col];
}

// input stream function
MAT_TEMPLATE inline istream&
operator >> (istream& istrm, matrixT& m)
{
   for (size_t i=0; i < m.RowNo(); i++)
      for (size_t j=0; j < m.ColNo(); j++)
      {
         T x;
         istrm >> x;
         m(i,j) = x;
      }
   return istrm;
}

// output stream function
MAT_TEMPLATE inline ostream&
operator << (ostream& ostrm, const matrixT& m)
{
   for (size_t i=0; i < m.RowNo(); i++)
   {
      for (size_t j=0; j < m.ColNo(); j++)
      {
         T x = m(i,j);
         ostrm << x << '\t';
      }
      ostrm << endl;
   }
   return ostrm;
}


// logical equal-to operator
MAT_TEMPLATE inline bool
operator == (const matrixT& m1, const matrixT& m2) _NO_THROW
{
   if (m1.RowNo() != m2.RowNo() || m1.ColNo() != m2.ColNo())
      return false;

   for (size_t i=0; i < m1.RowNo(); i++)
      for (size_t j=0; j < m1.ColNo(); j++)
	      if (m1(i,j) != m2(i,j))
	         return false;

   return true;
}

// logical no-equal-to operator
MAT_TEMPLATE inline bool
operator != (const matrixT& m1, const matrixT& m2) _NO_THROW
{
    return (m1 == m2) ? false : true;
}

// combined addition and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator += (const matrixT& m) _THROW_MATRIX_ERROR
{
   if (_m->Row != m._m->Row || _m->Col != m._m->Col)
      REPORT_ERROR( "matrixT::operator+= : Inconsistent matrix sizes in addition!");
   if (_m->Refcnt > 1) clone();
   for (size_t i=0; i < m._m->Row; i++)
      for (size_t j=0; j < m._m->Col; j++)
	 _m->Val[i][j] += m._m->Val[i][j];
   return *this;
}

// combined subtraction and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator -= (const matrixT& m) _THROW_MATRIX_ERROR
{
   if (_m->Row != m._m->Row || _m->Col != m._m->Col)
      REPORT_ERROR( "matrixT::operator-= : Inconsistent matrix sizes in subtraction!");
   if (_m->Refcnt > 1) clone();
   for (size_t i=0; i < m._m->Row; i++)
      for (size_t j=0; j < m._m->Col; j++)
	 _m->Val[i][j] -= m._m->Val[i][j];
   return *this;
}

// combined scalar multiplication and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator *= (const T& c) _NO_THROW
{
    if (_m->Refcnt > 1) clone();
    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] *= c;
    return *this;
}

// combined matrix multiplication and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator *= (const matrixT& m) _THROW_MATRIX_ERROR
{
   if (_m->Col != m._m->Row)
      REPORT_ERROR( "matrixT::operator*= : Inconsistent matrix sizes in multiplication!");

   matrixT temp(_m->Row,m._m->Col);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < m._m->Col; j++)
      {
         temp._m->Val[i][j] = T(0);
         for (size_t k=0; k < _m->Col; k++)
            temp._m->Val[i][j] += _m->Val[i][k] * m._m->Val[k][j];
      }
   *this = temp;

   return *this;
}

// combined scalar division and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator /= (const T& c) _NO_THROW
{
    if (_m->Refcnt > 1) clone();
    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] /= c;

    return *this;
}

// combined power and assignment operator
MAT_TEMPLATE inline matrixT&
matrixT::operator ^= (const size_t& pow) _THROW_MATRIX_ERROR
{
	matrixT temp(*this);

	for (size_t i=2; i <= pow; i++)
      *this = *this * temp;

	return *this;
}

// unary negation operator
MAT_TEMPLATE inline matrixT
matrixT::operator - () _NO_THROW
{
   matrixT temp(_m->Row,_m->Col);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 temp._m->Val[i][j] = - _m->Val[i][j];

   return temp;
}

// binary addition operator
MAT_TEMPLATE inline matrixT
operator + (const matrixT& m1, const matrixT& m2) _THROW_MATRIX_ERROR
{
   matrixT temp = m1;
   temp += m2;
   return temp;
}

// binary subtraction operator
MAT_TEMPLATE inline matrixT
operator - (const matrixT& m1, const matrixT& m2) _THROW_MATRIX_ERROR
{
   matrixT temp = m1;
   temp -= m2;
   return temp;
}

// binary scalar multiplication operator
MAT_TEMPLATE inline matrixT
operator * (const matrixT& m, const T& no) _NO_THROW
{
   matrixT temp = m;
   temp *= no;
   return temp;
}


// binary scalar multiplication operator
MAT_TEMPLATE inline matrixT
operator * (const T& no, const matrixT& m) _NO_THROW
{
   return (m * no);
}

// binary matrix multiplication operator
MAT_TEMPLATE inline matrixT
operator * (const matrixT& m1, const matrixT& m2) _THROW_MATRIX_ERROR
{
   matrixT temp = m1;
   temp *= m2;
   return temp;
}

// binary scalar division operator
MAT_TEMPLATE inline matrixT
operator / (const matrixT& m, const T& no) _NO_THROW
{
    return (m * (T(1) / no));
}


// binary scalar division operator
MAT_TEMPLATE inline matrixT
operator / (const T& no, const matrixT& m) _THROW_MATRIX_ERROR
{
    return (!m * no);
}

// binary matrix division operator
MAT_TEMPLATE inline matrixT
operator / (const matrixT& m1, const matrixT& m2) _THROW_MATRIX_ERROR
{
    return (m1 * !m2);
}

// binary power operator
MAT_TEMPLATE inline matrixT
operator ^ (const matrixT& m, const size_t& pow) _THROW_MATRIX_ERROR
{
   matrixT temp = m;
   temp ^= pow;
   return temp;
}

// unary transpose operator
MAT_TEMPLATE inline matrixT
operator ~ (const matrixT& m) _NO_THROW
{
   matrixT temp(m.ColNo(),m.RowNo());

   for (size_t i=0; i < m.RowNo(); i++)
      for (size_t j=0; j < m.ColNo(); j++)
      {
         T x = m(i,j);
	      temp(j,i) = x;
      }
   return temp;
}

// unary inversion operator
MAT_TEMPLATE inline matrixT
operator ! (const matrixT m) _THROW_MATRIX_ERROR
{
   matrixT temp = m;
   return temp.Inv();
}

// inversion function
MAT_TEMPLATE inline matrixT
matrixT::Inv () _THROW_MATRIX_ERROR
{
   size_t i,j,k;
   T a1,a2,*rowptr;

   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::operator!: Inversion of a non-square matrix");

   matrixT temp(_m->Row,_m->Col);
   if (_m->Refcnt > 1) clone();


   temp.Unit();
   for (k=0; k < _m->Row; k++)
   {
      int indx = pivot(k);
      if (indx == -1)
	      REPORT_ERROR( "matrixT::operator!: Inversion of a singular matrix");

      if (indx != 0)
      {
	      rowptr = temp._m->Val[k];
	      temp._m->Val[k] = temp._m->Val[indx];
	      temp._m->Val[indx] = rowptr;
      }
      a1 = _m->Val[k][k];
      for (j=0; j < _m->Row; j++)
      {
	      _m->Val[k][j] /= a1;
	      temp._m->Val[k][j] /= a1;
      }
      for (i=0; i < _m->Row; i++)
	   if (i != k)
	   {
	      a2 = _m->Val[i][k];
	      for (j=0; j < _m->Row; j++)
	      {
	         _m->Val[i][j] -= a2 * _m->Val[k][j];
	         temp._m->Val[i][j] -= a2 * temp._m->Val[k][j];
	      }
	   }
   }
   return temp;
}

// solve simultaneous equation
MAT_TEMPLATE inline matrixT
matrixT::Solve (const matrixT& v) const _THROW_MATRIX_ERROR
{
   size_t i,j,k;
   T a1;

   if (!(_m->Row == _m->Col && _m->Col == v._m->Row))
      REPORT_ERROR( "matrixT::Solve():Inconsistent matrices!");

   matrixT temp(_m->Row,_m->Col+v._m->Col);
   for (i=0; i < _m->Row; i++)
   {
      for (j=0; j < _m->Col; j++)
	 temp._m->Val[i][j] = _m->Val[i][j];
      for (k=0; k < v._m->Col; k++)
	 temp._m->Val[i][_m->Col+k] = v._m->Val[i][k];
   }
   for (k=0; k < _m->Row; k++)
   {
      int indx = temp.pivot(k);
      if (indx == -1)
	 REPORT_ERROR( "matrixT::Solve(): Singular matrix!");

      a1 = temp._m->Val[k][k];
      for (j=k; j < temp._m->Col; j++)
	 temp._m->Val[k][j] /= a1;

      for (i=k+1; i < _m->Row; i++)
      {
	 a1 = temp._m->Val[i][k];
	 for (j=k; j < temp._m->Col; j++)
	   temp._m->Val[i][j] -= a1 * temp._m->Val[k][j];
      }
   }
   matrixT s(v._m->Row,v._m->Col);
   for (k=0; k < v._m->Col; k++)
      for (int m=int(_m->Row)-1; m >= 0; m--)
      {
	 s._m->Val[m][k] = temp._m->Val[m][_m->Col+k];
	 for (j=m+1; j < _m->Col; j++)
	    s._m->Val[m][k] -= temp._m->Val[m][j] * s._m->Val[j][k];
      }
   return s;
}

// set zero to all elements of this matrix
MAT_TEMPLATE inline void
matrixT::Null (const size_t& row, const size_t& col) _NO_THROW
{
    if (row != _m->Row || col != _m->Col)
	realloc( row,col);

    if (_m->Refcnt > 1) 
	clone();

    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] = T(0);
    return;
}

// set zero to all elements of this matrix
MAT_TEMPLATE inline void
matrixT::Null() _NO_THROW
{
    if (_m->Refcnt > 1) clone();   
    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
		_m->Val[i][j] = T(0);
    return;
}

// set this matrix to unity
MAT_TEMPLATE inline void
matrixT::Unit (const size_t& row) _NO_THROW
{
    if (row != _m->Row || row != _m->Col)
	realloc( row, row);
	
    if (_m->Refcnt > 1) 
	clone();

    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] = i == j ? T(1) : T(0);
    return;
}

// set this matrix to unity
MAT_TEMPLATE inline void
matrixT::Unit () _NO_THROW
{
    if (_m->Refcnt > 1) clone();   
    size_t row = min(_m->Row,_m->Col);
    _m->Row = _m->Col = row;

    for (size_t i=0; i < _m->Row; i++)
	for (size_t j=0; j < _m->Col; j++)
	    _m->Val[i][j] = i == j ? T(1) : T(0);
    return;
}

// private partial pivoting method
MAT_TEMPLATE inline int
matrixT::pivot (size_t row)
{
  int k = int(row);
  double amax,temp;

  amax = -1;
  for (size_t i=row; i < _m->Row; i++)
    if ( (temp = abs( _m->Val[i][row])) > amax && temp != 0.0)
     {
       amax = temp;
       k = i;
     }
  if (_m->Val[k][row] == T(0))
     return -1;
  if (k != int(row))
  {
     T* rowptr = _m->Val[k];
     _m->Val[k] = _m->Val[row];
     _m->Val[row] = rowptr;
     return k;
  }
  return 0;
}

// calculate the determinant of a matrix
MAT_TEMPLATE inline T
matrixT::Det () const _THROW_MATRIX_ERROR
{
   size_t i,j,k;
   T piv,detVal = T(1);

   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::Det(): Determinant a non-square matrix!");
   
   matrixT temp(*this);
   if (temp._m->Refcnt > 1) temp.clone();

   for (k=0; k < _m->Row; k++)
   {
      int indx = temp.pivot(k);
      if (indx == -1)
	 return 0;
      if (indx != 0)
	 detVal = - detVal;
      detVal = detVal * temp._m->Val[k][k];
      for (i=k+1; i < _m->Row; i++)
      {
	 piv = temp._m->Val[i][k] / temp._m->Val[k][k];
	 for (j=k+1; j < _m->Row; j++)
	    temp._m->Val[i][j] -= piv * temp._m->Val[k][j];
      }
   }
   return detVal;
}

// calculate the norm of a matrix
MAT_TEMPLATE inline T
matrixT::Norm () _NO_THROW
{
   T retVal = T(0);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 retVal += _m->Val[i][j] * _m->Val[i][j];
   retVal = sqrt( retVal);

   return retVal;
}

// calculate the condition number of a matrix
MAT_TEMPLATE inline T
matrixT::Cond () _NO_THROW
{
   matrixT inv = ! (*this);
   return (Norm() * inv.Norm());
}

// calculate the cofactor of a matrix for a given element
MAT_TEMPLATE inline T
matrixT::Cofact (size_t row, size_t col) _THROW_MATRIX_ERROR
{
   size_t i,i1,j,j1;

   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::Cofact(): Cofactor of a non-square matrix!");

   if (row > _m->Row || col > _m->Col)
      REPORT_ERROR( "matrixT::Cofact(): Index out of range!");

   matrixT temp (_m->Row-1,_m->Col-1);

   for (i=i1=0; i < _m->Row; i++)
   {
      if (i == row)
	continue;
      for (j=j1=0; j < _m->Col; j++)
      {
	 if (j == col)
	    continue;
	 temp._m->Val[i1][j1] = _m->Val[i][j];
	 j1++;
      }
      i1++;
   }
   T  cof = temp.Det();
   if ((row+col)%2 == 1)
      cof = -cof;

   return cof;
}

// calculate adjoin of a matrix
MAT_TEMPLATE inline matrixT
matrixT::Transpose () _THROW_MATRIX_ERROR
{
/*
   matrixT temp(_m->Col,_m->Row);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	      temp._m->Val[j][i] = _m->Val[i][j];
*/
   return ~(*this);
}


// calculate adjoin of a matrix
MAT_TEMPLATE inline matrixT
matrixT::Adj () _THROW_MATRIX_ERROR
{
   if (_m->Row != _m->Col)
      REPORT_ERROR( "matrixT::Adj(): Adjoin of a non-square matrix.");

   matrixT temp(_m->Row,_m->Col);

   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	  temp._m->Val[j][i] = Cofact(i,j);
   return temp;
}

// Determine if the matrix is singular
MAT_TEMPLATE inline bool
matrixT::IsSingular () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   return (Det() == T(0));
}

// Determine if the matrix is diagonal
MAT_TEMPLATE inline bool
matrixT::IsDiagonal () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=0; i < _m->Row; i++)
     for (size_t j=0; j < _m->Col; j++)
	if (i != j && _m->Val[i][j] != T(0))
	  return false;
   return true;
}

// Determine if the matrix is scalar
MAT_TEMPLATE inline bool
matrixT::IsScalar () _NO_THROW
{
   if (!IsDiagonal())
     return false;
   T v = _m->Val[0][0];
   for (size_t i=1; i < _m->Row; i++)
     if (_m->Val[i][i] != v)
	return false;
   return true;
}

// Determine if the matrix is a unit matrix
MAT_TEMPLATE inline bool
matrixT::IsUnit () _NO_THROW
{
   if (IsScalar() && _m->Val[0][0] == T(1))
     return true;
   return false;
}

// Determine if this is a null matrix
MAT_TEMPLATE inline bool
matrixT::IsNull () _NO_THROW
{
   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 if (_m->Val[i][j] != T(0))
	    return false;
   return true;
}

// Determine if the matrix is symmetric
MAT_TEMPLATE inline bool
matrixT::IsSymmetric () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 if (_m->Val[i][j] != _m->Val[j][i])
	    return false;
   return true;
}
	   
// Determine if the matrix is skew-symmetric
MAT_TEMPLATE inline bool
matrixT::IsSkewSymmetric () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=0; i < _m->Row; i++)
      for (size_t j=0; j < _m->Col; j++)
	 if (_m->Val[i][j] != -_m->Val[j][i])
	    return false;
   return true;
}
   
// Determine if the matrix is upper triangular
MAT_TEMPLATE inline bool
matrixT::IsUpperTriangular () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;
   for (size_t i=1; i < _m->Row; i++)
      for (size_t j=0; j < i-1; j++)
	 if (_m->Val[i][j] != T(0))
	    return false;
   return true;
}

// Determine if the matrix is lower triangular
MAT_TEMPLATE inline bool
matrixT::IsLowerTriangular () _NO_THROW
{
   if (_m->Row != _m->Col)
      return false;

   for (size_t j=1; j < _m->Col; j++)
      for (size_t i=0; i < j-1; i++)
	 if (_m->Val[i][j] != T(0))
	    return false;

   return true;
}

MAT_TEMPLATE inline
TranslationMatrix<T>::TranslationMatrix (T x, T y, T z) : matrixT(4, 4)
{
  _m->Val[0][0] = T(1); _m->Val[0][1] = T(0); _m->Val[0][2] = T(0); _m->Val[0][3] = x;
  _m->Val[1][0] = T(0); _m->Val[1][1] = T(1); _m->Val[1][2] = T(0); _m->Val[1][3] = y;
  _m->Val[2][0] = T(0); _m->Val[2][1] = T(0); _m->Val[2][2] = T(1); _m->Val[2][3] = z;
  _m->Val[3][0] = T(0); _m->Val[3][1] = T(0); _m->Val[3][2] = T(0); _m->Val[3][3] = T(1);
}

MAT_TEMPLATE inline
RotationMatrix<T>::RotationMatrix (int axis, double angle) : matrixT(4, 4)
{
  if (axis == 0)  // X
  {
      _m->Val[0][0] = 1;           _m->Val[0][1] = 0;          _m->Val[0][2] =  0;          _m->Val[0][3] = 0;
      _m->Val[1][0] = 0;           _m->Val[1][1] = cos(angle); _m->Val[1][2] = -sin(angle); _m->Val[1][3] = 0;
      _m->Val[2][0] = 0;           _m->Val[2][1] = sin(angle); _m->Val[2][2] = cos(angle);  _m->Val[2][3] = 0;
      _m->Val[3][0] = 0;           _m->Val[3][1] = 0;          _m->Val[3][2] = 0;           _m->Val[3][3] = 1;
  }
  else if (axis == 1)  // Y
  {
      _m->Val[0][0] = cos(angle);  _m->Val[0][1] = 0;           _m->Val[0][2] =  sin(angle); _m->Val[0][3] = 0;
      _m->Val[1][0] = 0;           _m->Val[1][1] = 1;           _m->Val[1][2] = 0;           _m->Val[1][3] = 0;
      _m->Val[2][0] = -sin(angle); _m->Val[2][1] = 0;           _m->Val[2][2] = cos(angle);  _m->Val[2][3] = 0;
      _m->Val[3][0] = 0;           _m->Val[3][1] = 0;           _m->Val[3][2] = 0;           _m->Val[3][3] = 1;
  }
  else if (axis == 2) // Z
  {
      _m->Val[0][0] = cos(angle); _m->Val[0][1] = -sin(angle); _m->Val[0][2] = 0;            _m->Val[0][3] = 0;
      _m->Val[1][0] = sin(angle); _m->Val[1][1] = cos(angle);  _m->Val[1][2] = 0;            _m->Val[1][3] = 0;
      _m->Val[2][0] = 0;          _m->Val[2][1] = 0;           _m->Val[2][2] = 1;            _m->Val[2][3] = 0;
      _m->Val[3][0] = 0;          _m->Val[3][1] = 0;           _m->Val[3][2] = 0;            _m->Val[3][3] = 1;
  }
}

MAT_TEMPLATE inline
ScaleMatrix<T>::ScaleMatrix (T x, T y, T z) : matrixT(4, 4)
{
  _m->Val[0][0] = x;    _m->Val[0][1] = T(0); _m->Val[0][2] = T(0); _m->Val[0][3] = T(0);
  _m->Val[1][0] = T(0); _m->Val[1][1] = y;    _m->Val[1][2] = T(0); _m->Val[1][3] = T(0);
  _m->Val[2][0] = T(0); _m->Val[2][1] = T(0); _m->Val[2][2] = z;    _m->Val[2][3] = T(0);
  _m->Val[3][0] = T(0); _m->Val[3][1] = T(0); _m->Val[3][2] = T(0); _m->Val[3][3] = T(1);
}

MAT_TEMPLATE inline vec3 operator* (const matrixT& m, const vec3& v) _THROW_MATRIX_ERROR // transform point
{
   matrixT vMat(4, 1);
   vMat(0,0) = v[0]; vMat(1,0) = v[1]; vMat(2,0) = v[2]; vMat(3,0) = 1.0f;
   matrixT result = m*vMat;
   return vec3(result(0,0), result(1,0), result(2,0));
}

MAT_TEMPLATE inline vec3 operator^ (const matrixT& m, const vec3& v) _THROW_MATRIX_ERROR // transform vector
{
   matrixT vMat(4, 1);
   vMat(0,0) = v[0]; vMat(1,0) = v[1]; vMat(2,0) = v[2]; vMat(3,0) = 0.0f;
   matrixT result = m*vMat;
   vec4 tmp = vec4(result(0,0), result(1,0), result(2,0), result(3,0));
   return vec3(result(0,0), result(1,0), result(2,0));
}


#ifndef _NO_NAMESPACE
} 
#endif

#endif //__STD_MATRIX_H