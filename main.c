#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main(void) {
    BigInt *a = bigint_from_str("-100000000000000000");
    BigInt *b = bigint_from_str("987654321");
    BigInt *result = NULL;

    bigint_mul(a, b, &result);

    printf("print:  "); bigint_print(result); printf("\n");
    char *s = bigint_to_str(result);
    printf("string: %s\n", s);
    free(s); 

    bigint_free(a); bigint_free(b); bigint_free(result);
    return 0;
}