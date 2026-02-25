#ifndef BIGINT_H
#define BIGINT_H

#define RADIX 256

/* Klaidų kodai */
#define BIGINT_OK       0
#define BIGINT_ERR_NULL 1  /* NULL rodyklė */
#define BIGINT_ERR_DIV0 2  /* dalyba iš nulio */
#define BIGINT_ERR_MEM  3  /* nepakanka atminties */

typedef struct {
    unsigned char *digits; /* skaitmenys radix=256, digits[0] = jauniausias */
    int length;
    int sign; /* 1 arba -1 */
} BigInt;

BigInt *bigint_from_int(long long value);
BigInt *bigint_from_str(const char *str);
BigInt *bigint_copy(const BigInt *a);
void    bigint_free(BigInt *a);
void    bigint_print(const BigInt *a);
int     bigint_cmp(const BigInt *a, const BigInt *b);

/* Aritmetikos funkcijos grąžina BIGINT_OK arba klaidos kodą;
   rezultatas rašomas į *out (NULL jei klaida) */
int bigint_add(const BigInt *a, const BigInt *b, BigInt **out);
int bigint_sub(const BigInt *a, const BigInt *b, BigInt **out);
int bigint_mul(const BigInt *a, const BigInt *b, BigInt **out);
int bigint_div(const BigInt *a, const BigInt *b, BigInt **out, BigInt **remainder);

#endif