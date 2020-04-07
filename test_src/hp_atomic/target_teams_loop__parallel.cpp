#include <iostream>
#include <stdexcept>
#include <omp.h>

#include <cmath>
#include <limits>
template<class T>
bool almost_equal(T x, T y, int ulp) {
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
void test_target_teams_loop__parallel(){

 // Input and Outputs
 
 const int L = 5;

T counter{};

// Main program

#pragma omp target teams loop  map(tofrom: counter) 

    for (int i = 0 ; i < L ; i++ )

{



#pragma omp parallel 

{


const int num_threads = omp_get_num_threads();



#pragma omp atomic update

counter = counter + 1./num_threads;



}

}


// Validation
if ( !almost_equal(counter,T{ L }, 1)  ) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    throw std::runtime_error( "target_teams_loop__parallel give incorect value when offloaded");
}

}
int main()
{
    test_target_teams_loop__parallel<double>();
}
