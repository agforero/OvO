#include <iostream>
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams()   {return 1;}
int omp_get_num_threads() {return 1;}
#endif
bool almost_equal(float x, float gold, float tol) {
        return gold * (1-tol) <= x && x <= gold * (1 + tol);
}
void test_target_teams_distribute__parallel__simd(){
 const int N0 = 512;
 const int N1 = 512;
 float counter{};
#pragma omp target teams distribute reduction(+: counter) map(tofrom: counter)
      for (int i0 = 0 ; i0 < N0 ; i0++ )
      {
#pragma omp parallel reduction(+: counter)
    {
const int num_threads = omp_get_num_threads();
#pragma omp simd reduction(+: counter)
      for (int i1 = 0 ; i1 < N1 ; i1++ )
      {
counter += float { 1.0f/num_threads };
    }
    }
    }
if ( !almost_equal(counter,float { N0*N1 }, 0.1)  ) {
    std::cerr << "Expected: " << N0*N1 << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_target_teams_distribute__parallel__simd();
}