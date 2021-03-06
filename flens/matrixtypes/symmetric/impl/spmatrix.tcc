/*
 *   Copyright (c) 2012, Michael Lehn, Klaus Pototzky
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2) Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3) Neither the name of the FLENS development group nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLENS_MATRIXTYPES_SYMMETRIC_IMPL_SPMATRIX_TCC
#define FLENS_MATRIXTYPES_SYMMETRIC_IMPL_SPMATRIX_TCC 1

#include <flens/auxiliary/auxiliary.h>
#include <flens/blas/level1/copy.h>
#include <flens/matrixtypes/symmetric/impl/spmatrix.h>
#include <flens/typedefs.h>

namespace flens {

//-- Constructors --------------------------------------------------------------

template <typename PS>
SpMatrix<PS>::SpMatrix(IndexType dim, StorageUpLo upLo)
      : engine_(dim), upLo_(upLo)
{
}

template <typename PS>
SpMatrix<PS>::SpMatrix(const Engine &engine, StorageUpLo upLo)
    : engine_(engine), upLo_(upLo)
{
}

template <typename PS>
SpMatrix<PS>::SpMatrix(const SpMatrix &rhs)
    : engine_(rhs.engine()), upLo_(rhs.upLo())
{
}

template <typename PS>
template <typename RHS>
SpMatrix<PS>::SpMatrix(const SpMatrix<RHS> &rhs)
    : engine_(rhs.engine()), upLo_(rhs.upLo())
{
}

template <typename PS>
template <typename RHS>
SpMatrix<PS>::SpMatrix(SpMatrix<RHS> &rhs)
    : engine_(rhs.engine()), upLo_(rhs.upLo())
{
}

template <typename PS>
template <typename RHS, class>
SpMatrix<PS>::SpMatrix(SpMatrix<RHS> &&rhs)
    : engine_(rhs.engine()), upLo_(rhs.upLo())
{
}

template <typename PS>
template <typename RHS>
SpMatrix<PS>::SpMatrix(const Matrix<RHS> &rhs)
{
    assign(rhs, *this);
}

//-- Operators -----------------------------------------------------------------

template <typename PS>
SpMatrix<PS> &
SpMatrix<PS>::operator=(const ElementType &alpha)
{
    VectorView x = ArrayView(engine_.numNonZeros(), engine_.data());

    x = alpha;

    return *this;
}

template <typename PS>
SpMatrix<PS> &
SpMatrix<PS>::operator=(const SpMatrix &rhs)
{
    if (this!=&rhs) {
        assign(rhs, *this);
    }
    return *this;
}

template <typename PS>
template <typename RHS>
SpMatrix<PS> &
SpMatrix<PS>::operator=(const Matrix<RHS> &rhs)
{
    assign(rhs, *this);
    return *this;
}

template <typename PS>
template <typename RHS>
SpMatrix<PS> &
SpMatrix<PS>::operator+=(const Matrix<RHS> &rhs)
{
    plusAssign(rhs, *this);
    return *this;
}

template <typename PS>
template <typename RHS>
SpMatrix<PS> &
SpMatrix<PS>::operator-=(const Matrix<RHS> &rhs)
{
    minusAssign(rhs, *this);
    return *this;
}

template <typename PS>
const typename SpMatrix<PS>::ElementType &
SpMatrix<PS>::operator()(IndexType row, IndexType col) const
{
#   ifndef NDEBUG
    if (upLo()==Upper) {
        ASSERT(col>=row);
    } else {
        ASSERT(col<=row);
    }
#   endif

    return engine_(upLo(), row, col);
}

template <typename PS>
typename SpMatrix<PS>::ElementType &
SpMatrix<PS>::operator()(IndexType row, IndexType col)
{
#   ifndef NDEBUG
    if (upLo()==Upper) {
        ASSERT(col>=row);
    } else {
        ASSERT(col<=row);
    }
#   endif

    return engine_(upLo(), row, col);
}


template <typename PS>
SpMatrix<PS> &
SpMatrix<PS>::operator+=(const ElementType &alpha)
{
    VectorView x = ArrayView(engine_.numNonZeros(), engine_.data());

    x += alpha;

    return *this;
}

template <typename PS>
SpMatrix<PS> &
SpMatrix<PS>::operator-=(const ElementType &alpha)
{
    VectorView x = ArrayView(engine_.numNonZeros(), engine_.data());

    x -= alpha;

    return *this;
}

template <typename PS>
SpMatrix<PS> &
SpMatrix<PS>::operator*=(const ElementType &alpha)
{
    VectorView x = ArrayView(engine_.numNonZeros(), engine_.data());

    x *= alpha;

    return *this;
}

template <typename PS>
SpMatrix<PS> &
SpMatrix<PS>::operator/=(const ElementType &alpha)
{
    VectorView x = ArrayView(engine_.numNonZeros(), engine_.data());

    x /= alpha;

    return *this;
}

// -- methods ------------------------------------------------------------------
template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::numRows() const
{
    return engine_.dim();
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::numCols() const
{
    return engine_.dim();
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::dim() const
{
    return engine_.dim();
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::firstRow() const
{
    return engine_.indexBase();
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::lastRow() const
{
    return firstRow()+numRows()-1;
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::firstCol() const
{
    return engine_.indexBase();
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::lastCol() const
{
    return firstCol()+numCols()-1;
}

template <typename PS>
typename SpMatrix<PS>::IndexType
SpMatrix<PS>::indexBase() const
{
    return engine_.indexBase();
}

template <typename PS>
StorageUpLo
SpMatrix<PS>::upLo() const
{
    return upLo_;
}

template <typename PS>
StorageUpLo &
SpMatrix<PS>::upLo()
{
    return upLo_;
}

template <typename PS>
const typename SpMatrix<PS>::ElementType *
SpMatrix<PS>::data() const
{
    return engine_.data();
}

template <typename PS>
typename SpMatrix<PS>::ElementType *
SpMatrix<PS>::data()
{
    return engine_.data();
}

template <typename PS>
StorageOrder
SpMatrix<PS>::order() const
{
    return engine_.order;
}

template <typename PS>
bool
SpMatrix<PS>::fill(const ElementType &value)
{
    return engine_.fill(value);
}

template <typename PS>
template <typename RHS>
bool
SpMatrix<PS>::resize(const SpMatrix<RHS> &rhs,
                     const ElementType &value)
{
    return engine_.resize(rhs.dim(), rhs.indexBase(), value);
}

template <typename PS>
bool
SpMatrix<PS>::resize(IndexType dim, IndexType firstIndex,
                     const ElementType &value)
{
    return engine_.resize(dim, firstIndex, value);
}

// -- views --------------------------------------------------------------------

// hermitian views
template <typename PS>
const typename SpMatrix<PS>::ConstHermitianView
SpMatrix<PS>::hermitian() const
{
    return ConstHermitianView(engine_);
}

template <typename PS>
typename SpMatrix<PS>::HermitianView
SpMatrix<PS>::hermitian()
{
    return HermitianView(engine_);
}

// symmetric views
template <typename PS>
const typename SpMatrix<PS>::ConstView
SpMatrix<PS>::symmetric() const
{
    return ConstView(engine_);
}

template <typename PS>
typename SpMatrix<PS>::View
SpMatrix<PS>::symmetric()
{
    return View(engine_);
}

// triangular views
template <typename PS>
const typename SpMatrix<PS>::ConstTriangularView
SpMatrix<PS>::triangular() const
{
    return ConstTriangularView(engine_);
}

template <typename PS>
typename SpMatrix<PS>::TriangularView
SpMatrix<PS>::triangular()
{
    return TriangularView(engine_);
}

// -- implementation -----------------------------------------------------------

template <typename PS>
const typename SpMatrix<PS>::Engine &
SpMatrix<PS>::engine() const
{
    return engine_;
}

template <typename PS>
typename SpMatrix<PS>::Engine &
SpMatrix<PS>::engine()
{
    return engine_;
}

} // namespace flens

#endif // FLENS_MATRIXTYPES_SYMMETRIC_IMPL_SPMATRIX_TCC
