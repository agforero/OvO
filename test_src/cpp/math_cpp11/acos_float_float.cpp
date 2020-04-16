
#include <cmath>

#include <limits>
#include <iostream>
#include <stdexcept>
#
using namespace std;

 
bool almost_equal(float x, float y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();

}


void test_acos(){
   
   float x {  0.42 };
   

   
   float o_host;
   float o_device;
   

    o_host =  acos( x);
   
   #pragma omp target map(from: o_device )
   {
     o_device =  acos( x);
   }

   
   if ( !almost_equal(o_host,o_device,1) ) {
        std::cerr << "Host: " << o_host << " GPU: " << o_device << std::endl;
        throw std::runtime_error( "acos give incorect value when offloaded");
    }
    
}

int main()
{
    test_acos();
}
