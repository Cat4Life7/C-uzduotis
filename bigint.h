#ifndef BIGINT_H
#define BIGINT_H

#define MAX_DIGITS 1000

typedef struct {
    int digits[MAX_DIGITS];   // skaitmenys (atvirkštine tvarka)
    int length;               // kiek skaitmenų
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

#endif