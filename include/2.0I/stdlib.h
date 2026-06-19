#ifndef __STDLIB_H__
#define __STDLIB_H__

#ifndef	NULL
#define NULL	0
#endif
typedef struct lldiv_t
{
    long long quot;
    long long rem;
} lldiv_t;

typedef struct ldiv_t
{
    long quot;
    long rem;
} ldiv_t;

lldiv_t lldiv(long long num, long long denom);
ldiv_t ldiv(long num, long denom);
#endif /* !__STDLIB_H__ */
#include <stddef.h>
extern void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));
