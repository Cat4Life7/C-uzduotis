#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main(void) {
    BigInt *a = bigint_from_str("99999999999999999999");
    BigInt *b = bigint_from_str("12345678901234567890");

    BigInt *sum = bigint_add(a, b);
    printf("a + b = "); bigint_print(sum); printf("\n");

    BigInt *prod = bigint_mul(a, b);
    printf("a * b = "); bigint_print(prod); printf("\n");

    bigint_free(a); bigint_free(b);
    bigint_free(sum); bigint_free(prod);
    return 0;
}