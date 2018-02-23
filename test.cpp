#include <eigen3/Eigen/SparseCore>
#include <eigen3/unsupported/Eigen/KroneckerProduct>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <memory>
#include <limits>
#include <cmath>

#include "templated-arpack.hxx"

using std::sin;
using std::sqrt;
using Real=double;
constexpr Real PI=3.1415926535897932384626433832795029L;
using SparseMatrix=Eigen::SparseMatrix<Real>;
using DenseMatrix=Eigen::Matrix<Real,Eigen::Dynamic,Eigen::Dynamic>;

Real sqr(Real x) { return x*x; }

Real opT(const int xiZeroBased, const int xi1ZeroBased, const int N, const Real w)
{
    const auto xi=xiZeroBased+1, xi1=xi1ZeroBased+1;

    const Real m=N+1;
    const auto coef=sqr(PI)/(2*sqr(w));
    if(xi!=xi1)
    {
        const auto cscDif=1/sqr(sin(PI*(xi-xi1)/(2*m)))-1/sqr(sin(PI*(xi+xi1)/(2*m)));
        if((xi-xi1)%2)
            return -coef*cscDif;
        else
            return +coef*cscDif;
    }
    else
    {
        return coef*(2*sqr(m)-3/sqr(sin(PI*xi/m))+1)/3;
    }
}

SparseMatrix genT1D(const int N, const Real w)
{
    SparseMatrix T(N,N);
    std::vector<Eigen::Triplet<Real>> triplets;
    triplets.reserve(N*N);
    for(int xi=0;xi<N;++xi)
        for(int xi1=0;xi1<N;++xi1)
            T.coeffRef(xi,xi1)=opT(xi,xi1,N,w);
    T.makeCompressed();
    return T;
}

SparseMatrix makeH()
{
    const int N=50;
    const Real aMax=5;
    const Real W1=2*aMax, W2=W1;

    const auto T1d=genT1D(N,W1);
    SparseMatrix I(N,N); I.setIdentity();
    SparseMatrix Tlhs, Trhs;
    Eigen::kroneckerProduct(T1d, I).evalTo(Tlhs);
    Eigen::kroneckerProduct(I, T1d).evalTo(Trhs);
    return Tlhs+Trhs;
}

class SparseSymMatProd
{
    SparseMatrix const& m;
public:
    SparseSymMatProd(SparseMatrix const& m)
        : m(m)
    {}
    int rows() const { return m.rows(); }
    int cols() const { return m.cols(); }
    void perform_op(Real const*const xIn, Real*const yOut) const
    {
        using Vec=Eigen::Matrix<Real, Eigen::Dynamic, 1>;
        Eigen::Map<const Vec> x(xIn, m.cols());
        Eigen::Map<Vec> y(yOut, m.rows());
        y.noalias()=m.template selfadjointView<Eigen::Lower>()*x;
    }
};

Real stDev(DenseMatrix const& v)
{
    const auto N=v.rows();
    Real sumOfSquares=0;
    for(int i=0;i<N;++i)
        sumOfSquares+=sqr(v(i));
    sumOfSquares/=N-1;
    return sqrt(sumOfSquares);
}

int main(int argc, char** argv)
{
    std::cout.precision(std::numeric_limits<Real>::digits10);
    const auto H = makeH();
    SparseSymMatProd matrixOperator(H);

    // Parameters are documented in dsaupd.f
    Real tolerance=0.; // 0. -> machine precision //std::numeric_limits<Real>::epsilon();
    char bmat='I'; // standard problem: A*x=lambda*x
    char which[]="SA"; // largest magnitude
    integer nev=3; // number of eigenvalues
    integer ncv=6; // size of Arnoldi basis
    integer n=H.cols();
    std::vector<Real> resid(n); // initial vector (or randomized if initial info==0)
    integer ldv=n;
    std::vector<Real> v(ldv*ncv); // Lanczos basis vectors
    integer iparam[11]={};
    iparam[0]=1; // exact shift strategy
    iparam[2]=10000; // max Arnoldi iterations
    iparam[6]=1; // mode 1 of DSAUPD
    std::vector<Real> workd(3*n); // working data which is used for input and output for matrix-vector multiplication
    std::vector<Real> workl(ncv*(ncv+8)); // DSAUPD workspace
    integer lworkl=workl.size();
    integer ido=0; // reverse communication parameter
    integer info=0; // request random initial vector
    integer ipntr[11]={};

    ARPACK<Real> arpack;
    do
    {
        arpack.dsaupd_(&ido, &bmat, &n, which, &nev, &tolerance, resid.data(), &ncv,
               v.data(), &ldv, iparam, ipntr, workd.data(),
               workl.data(), &lworkl, &info, sizeof bmat, sizeof which);
        if(!(ido==-1 || ido==1))
            break;
        matrixOperator.perform_op(&workd[ipntr[0]-1], &workd[ipntr[1]-1]);
    }while(true);

    if(info<0)
    {
        std::cerr << "dsaupd failed, info=" << info << "\n";
        return 1;
    }

    logical wantEigenvectors=true;
    char howmny[]="All";
    std::vector<integer> select(ncv);
    Real sigma; // shift, not used
    std::vector<Real> d(nev); // eigenvalues
    integer ldz=n;
    std::vector<Real> z(ldz*nev); // orthonormal Ritz values
    arpack.dseupd_(&wantEigenvectors, howmny, select.data(), d.data(), z.data(), &ldz, &sigma,
           &bmat, &n, which, &nev, &tolerance, resid.data(), &ncv, v.data(),
           &ldv, iparam, ipntr, workd.data(), workl.data(), &lworkl, &info,
           sizeof howmny, sizeof bmat, sizeof which);
    if(info!=0)
    {
        std::cerr << "dseupd failed, info=" << info << "\n";
        return 1;
    }
    const auto niter=iparam[2];
    const auto nconv=iparam[4];
    std::cout << "nconv: " << nconv << ", iterations taken: " << niter << "\n";

    std::vector<Real> evals(d.data(), d.data()+nconv);

    std::cout << "Eigenvalues found:\n";
    for(const auto v : evals)
       std::cout << v << '\n';

    DenseMatrix evecs(n,nconv);
    for(int i=0;i<nconv;++i)
    {
        Real*const vec=z.data()+i*n;
        std::copy(vec, vec+n, evecs.data()+i*n);
    }

    std::cout << "Standard deviations of H*v-lambda*v for computed eigenvectors:\n";
    const auto Hv=H*evecs;
    for(int i=0;i<evals.size();++i)
        std::cout << stDev(Hv.col(i)-evals[i]*evecs.col(i)) << "\n";
}
