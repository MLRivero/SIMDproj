#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <sys/time.h>
#include <smmintrin.h>

#define N 600000


double gettime(void)

{

    struct timeval tv;

    gettimeofday(&tv,NULL);

    return tv.tv_sec + 1e-6*tv.tv_usec;

}


int main(int argc, char *argv[])

{

    float r[N], a[N], b[N], maxvector[4];

    float max = 0.0;

    int i;

    double dtime;


    // Initialize input matrices

    for (i = 0; i < N; i++)

    {

       a[i] = (float) i;

       b[N-i-1] = (float) 2*i;

    }


    // Loop

    dtime = gettime();


/*    for (i = 0; i < N; i++)

    {

       r[i] = sqrtf(a[i]*a[i] + b[i]*b[i]) + 0.5;

       if (r[i] > max) max = r[i];

    }
*/
    __m128 matmax;
    for (i = 0; i < N; i+=4) {
       __m128 av = _mm_load_ps(&a[i]);
       __m128 bv = _mm_load_ps(&b[i]);
              av = _mm_mul_ps(av,av);
	      bv = _mm_mul_ps(bv,bv);
       __m128 vfinal = _mm_add_ps(av,bv);
              vfinal = _mm_sqrt_ps(vfinal);
	      vfinal = _mm_add_ps(vfinal, _mm_set1_ps(0.5));
              matmax = _mm_max_ps(vfinal, matmax);
	      _mm_store_ps(&r[i],vfinal);

    }
    
    
    _mm_store_ps(&maxvector[0],matmax);

    for (i = 0; i<4; i++) {
    if (maxvector[i] > max) {
       max = maxvector[i];
    }
    }
    dtime = gettime() - dtime;


    // Print results

    printf("r: %f %f %f %f %f %f\n",r[0],r[1],r[2],r[3],r[1000],r[1001]);

    printf("max: %f\n",max);

    printf("Exec time: %9.5f sec.\n",dtime);

}
