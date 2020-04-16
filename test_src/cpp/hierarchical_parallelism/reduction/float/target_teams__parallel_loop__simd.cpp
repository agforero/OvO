#include <iostream>
#include <limits>
#include <cmath>
#include <stdexcept>



#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams() {return 1;}
int omp_get_num_threads() {return 1;}
#endif


bool almost_equal(float x, float y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();

}



void test_target_teams__parallel_loop__simd(){

 // Input and Outputs
 
 const int L = 5;
 const int M = 6;

float counter{};

// Main program

#pragma omp target teams  reduction(+: counter)   map(tofrom:counter) 

{

const int num_teams = omp_get_num_teams();


#pragma omp parallel loop  reduction(+: counter)  

    for (int i = 0 ; i < L ; i++ )

{


#pragma omp simd  reduction(+: counter)  

    for (int j = 0 ; j < M ; j++ )

{




counter += float { 1.0f/num_teams } ;



}

}

}


// Validation
if ( !almost_equal(counter,float { L*M }, 10)  ) {
    std::cerr << "Expected: " << L*M << " Got: " << counter << std::endl;
    throw std::runtime_error( "target_teams__parallel_loop__simd give incorect value when offloaded");
}

}
int main()
{
    test_target_teams__parallel_loop__simd();
}
