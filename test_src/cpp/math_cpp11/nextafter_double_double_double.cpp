
#include <cmath>

#include <limits>
#include <iostream>

using namespace std;

 
bool almost_equal(double x, double y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<double>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<double>::min();

}


void test_nextafter(){
   
   double in0 {  0.42 };
   
   double in1 {  0.42 };
   

   
   double out2_host;
   double out2_device;
   

   out2_host = nextafter( in0, in1);

   #pragma omp target map(from: out2_device )
   {
   out2_device = nextafter( in0, in1);
   }

   
   if ( !almost_equal(out2_host,out2_device,1) ) {
        std::cerr << "Host: " << out2_host << " GPU: " << out2_device << std::endl;
        throw std::runtime_error( "nextafter give incorect value when offloaded");
    }
    
}

int main()
{
    test_nextafter();
}
