#include <stdio.h>
#include "bigint.h"

int main(void) {
    BigInt *a = bigint_from_str("99999999999999999999");
    BigInt *b = bigint_from_str("1");
    BigInt *c = bigint_from_str("123456789");
    BigInt *d = bigint_from_str("987654321");
    BigInt *result = NULL, *rem = NULL;
    int err;

    err = bigint_add(a, b, &result);
    if (err) { printf("add klaida: %d\n", err); return 1; }
    printf("a + b = "); bigint_print(result); printf("\n");
    bigint_free(result);

    err = bigint_sub(a, b, &result);
    if (err) { printf("sub klaida: %d\n", err); return 1; }
    printf("a - b = "); bigint_print(result); printf("\n");
    bigint_free(result);

    err = bigint_mul(c, d, &result);
    if (err) { printf("mul klaida: %d\n", err); return 1; }
    printf("c * d = "); bigint_print(result); printf("\n");
    bigint_free(result);

    err = bigint_div(c, d, &result, &rem);
    if (err) { printf("div klaida: %d\n", err); return 1; }
    printf("c / d = "); bigint_print(result); printf("\n");
    printf("c %% d = "); bigint_print(rem);    printf("\n");
    bigint_free(result); bigint_free(rem);

    /* Dalyba iš nulio - tikrinamas klaidos kodas */
    BigInt *zero = bigint_from_int(0);
    err = bigint_div(c, zero, &result, NULL);
    printf("div iš 0: klaidos kodas = %d (laukiama %d)\n", err, BIGINT_ERR_DIV0);
    bigint_free(zero);

    bigint_free(a); bigint_free(b); bigint_free(c); bigint_free(d);
    return 0;
}