#ifndef BIGINT_H
#define BIGINT_H


#define RADIX 256

#define BIGINT_OK       0  /* success                    */
#define BIGINT_ERR_NULL 1  /* NULL pointer passed in     */
#define BIGINT_ERR_DIV0 2  /* division by zero           */
#define BIGINT_ERR_MEM  3  /* memory allocation failed   */

typedef struct {
    unsigned char *digits; /* base-256 digits, least significant first */
    int length;
    int sign;              /* 1 = positive/zero, -1 = negative */
} BigInt;

/* --- Construction / destruction --- */
BigInt *bigint_from_int(long long value);
BigInt *bigint_from_str(const char *str);  /* decimal string, e.g. "-12345" */
BigInt *bigint_copy(const BigInt *a);
void    bigint_free(BigInt *a);            /* safe to call with NULL */

/* --- Output --- */
void    bigint_print(const BigInt *a);     /* prints decimal to stdout */
char   *bigint_to_str(const BigInt *a);    /* returns decimal string; caller must free() */

/* --- Comparison --- returns -1, 0, or 1 --- */
int     bigint_cmp(const BigInt *a, const BigInt *b);

/* --- Arithmetic --- all write result to *out, return error code --- */
int bigint_add(const BigInt *a, const BigInt *b, BigInt **out);
int bigint_sub(const BigInt *a, const BigInt *b, BigInt **out);
int bigint_mul(const BigInt *a, const BigInt *b, BigInt **out);
int bigint_div(const BigInt *a, const BigInt *b, BigInt **out, BigInt **remainder);

#endif