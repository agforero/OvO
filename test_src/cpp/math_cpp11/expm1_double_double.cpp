
#include <cmath>

#include <limits>
#include <iostream>
#include <stdexcept>
#
using namespace std;

 
bool almost_equal(double x, double y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<double>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<double>::min();

}


void test_expm1(){
   
   double in0 {  0.42 };
   

   
   double out1_host;
   double out1_device;
   

    out1_host =  expm1( in0);
   
   #pragma omp target map(from: out1_device )
   {
     out1_device =  expm1( in0);
   }

   
   if ( !almost_equal(out1_host,out1_device,1) ) {
        std::cerr << "Host: " << out1_host << " GPU: " << out1_device << std::endl;
        throw std::runtime_error( "expm1 give incorect value when offloaded");
    }
    
}

int main()
{
    test_expm1();
}
