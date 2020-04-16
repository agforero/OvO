#include <iostream>
#include <limits>
#include <cmath>
#include <stdexcept>
#



#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams() {return 1;}
int omp_get_num_threads() {return 1;}
#endif


bool almost_equal(float x, float y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();

}

void test_target__teams__parallel(){

 // Input and Outputs
 

float counter{};

// Main program

#pragma omp target  map(tofrom:counter) 

{


#pragma omp teams 

{

const int num_teams = omp_get_num_teams();


#pragma omp parallel 

{

const int num_threads = omp_get_num_threads();



#pragma omp atomic update

counter += float { 1.0f } / (num_teams*num_threads)  ;



}

}

}


// Validation
if ( !almost_equal(counter,float { 1 }, 10)  ) {
    std::cerr << "Expected: " << 1 << " Got: " << counter << std::endl;
    throw std::runtime_error( "target__teams__parallel give incorect value when offloaded");
}

}
int main()
{
    test_target__teams__parallel();
}
