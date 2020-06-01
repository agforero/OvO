#include <iostream>
#include <cstdlib>
#include <cmath>
#include <complex>
using namespace std;
bool almost_equal(complex<double> x, complex<double> gold, float tol) {
        return abs(gold) * (1-tol) <= abs(x) && abs(x) <= abs(gold) * (1 + tol);
}
#ifndef _NO_UDR
#pragma omp declare reduction(+: complex<double>: omp_out += omp_in)
#endif
void test_parallel__for__target__teams__distribute__parallel__for(){
 const int L = 64;
 const int M = 64;
 const int N = 64;
 complex<double> counter{};
#pragma omp parallel reduction(+: counter)
#pragma omp for
    for (int i = 0 ; i < L ; i++ )
    {
#pragma omp target map(tofrom: counter) 
#pragma omp teams reduction(+: counter)
#pragma omp distribute
    for (int j = 0 ; j < M ; j++ )
    {
#pragma omp parallel reduction(+: counter)
#pragma omp for
    for (int k = 0 ; k < N ; k++ )
    {
counter += complex<double> { 1.0f };
    }
    }
    }
if ( !almost_equal(counter,complex<double> { L*M*N }, 0.1)  ) {
    std::cerr << "Expected: " << L*M*N << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_parallel__for__target__teams__distribute__parallel__for();
}