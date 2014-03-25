#include <flens/blas/interface/blas/config.h>


using namespace flens;

extern "C" {

void
BLAS(strmm)(const char      *SIDE,
            const char      *UPLO,
            const char      *TRANSA,
            const char      *DIAG,
            const INTEGER   *M,
            const INTEGER   *N,
            const float     *ALPHA,
            const float     *_A,
            const INTEGER   *LDA,
            float           *_B,
            const INTEGER   *LDB)
{
#   ifdef TEST_DIRECT_CBLAS
    
    char    _SIDE   = toupper(*SIDE);
    char    _UPLO   = toupper(*UPLO);
    char    _TRANSA = toupper(*TRANSA);
    char    _DIAG   = toupper(*DIAG);
    
    Side           side   = Side(_SIDE);
    StorageUpLo    upLo   = StorageUpLo(_UPLO);
    Transpose      transA = convertTo<Transpose>(_TRANSA);
    Diag           diag   = Diag(_DIAG);
    
        cblas_strmm(CBLAS_ORDER::CblasColMajor,
                    cxxblas::CBLAS::getCblasType(side),
                    cxxblas::CBLAS::getCblasType(upLo),
                    cxxblas::CBLAS::getCblasType(transA),
                    cxxblas::CBLAS::getCblasType(diag),
                    *M, *N, *ALPHA,
                    _A, *LDA, _B, *LDB);
        
#   else
        using std::abs;
        using std::max;

        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        INTEGER nRowA   = (_SIDE=='L') ? *M : *N;

#       ifndef NO_INPUT_CHECK
            INTEGER info  = 0;
            if (_SIDE!='L' && _SIDE!='R') {
                info = 1;
            } else if (_UPLO!='U' && _UPLO!='L') {
                info = 2;
            } else if (_TRANSA!='N' && _TRANSA!='T' && _TRANSA!='C') {
                info = 3;
            } else if (_DIAG!='U' && _DIAG!='N') {
                info = 4;
            } else if (*M<0) {
                info = 5;
            } else if (*N<0) {
                info = 6;
            } else if (*LDA<max(INTEGER(1),nRowA)) {
                info = 9;
            } else if (*LDB<max(INTEGER(1),*M)) {
                info = 11;
            }
            if (info!=0) {
                BLAS(xerbla)("STRMM ", &info);
                return;
            }
#       endif
    
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        const INTEGER  ka     = side==Left ? *M : *N;

        STrMatrixConstView  A(SFullConstView(ka, ka, _A, *LDA), upLo, diag);
        SGeMatrixView       B = SFullView(*M, *N, _B, *LDB);

#       ifdef TEST_OVERLOADED_OPERATORS
            const auto alpha = *ALPHA;

            if (side==Left) {
                if (transA==NoTrans) {
                    B = alpha*A*B;
                } else {
                    B = alpha*transpose(A)*B;
                }
            } else if (side==Right) {
                if (transA==NoTrans) {
                    B = alpha*B*A;
                } else {
                    B = alpha*B*transpose(A);
                }
            }
#       else
            blas::mm(side, transA, *ALPHA, A, B);
#       endif
#   endif
}

void
BLAS(dtrmm)(const char      *SIDE,
            const char      *UPLO,
            const char      *TRANSA,
            const char      *DIAG,
            const INTEGER   *M,
            const INTEGER   *N,
            const double    *ALPHA,
            const double    *_A,
            const INTEGER   *LDA,
            double          *_B,
            const INTEGER   *LDB)
{
#   ifdef TEST_DIRECT_CBLAS
    
        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        
        cblas_dtrmm(CBLAS_ORDER::CblasColMajor,
                    cxxblas::CBLAS::getCblasType(side),
                    cxxblas::CBLAS::getCblasType(upLo),
                    cxxblas::CBLAS::getCblasType(transA),
                    cxxblas::CBLAS::getCblasType(diag),
                    *M, *N, *ALPHA,
                    _A, *LDA, _B, *LDB);
    
#   else
    
        using std::abs;
        using std::max;

        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        INTEGER nRowA   = (_SIDE=='L') ? *M : *N;

#       ifndef NO_INPUT_CHECK
            INTEGER info  = 0;
            if (_SIDE!='L' && _SIDE!='R') {
                info = 1;
            } else if (_UPLO!='U' && _UPLO!='L') {
                info = 2;
            } else if (_TRANSA!='N' && _TRANSA!='T' && _TRANSA!='C') {
                info = 3;
            } else if (_DIAG!='U' && _DIAG!='N') {
                info = 4;
            } else if (*M<0) {
                info = 5;
            } else if (*N<0) {
                info = 6;
            } else if (*LDA<max(INTEGER(1),nRowA)) {
                info = 9;
            } else if (*LDB<max(INTEGER(1),*M)) {
                info = 11;
            }
            if (info!=0) {
                BLAS(xerbla)("DTRMM ", &info);
                return;
            }
#       endif
    
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        const INTEGER  ka     = side==Left ? *M : *N;

        DTrMatrixConstView  A(DFullConstView(ka, ka, _A, *LDA), upLo, diag);
        DGeMatrixView       B = DFullView(*M, *N, _B, *LDB);

#       ifdef TEST_OVERLOADED_OPERATORS
            const auto alpha = *ALPHA;

            if (side==Left) {
                if (transA==NoTrans) {
                    B = alpha*A*B;
                } else {
                    B = alpha*transpose(A)*B;
                }
            } else if (side==Right) {
                if (transA==NoTrans) {
                    B = alpha*B*A;
                } else {
                    B = alpha*B*transpose(A);
                }
            }
#       else
            blas::mm(side, transA, *ALPHA, A, B);
#       endif
#   endif
}

void
BLAS(ctrmm)(const char      *SIDE,
            const char      *UPLO,
            const char      *TRANSA,
            const char      *DIAG,
            const INTEGER   *M,
            const INTEGER   *N,
            const cfloat    *ALPHA,
            const cfloat    *_A,
            const INTEGER   *LDA,
            cfloat          *_B,
            const INTEGER   *LDB)
{
#   ifdef TEST_DIRECT_CBLAS
    
        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        
        cblas_ctrmm(CBLAS_ORDER::CblasColMajor,
                    cxxblas::CBLAS::getCblasType(side),
                    cxxblas::CBLAS::getCblasType(upLo),
                    cxxblas::CBLAS::getCblasType(transA),
                    cxxblas::CBLAS::getCblasType(diag),
                    *M, *N,
                    reinterpret_cast<const float *>(ALPHA),
                    reinterpret_cast<const float *>(_A), *LDA,
                    reinterpret_cast<float *>(_B), *LDB);
    
#   else
    
        using std::abs;
        using std::max;

        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        INTEGER nRowA   = (_SIDE=='L') ? *M : *N;

#       ifndef NO_INPUT_CHECK
            INTEGER info  = 0;
            if (_SIDE!='L' && _SIDE!='R') {
                info = 1;
            } else if (_UPLO!='U' && _UPLO!='L') {
                info = 2;
            } else if (_TRANSA!='N' && _TRANSA!='T' && _TRANSA!='C') {
                info = 3;
            } else if (_DIAG!='U' && _DIAG!='N') {
                info = 4;
            } else if (*M<0) {
                info = 5;
            } else if (*N<0) {
                info = 6;
            } else if (*LDA<max(INTEGER(1),nRowA)) {
                info = 9;
            } else if (*LDB<max(INTEGER(1),*M)) {
                info = 11;
            }
            if (info!=0) {
                BLAS(xerbla)("CTRMM ", &info);
                return;
            }
#       endif
    
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        const INTEGER  ka     = side==Left ? *M : *N;

        CTrMatrixConstView  A(CFullConstView(ka, ka, _A, *LDA), upLo, diag);
        CGeMatrixView       B = CFullView(*M, *N, _B, *LDB);

#       ifdef TEST_OVERLOADED_OPERATORS
            const auto alpha = *ALPHA;

            if (side==Left) {
                if (transA==NoTrans) {
                    B = alpha*A*B;
                } else if (transA==Trans) {
                    B = alpha*transpose(A)*B;
                } else if (transA==ConjTrans) {
                    B = alpha*conjTrans(A)*B;
                }
            } else if (side==Right) {
                if (transA==NoTrans) {
                    B = alpha*B*A;
                } else if (transA==Trans) {
                    B = alpha*B*transpose(A);
                } else if (transA==ConjTrans) {
                    B = alpha*B*conjTrans(A);
                }
            }
#       else
            blas::mm(side, transA, *ALPHA, A, B);
#       endif
#   endif
}

void
BLAS(ztrmm)(const char      *SIDE,
            const char      *UPLO,
            const char      *TRANSA,
            const char      *DIAG,
            const INTEGER   *M,
            const INTEGER   *N,
            const cdouble   *ALPHA,
            const cdouble   *_A,
            const INTEGER   *LDA,
            cdouble         *_B,
            const INTEGER   *LDB)
{
    
#   ifdef TEST_DIRECT_CBLAS
    
        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        
        cblas_ztrmm(CBLAS_ORDER::CblasColMajor,
                    cxxblas::CBLAS::getCblasType(side),
                    cxxblas::CBLAS::getCblasType(upLo),
                    cxxblas::CBLAS::getCblasType(transA),
                    cxxblas::CBLAS::getCblasType(diag),
                    *M, *N,
                    reinterpret_cast<const double *>(ALPHA),
                    reinterpret_cast<const double *>(_A), *LDA,
                    reinterpret_cast<double *>(_B), *LDB);
    
#   else
    
        using std::abs;
        using std::max;
    
        char    _SIDE   = toupper(*SIDE);
        char    _UPLO   = toupper(*UPLO);
        char    _TRANSA = toupper(*TRANSA);
        char    _DIAG   = toupper(*DIAG);
        INTEGER nRowA   = (_SIDE=='L') ? *M : *N;

#       ifndef NO_INPUT_CHECK
            INTEGER info  = 0;
            if (_SIDE!='L' && _SIDE!='R') {
                info = 1;
            } else if (_UPLO!='U' && _UPLO!='L') {
                info = 2;
            } else if (_TRANSA!='N' && _TRANSA!='T' && _TRANSA!='C') {
                info = 3;
            } else if (_DIAG!='U' && _DIAG!='N') {
                info = 4;
            } else if (*M<0) {
                info = 5;
            } else if (*N<0) {
                info = 6;
            } else if (*LDA<max(INTEGER(1),nRowA)) {
                info = 9;
            } else if (*LDB<max(INTEGER(1),*M)) {
                info = 11;
            }
            if (info!=0) {
                BLAS(xerbla)("ZTRMM ", &info);
                return;
            }
#       endif
    
        Side           side   = Side(_SIDE);
        StorageUpLo    upLo   = StorageUpLo(_UPLO);
        Transpose      transA = convertTo<Transpose>(_TRANSA);
        Diag           diag   = Diag(_DIAG);
        const INTEGER  ka     = side==Left ? *M : *N;

        ZTrMatrixConstView  A(ZFullConstView(ka, ka, _A, *LDA), upLo, diag);
        ZGeMatrixView       B = ZFullView(*M, *N, _B, *LDB);

#   ifdef TEST_OVERLOADED_OPERATORS
            const auto alpha = *ALPHA;

            if (side==Left) {
                if (transA==NoTrans) {
                    B = alpha*A*B;
                } else if (transA==Trans) {
                    B = alpha*transpose(A)*B;
                } else if (transA==ConjTrans) {
                    B = alpha*conjTrans(A)*B;
                }
            } else if (side==Right) {
                if (transA==NoTrans) {
                    B = alpha*B*A;
                } else if (transA==Trans) {
                    B = alpha*B*transpose(A);
                } else if (transA==ConjTrans) {
                    B = alpha*B*conjTrans(A);
                }
            }
#       else
            blas::mm(side, transA, *ALPHA, A, B);
#       endif
#   endif
}


} // extern "C"
