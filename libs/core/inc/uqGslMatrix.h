/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id$
 *
 * Brief description of this file: 
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#ifndef __UQ_GSL_MATRIX_H__
#define __UQ_GSL_MATRIX_H__

#include <uqMatrix.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <uqGslVector.h>

class uqGslMatrixClass : public uqMatrixClass
{
public:
  uqGslMatrixClass();
  uqGslMatrixClass(const uqBaseEnvironmentClass& env,
                   const Epetra_Map&             map,
                   unsigned int                  numCols);
  uqGslMatrixClass(const uqBaseEnvironmentClass& env,
                   const Epetra_Map&             map,
                   double                        diagValue); // MATLAB eye
  uqGslMatrixClass(const uqGslVectorClass&       v,
                   double                        diagValue); // MATLAB eye
  uqGslMatrixClass(const uqGslVectorClass&       v);         // MATLAB diag
  uqGslMatrixClass(const uqGslMatrixClass&       B);
 ~uqGslMatrixClass();

  uqGslMatrixClass& operator= (const uqGslMatrixClass& rhs);
  uqGslMatrixClass& operator*=(double a);
  uqGslMatrixClass& operator/=(double a);
  uqGslMatrixClass& operator+=(const uqGslMatrixClass& rhs);
  uqGslMatrixClass& operator-=(const uqGslMatrixClass& rhs);
            double& operator()(unsigned int i, unsigned int j);
      const double& operator()(unsigned int i, unsigned int j) const;

  unsigned int      numRowsLocal  () const;
  unsigned int      numRowsGlobal () const;
  unsigned int      numCols       () const;
  int               chol          ();
  void              zeroLower     (bool includeDiagonal = false);
  void              zeroUpper     (bool includeDiagonal = false);
  uqGslMatrixClass  transpose     () const;
  uqGslVectorClass  multiply      (const uqGslVectorClass& x) const;
  uqGslVectorClass  invertMultiply(const uqGslVectorClass& b) const;
  void              invertMultiply(const uqGslVectorClass& b, uqGslVectorClass& x) const;
  void              invertMultiply(const uqGslMatrixClass& B, uqGslMatrixClass& X) const;

  void              eigen         (uqGslVectorClass& eigenValues, uqGslMatrixClass* eigenVectors) const;
  void              largestEigen  (double& eigenValue, uqGslVectorClass& eigenVector) const;
  void              smallestEigen (double& eigenValue, uqGslVectorClass& eigenVector) const;

  uqGslVectorClass  invertMultiplyForceLU(const uqGslVectorClass& b) const;
  void              invertMultiplyForceLU(const uqGslVectorClass& b, uqGslVectorClass& x) const;

  void              print                (std::ostream& os) const;
  void              subWriteContents     (const std::string&            varNamePrefix,
                                          const std::string&            fileName,
                                          const std::set<unsigned int>& allowedSubEnvIds) const;

  void              getColumn            (const unsigned int column_num, uqGslVectorClass& column) const;
  void              getRow               (const unsigned int row_num, uqGslVectorClass& row) const;
  uqGslVectorClass  getColumn            (const unsigned int column_num ) const;
  uqGslVectorClass  getRow               (const unsigned int row_num ) const;
  void              setColumn            (const unsigned int column_num, const uqGslVectorClass& column);
  void              setRow               (const unsigned int row_num, const uqGslVectorClass& row);

  void              mpiSum               (const MPI_Comm& comm, uqGslMatrixClass& M_global ) const;

private:
  void              copy                 (const uqGslMatrixClass& src);
  void              multiply             (const uqGslVectorClass& x, uqGslVectorClass& y) const;

          gsl_matrix*      m_mat;
  mutable gsl_matrix*      m_LU;
  mutable gsl_permutation* m_permutation;
};

uqGslMatrixClass operator*    (double a,                    const uqGslMatrixClass& mat);
uqGslVectorClass operator*    (const uqGslMatrixClass& mat, const uqGslVectorClass& vec);
uqGslMatrixClass operator*    (const uqGslMatrixClass& m1,  const uqGslMatrixClass& m2 );
uqGslMatrixClass operator+    (const uqGslMatrixClass& m1,  const uqGslMatrixClass& m2 );
uqGslMatrixClass operator-    (const uqGslMatrixClass& m1,  const uqGslMatrixClass& m2 );
uqGslMatrixClass matrixProduct(const uqGslVectorClass& v1,  const uqGslVectorClass& v2 );
uqGslMatrixClass diagScaling  (const uqGslVectorClass& vec, const uqGslMatrixClass& mat);
std::ostream&    operator<<   (std::ostream& os,            const uqGslMatrixClass& obj);

#endif // __UQ_GSL_MATRIX_H__
