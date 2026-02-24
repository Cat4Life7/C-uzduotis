#include <stdio.h>
#include <string.h>
#include "bigint.h"

void initBigInt(BigInt *a) {
    a->length = 1;
    a->digits[0] = 0;
}

void fromString(BigInt *a, const char *str) {
    int len = strlen(str);
    a->length = len;

    for (int i = 0; i < len; i++) {
        a->digits[i] = str[len - 1 - i] - '0';
    }
}

void printBigInt(const BigInt *a) {
    for (int i = a->length - 1; i >= 0; i--) {
        printf("%d", a->digits[i]);
    }
    printf("\n");
}

int compareBigInt(const BigInt *a, const BigInt *b) {
    if (a->length > b->length) return 1;
    if (a->length < b->length) return -1;

    for (int i = a->length - 1; i >= 0; i--) {
        if (a->digits[i] > b->digits[i]) return 1;
        if (a->digits[i] < b->digits[i]) return -1;
    }
    return 0;
}

/* ===================== SUDĖTIS ===================== */

BigInt addBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;
    result.length = 0;

    int carry = 0;
    int maxLen = (a->length > b->length) ? a->length : b->length;

    for (int i = 0; i < maxLen || carry; i++) {
        int sum = carry;

        if (i < a->length) sum += a->digits[i];
        if (i < b->length) sum += b->digits[i];

        result.digits[result.length++] = sum % 10;
        carry = sum / 10;
    }

    return result;
}

/* ===================== ATIMTIS ===================== */

BigInt subtractBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;
    result.length = a->length;

    int borrow = 0;

    for (int i = 0; i < a->length; i++) {
        int diff = a->digits[i] - borrow;

        if (i < b->length)
            diff -= b->digits[i];

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.digits[i] = diff;
    }

    while (result.length > 1 &&
           result.digits[result.length - 1] == 0)
        result.length--;

    return result;
}

/* ===================== DAUGYBA ===================== */

BigInt multiplyBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;
    result.length = a->length + b->length;

    for (int i = 0; i < result.length; i++)
        result.digits[i] = 0;

    for (int i = 0; i < a->length; i++) {
        int carry = 0;

        for (int j = 0; j < b->length; j++) {
            int temp = result.digits[i + j] +
                       a->digits[i] * b->digits[j] + carry;

            result.digits[i + j] = temp % 10;
            carry = temp / 10;
        }

        result.digits[i + b->length] += carry;
    }

    while (result.length > 1 &&
           result.digits[result.length - 1] == 0)
        result.length--;

    return result;
}

/* ===================== DALYBA ===================== */

BigInt divideBigInt(const BigInt *a, const BigInt *b) {
    BigInt zero;
    initBigInt(&zero);

    if (compareBigInt(b, &zero) == 0) {
        printf("Klaida: dalyba is nulio!\n");
        return zero;
    }

    BigInt quotient;
    BigInt remainder;

    initBigInt(&quotient);
    initBigInt(&remainder);

    quotient.length = a->length;

    for (int i = a->length - 1; i >= 0; i--) {

        /* remainder = remainder * 10 */
        for (int j = remainder.length; j > 0; j--)
            remainder.digits[j] = remainder.digits[j - 1];

        remainder.digits[0] = a->digits[i];
        remainder.length++;

        while (remainder.length > 1 &&
               remainder.digits[remainder.length - 1] == 0)
            remainder.length--;

        int count = 0;

        while (compareBigInt(&remainder, b) >= 0) {
            remainder = subtractBigInt(&remainder, b);
            count++;
        }

        quotient.digits[i] = count;
    }

    while (quotient.length > 1 &&
           quotient.digits[quotient.length - 1] == 0)
        quotient.length--;

    return quotient;
}