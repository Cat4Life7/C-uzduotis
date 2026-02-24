#ifndef BIGINT_H
#define BIGINT_H

#include <stdlib.h>

/* Each node stores one digit (in reverse order, least significant first) */
typedef struct Node {
    int digit;
    struct Node *next;
} Node;

typedef struct {
    Node *head;   // least significant digit first
    int length;
} BigInt;

/* Pagrindinės funkcijos */
void initBigInt(BigInt *a);
void fromString(BigInt *a, const char *str);
void printBigInt(const BigInt *a);
int compareBigInt(const BigInt *a, const BigInt *b);

/* Aritmetika */
BigInt addBigInt(const BigInt *a, const BigInt *b);
BigInt subtractBigInt(const BigInt *a, const BigInt *b);   // kai a >= b
BigInt multiplyBigInt(const BigInt *a, const BigInt *b);
BigInt divideBigInt(const BigInt *a, const BigInt *b);     // tik dalmuo

/* Memory management */
void freeBigInt(BigInt *a);
Node *newNode(int digit);
void appendDigit(BigInt *a, int digit);   // appends to least-significant end
void copyBigInt(BigInt *dest, const BigInt *src);

#endif