#include <stdio.h>
#include <math.h>
#include <time.h>
#include "stc/cbits.h"
#include "stc/algorithm.h"

cbits sieveOfEratosthenes(intptr_t n)
{
    cbits bits = cbits_with_size(n/2 + 1, true);
    int q = (int)sqrt((double) n) + 1;
    for (int i = 3; i < q; i += 2) {
        for (intptr_t j = i; j < n; j += 2) {
            if (cbits_test(&bits, j/2)) {
                i = j;
                break;
            }
        }
        for (intptr_t j = i*i; j < n; j += i*2)
            cbits_reset(&bits, j/2);
    }
    return bits;
}

int main(void)
{
    int n = 100000000;
    printf("Computing prime numbers up to %d\n", n);

    clock_t t = clock();
    cbits primes = sieveOfEratosthenes(n + 1);

    int np = cbits_count(&primes);
    t = clock() - t;

    printf("Number of primes: %d, time: %f\n\n", np, (double)t/CLOCKS_PER_SEC);

    puts("Show all the primes in the range [2, 1000):");
    printf("2");
    c_forrange (i, 3, 1000, 2)
        if (cbits_test(&primes, i/2)) printf(" %d", (int)i);
    puts("\n");

    puts("Show the last 50 primes using a temporary crange generator:");
    crange range = crange_make(n - 1, 0, -2);

    c_forfilter (i, crange, range,
        cbits_test(&primes, *i.ref/2) &&
        c_flt_take(i, 50)
    ){
        printf("%d ", (int)*i.ref);
        if (c_flt_getcount(i) % 10 == 0) puts("");
    }

    cbits_drop(&primes);
}
