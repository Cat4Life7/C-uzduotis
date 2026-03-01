#ifndef BIGINT_H
#define BIGINT_H

/* Didelių skaičių ADT. Saugoma radix=256 pagrindo skaitmenų dinaminiame masyve. */

#define RADIX 256

typedef struct {
    unsigned char *digits; 
    int length;
    int sign;            
} BigInt;

/* Kūrimas */
BigInt *bigint_from_str(const char *str); /* iš skaiciu eilutės, pvz. "123456" */
BigInt *bigint_clone(const BigInt *a);  
void    bigint_free(BigInt *a);
void    bigint_make_empty(BigInt *a);     /* išvalo*/

/* Užklausos */
char *bigint_to_string(const BigInt *a);  /* Gražina kaip skaiciaus eilute tekstu*/
void  bigint_print(const BigInt *a);      /* spausdina į ekraną skaiciu*/
int   bigint_count(const BigInt *a);      /* skaitmenų kiekis */
int   bigint_is_empty(const BigInt *a);   /* 1 jei tuščias */
int   bigint_is_full(const BigInt *a);   

/* Aritmetika, grąžina rezultatą arba NULL jei klaida */
BigInt *bigint_add(const BigInt *a, const BigInt *b);
BigInt *bigint_sub(const BigInt *a, const BigInt *b);
BigInt *bigint_mul(const BigInt *a, const BigInt *b);
BigInt *bigint_div(const BigInt *a, const BigInt *b, BigInt **remainder);

#endif