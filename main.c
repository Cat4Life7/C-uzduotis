#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main(void) {
    BigInt *a = bigint_from_str("99999999999999999999");
    BigInt *b = bigint_from_str("12345678901234567890");
    BigInt *rem = NULL;

    printf("a     = "); bigint_print(a); printf("\n");
    printf("b     = "); bigint_print(b); printf("\n\n");

    printf("a + b = "); bigint_print(bigint_add(a, b)); printf("\n");
    printf("a - b = "); bigint_print(bigint_sub(a, b)); printf("\n");
    printf("a * b = "); bigint_print(bigint_mul(a, b)); printf("\n");
    printf("a / b = "); bigint_print(bigint_div(a, b, &rem));
    printf(" , a %% b = "); bigint_print(rem); printf("\n");

    bigint_free(a);
    bigint_free(b);
    bigint_free(rem);
    return 0;
}