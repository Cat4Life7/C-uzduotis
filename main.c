#include <stdio.h>
#include "bigint.h"

int main() {
    BigInt A, B, C;

    char str1[MAX_DIGITS];
    char str2[MAX_DIGITS];

    printf("Iveskite pirma ilga skaiciu: ");
    scanf("%s", str1);

    printf("Iveskite antra ilga skaiciu: ");
    scanf("%s", str2);

    fromString(&A, str1);
    fromString(&B, str2);

    printf("\nSudetis: ");
    C = addBigInt(&A, &B);
    printBigInt(&C);

    printf("Atimtis (A-B): ");
    if (compareBigInt(&A, &B) >= 0) {
        C = subtractBigInt(&A, &B);
        printBigInt(&C);
    } else {
        printf("Neigiami skaiciai nepalaikomi\n");
    }

    printf("Daugyba: ");
    C = multiplyBigInt(&A, &B);
    printBigInt(&C);

    printf("Dalyba (A/B): ");
    C = divideBigInt(&A, &B);
    printBigInt(&C);

    return 0;
}