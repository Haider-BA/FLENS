#include <flens/lapack/interface/include/config.h>


namespace flens { namespace lapack {

extern "C" {

//-- dgetri --------------------------------------------------------------------
void
LAPACK_DECL(dgetri)(const INTEGER    *N,
                    DOUBLE           *A,
                    const INTEGER    *LDA,
                    const INTEGER    *IPIV,
                    DOUBLE           *WORK,
                    const INTEGER    *LWORK,
                    INTEGER          *INFO)
{
    using std::max;
    using std::min;
//
//  Test the input parameters so that we pass LAPACK error checks
//
    *INFO = 0;
    bool lQuery = (*LWORK==-1);

    if (*N<0) {
        *INFO = 1;
    } else if (*LDA<max(INTEGER(1),*N)) {
        *INFO = 3;
    } else if (*LWORK<max(INTEGER(1),*N) && !lQuery) {
        *INFO = 6;
    }

    if (*INFO!=0) {
        LAPACK_ERROR("DGETRI", INFO);
        *INFO = -(*INFO);
        return;
    }

//
//  Setup FLENS matrix/vector types
//
    DGeMatrixView          A_      = DFSView(*N, *N, A, *LDA);
    IConstDenseVectorView  IPIV_   = IConstArrayView(*N, IPIV, 1);
    DDenseVectorView       WORK_   = DArrayView(*LWORK, WORK, 1);

//
//  Call FLENS implementation
//
    tri(A_, IPIV_, WORK_);
}

//-- zgetri --------------------------------------------------------------------
void
LAPACK_DECL(zgetri)(const INTEGER    *N,
                    DOUBLE_COMPLEX   *A,
                    const INTEGER    *LDA,
                    const INTEGER    *IPIV,
                    DOUBLE_COMPLEX   *WORK,
                    const INTEGER    *LWORK,
                    INTEGER          *INFO)
{
    using std::max;
    using std::min;
//
//  Test the input parameters so that we pass LAPACK error checks
//
    *INFO = 0;
    bool lQuery = (*LWORK==-1);

    if (*N<0) {
        *INFO = 1;
    } else if (*LDA<max(INTEGER(1),*N)) {
        *INFO = 3;
    } else if (*LWORK<max(INTEGER(1),*N) && !lQuery) {
        *INFO = 6;
    }

    if (*INFO!=0) {
        LAPACK_ERROR("ZGETRI", INFO);
        *INFO = -(*INFO);
        return;
    }

//
//  Setup FLENS matrix/vector types
//
    auto zA     = reinterpret_cast<CXX_DOUBLE_COMPLEX *>(A);
    auto zWORK  = reinterpret_cast<CXX_DOUBLE_COMPLEX *>(WORK);

    ZGeMatrixView          A_      = ZFSView(*N, *N, zA, *LDA);
    IConstDenseVectorView  IPIV_   = IConstArrayView(*N, IPIV, 1);
    ZDenseVectorView       WORK_   = ZArrayView(*LWORK, zWORK, 1);

//
//  Call FLENS implementation
//
    tri(A_, IPIV_, WORK_);
}

} // extern "C"

} } // namespace lapack, flens
