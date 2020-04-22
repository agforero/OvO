#include <cmath>
#include <limits>
#include <iostream>
#include <stdexcept>
#
using namespace std;
bool almost_equal(float x, float y, int ulp) {
     return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();
}
void test_acosh(){
   float x {  4.42 };
   float o_host;
   float o_device;
    o_host =  acosh( x);
   #pragma omp target map(from: o_device )
   {
     o_device =  acosh( x);
   }
   if ( !almost_equal(o_host,o_device,4) ) {
        std::cerr << "Host: " << o_host << " GPU: " << o_device << std::endl;
        throw std::runtime_error( "acosh give incorect value when offloaded");
    }
}
int main()
{
    test_acosh();
}
