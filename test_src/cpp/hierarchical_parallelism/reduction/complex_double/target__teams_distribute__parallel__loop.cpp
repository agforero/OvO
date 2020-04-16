#include <iostream>
#include <limits>
#include <cmath>
#include <stdexcept>

#include <complex>
using namespace std;




bool almost_equal(complex<double> x, complex<double> y, int ulp) {

    bool r = std::fabs(x.real()-y.real()) <= std::numeric_limits<double>::epsilon() * std::fabs(x.real()+y.real()) * ulp ||  std::fabs(x.real()-y.real()) < std::numeric_limits<double>::min();
    bool i = std::fabs(x.imag()-y.imag()) <= std::numeric_limits<double>::epsilon() * std::fabs(x.imag()+y.imag()) * ulp ||  std::fabs(x.imag()-y.imag()) < std::numeric_limits<double>::min();
    return r && i;

}


#pragma omp declare reduction(+: complex<double>:   omp_out += omp_in) 


void test_target__teams_distribute__parallel__loop(){

 // Input and Outputs
 
 const int L = 5;
 const int M = 6;

complex<double> counter{};

// Main program

#pragma omp target   map(tofrom:counter) 

{


#pragma omp teams distribute  reduction(+: counter)  

    for (int i = 0 ; i < L ; i++ )

{


#pragma omp parallel  reduction(+: counter)  

{


#pragma omp loop  reduction(+: counter)  

    for (int j = 0 ; j < M ; j++ )

{




counter += complex<double> { 1.0f };



}

}

}

}


// Validation
if ( !almost_equal(counter,complex<double> { L*M }, 10)  ) {
    std::cerr << "Expected: " << L*M << " Got: " << counter << std::endl;
    throw std::runtime_error( "target__teams_distribute__parallel__loop give incorect value when offloaded");
}

}
int main()
{
    test_target__teams_distribute__parallel__loop();
}
