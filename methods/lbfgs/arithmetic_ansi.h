#include <cstdlib>
#include <memory>

#if     LBFGS_FLOAT == 32 && LBFGS_IEEE_FLOAT
#define fsigndiff(x, y) (((*(uint32_t*)(x)) ^ (*(uint32_t*)(y))) & 0x80000000U)
#else
#define fsigndiff(x, y) (*(x) * (*(y) / fabs(*(y))) < 0.)
#endif/*LBFGS_IEEE_FLOAT*/

inline static void* vecalloc(size_t size)
{
    void *memblock = malloc(size);
    if (memblock) {
        memset(memblock, 0, size);
    }
    return memblock;
}

inline static void vecfree(void *memblock)
{
    free(memblock);
}

inline static void vecset(lbfgsfloatval_t *x, const lbfgsfloatval_t c, const int n)
{
    int i;
    
    for (i = 0;i < n;++i) {
        x[i] = c;
    }
}

inline static void veccpy(lbfgsfloatval_t *y, const lbfgsfloatval_t *x, const int n)
{
    int i;

    for (i = 0;i < n;++i) {
        y[i] = x[i];
    }
}

inline static void vecncpy(lbfgsfloatval_t *y, const lbfgsfloatval_t *x, const int n)
{
    int i;

    for (i = 0;i < n;++i) {
        y[i] = -x[i];
    }
}

inline static void vecadd(lbfgsfloatval_t *y, const lbfgsfloatval_t *x, const lbfgsfloatval_t c, const int n)
{
    int i;

    for (i = 0;i < n;++i) {
        y[i] += c * x[i];
    }
}

inline static void vecdiff(lbfgsfloatval_t *z, const lbfgsfloatval_t *x, const lbfgsfloatval_t *y, const int n)
{
    int i;

    for (i = 0;i < n;++i) {
        z[i] = x[i] - y[i];
    }
}

inline static void vecscale(lbfgsfloatval_t *y, const lbfgsfloatval_t c, const int n)
{
    int i;

    for (i = 0;i < n;++i) {
        y[i] *= c;
    }
}

inline static void vecmul(lbfgsfloatval_t *y, const lbfgsfloatval_t *x, const int n)
{
    int i;

    for (i = 0;i < n;++i) {
        y[i] *= x[i];
    }
}

inline static void vecdot(lbfgsfloatval_t* s, const lbfgsfloatval_t *x, const lbfgsfloatval_t *y, const int n)
{
    int i;
    *s = 0.;
    for (i = 0;i < n;++i) {
        *s += x[i] * y[i];
    }
}

inline static void vec2norm(lbfgsfloatval_t* s, const lbfgsfloatval_t *x, const int n)
{
    vecdot(s, x, x, n);
    *s = (lbfgsfloatval_t)sqrt(*s);
}

inline static void vec2norminv(lbfgsfloatval_t* s, const lbfgsfloatval_t *x, const int n)
{
    vec2norm(s, x, n);
    *s = (lbfgsfloatval_t)(1.0 / *s);
}
