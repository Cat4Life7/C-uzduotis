#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"

/* ===================== MEMORY HELPERS ===================== */

Node *newNode(int digit) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->digit = digit;
    n->next = NULL;
    return n;
}

void freeBigInt(BigInt *a) {
    Node *cur = a->head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    a->head = NULL;
    a->length = 0;
}

/* Append a digit to the MOST significant end (used during construction) */
void appendDigit(BigInt *a, int digit) {
    Node *n = newNode(digit);
    if (!a->head) {
        a->head = n;
    } else {
        Node *cur = a->head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    a->length++;
}

void copyBigInt(BigInt *dest, const BigInt *src) {
    dest->head = NULL;
    dest->length = 0;
    Node *cur = src->head;
    while (cur) {
        appendDigit(dest, cur->digit);
        cur = cur->next;
    }
}

/* ===================== PAGRINDINĖS ===================== */

void initBigInt(BigInt *a) {
    a->head = newNode(0);
    a->length = 1;
}

/* str is a normal human-readable number string e.g. "12345"
   We store digits in reverse (least significant first) */
void fromString(BigInt *a, const char *str) {
    a->head = NULL;
    a->length = 0;
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        appendDigit(a, str[i] - '0');
    }
}

void printBigInt(const BigInt *a) {
    /* Collect into array for reverse printing */
    int *arr = (int *)malloc(a->length * sizeof(int));
    Node *cur = a->head;
    for (int i = 0; i < a->length; i++) {
        arr[i] = cur->digit;
        cur = cur->next;
    }
    for (int i = a->length - 1; i >= 0; i--) {
        printf("%d", arr[i]);
    }
    printf("\n");
    free(arr);
}

int compareBigInt(const BigInt *a, const BigInt *b) {
    if (a->length > b->length) return 1;
    if (a->length < b->length) return -1;

    /* Same length: compare from most significant digit */
    int *da = (int *)malloc(a->length * sizeof(int));
    int *db = (int *)malloc(b->length * sizeof(int));

    Node *ca = a->head, *cb = b->head;
    for (int i = 0; i < a->length; i++) {
        da[i] = ca->digit; ca = ca->next;
        db[i] = cb->digit; cb = cb->next;
    }

    int result = 0;
    for (int i = a->length - 1; i >= 0; i--) {
        if (da[i] > db[i]) { result = 1; break; }
        if (da[i] < db[i]) { result = -1; break; }
    }

    free(da);
    free(db);
    return result;
}

/* ===================== SUDĖTIS ===================== */

BigInt addBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;
    result.head = NULL;
    result.length = 0;

    int carry = 0;
    Node *ca = a->head, *cb = b->head;

    while (ca || cb || carry) {
        int sum = carry;
        if (ca) { sum += ca->digit; ca = ca->next; }
        if (cb) { sum += cb->digit; cb = cb->next; }
        appendDigit(&result, sum % 10);
        carry = sum / 10;
    }

    return result;
}

/* ===================== ATIMTIS ===================== */

BigInt subtractBigInt(const BigInt *a, const BigInt *b) {
    BigInt result;
    result.head = NULL;
    result.length = 0;

    int borrow = 0;
    Node *ca = a->head, *cb = b->head;

    while (ca) {
        int diff = ca->digit - borrow;
        if (cb) { diff -= cb->digit; cb = cb->next; }

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        appendDigit(&result, diff);
        ca = ca->next;
    }

    /* Remove leading zeros (trailing in our list) */
    while (result.length > 1) {
        /* Find second-to-last node */
        Node *cur = result.head;
        while (cur->next && cur->next->next) cur = cur->next;
        if (cur->next && cur->next->digit == 0) {
            free(cur->next);
            cur->next = NULL;
            result.length--;
        } else break;
    }

    return result;
}

/* ===================== DAUGYBA ===================== */

BigInt multiplyBigInt(const BigInt *a, const BigInt *b) {
    int lenA = a->length, lenB = b->length;
    int lenR = lenA + lenB;

    /* Use a temporary array for intermediate results */
    int *temp = (int *)calloc(lenR, sizeof(int));

    /* Flatten both lists into arrays */
    int *da = (int *)malloc(lenA * sizeof(int));
    int *db = (int *)malloc(lenB * sizeof(int));

    Node *ca = a->head;
    for (int i = 0; i < lenA; i++) { da[i] = ca->digit; ca = ca->next; }

    Node *cb = b->head;
    for (int i = 0; i < lenB; i++) { db[i] = cb->digit; cb = cb->next; }

    for (int i = 0; i < lenA; i++) {
        int carry = 0;
        for (int j = 0; j < lenB; j++) {
            int t = temp[i + j] + da[i] * db[j] + carry;
            temp[i + j] = t % 10;
            carry = t / 10;
        }
        temp[i + lenB] += carry;
    }

    free(da);
    free(db);

    /* Build result from temp array */
    BigInt result;
    result.head = NULL;
    result.length = 0;

    for (int i = 0; i < lenR; i++)
        appendDigit(&result, temp[i]);

    free(temp);

    /* Trim leading zeros */
    while (result.length > 1) {
        Node *cur = result.head;
        while (cur->next && cur->next->next) cur = cur->next;
        if (cur->next && cur->next->digit == 0) {
            free(cur->next);
            cur->next = NULL;
            result.length--;
        } else break;
    }

    return result;
}

/* ===================== DALYBA ===================== */

BigInt divideBigInt(const BigInt *a, const BigInt *b) {
    BigInt zero;
    initBigInt(&zero);

    if (compareBigInt(b, &zero) == 0) {
        printf("Klaida: dalyba is nulio!\n");
        freeBigInt(&zero);
        return zero;
    }

    /* Flatten a into array for digit-by-digit long division */
    int lenA = a->length;
    int *da = (int *)malloc(lenA * sizeof(int));
    Node *ca = a->head;
    for (int i = 0; i < lenA; i++) { da[i] = ca->digit; ca = ca->next; }

    int *qdigits = (int *)calloc(lenA, sizeof(int));

    BigInt remainder;
    initBigInt(&remainder);

    for (int i = lenA - 1; i >= 0; i--) {
        /* remainder = remainder * 10 + da[i] */
        BigInt ten;
        fromString(&ten, "10");
        BigInt newRem = multiplyBigInt(&remainder, &ten);
        freeBigInt(&ten);
        freeBigInt(&remainder);

        BigInt digit;
        initBigInt(&digit);
        digit.head->digit = da[i];

        remainder = addBigInt(&newRem, &digit);
        freeBigInt(&newRem);
        freeBigInt(&digit);

        int count = 0;
        while (compareBigInt(&remainder, b) >= 0) {
            BigInt newR = subtractBigInt(&remainder, b);
            freeBigInt(&remainder);
            remainder = newR;
            count++;
        }

        qdigits[i] = count;
    }

    freeBigInt(&remainder);
    free(da);

    /* Build quotient BigInt from qdigits (stored least significant first) */
    BigInt quotient;
    quotient.head = NULL;
    quotient.length = 0;
    for (int i = 0; i < lenA; i++)
        appendDigit(&quotient, qdigits[i]);

    free(qdigits);

    /* Trim leading zeros */
    while (quotient.length > 1) {
        Node *cur = quotient.head;
        while (cur->next && cur->next->next) cur = cur->next;
        if (cur->next && cur->next->digit == 0) {
            free(cur->next);
            cur->next = NULL;
            quotient.length--;
        } else break;
    }

    freeBigInt(&zero);
    return quotient;
}