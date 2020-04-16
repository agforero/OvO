#include <iostream>
#include <limits>
#include <cmath>
#include <stdexcept>

#include <complex>
using namespace std;



#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams() {return 1;}
int omp_get_num_threads() {return 1;}
#endif


bool almost_equal(complex<double> x, complex<double> y, int ulp) {

    bool r = std::fabs(x.real()-y.real()) <= std::numeric_limits<double>::epsilon() * std::fabs(x.real()+y.real()) * ulp ||  std::fabs(x.real()-y.real()) < std::numeric_limits<double>::min();
    bool i = std::fabs(x.imag()-y.imag()) <= std::numeric_limits<double>::epsilon() * std::fabs(x.imag()+y.imag()) * ulp ||  std::fabs(x.imag()-y.imag()) < std::numeric_limits<double>::min();
    return r && i;

}


#pragma omp declare reduction(+: complex<double>:   omp_out += omp_in) 


void test_target_parallel(){

 // Input and Outputs
 

complex<double> counter{};

// Main program

#pragma omp target parallel  reduction(+: counter)   map(tofrom:counter) 

{

const int num_threads = omp_get_num_threads();




counter += complex<double> { 1.0f/num_threads };



}


// Validation
if ( !almost_equal(counter,complex<double> { 1 }, 10)  ) {
    std::cerr << "Expected: " << 1 << " Got: " << counter << std::endl;
    throw std::runtime_error( "target_parallel give incorect value when offloaded");
}

}
int main()
{
    test_target_parallel();
}
