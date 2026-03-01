#include "bigint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static BigInt *bigint_alloc(int length)
{
    BigInt *n = malloc(sizeof(BigInt));
    n->digits = calloc(length, 1);
    n->length = length;
    n->sign = 1;
    return n;
}

static void bigint_trim(BigInt *a)
{
    while (a->length > 1 && a->digits[a->length - 1] == 0)
        a->length--;
    if (a->length == 1 && a->digits[0] == 0)
        a->sign = 1;
}

static int cmp_abs(const BigInt *a, const BigInt *b)
{
    if (a->length != b->length)
        return a->length > b->length ? 1 : -1;
    for (int i = a->length - 1; i >= 0; i--)
        if (a->digits[i] != b->digits[i])
            return a->digits[i] > b->digits[i] ? 1 : -1;
    return 0;
}

static BigInt *add_abs(const BigInt *a, const BigInt *b)
{
    int len = (a->length > b->length ? a->length : b->length) + 1;
    BigInt *result = bigint_alloc(len);
    int carry = 0;
    for (int i = 0; i < len; i++)
    {
        int sum = (i < a->length ? a->digits[i] : 0) + (i < b->length ? b->digits[i] : 0) + carry;
        result->digits[i] = sum % RADIX;
        carry = sum / RADIX;
    }
    bigint_trim(result);
    return result;
}

static BigInt *sub_abs(const BigInt *a, const BigInt *b)
{
    BigInt *result = bigint_alloc(a->length);
    int borrow = 0;
    for (int i = 0; i < a->length; i++)
    {
        int diff = a->digits[i] - (i < b->length ? b->digits[i] : 0) - borrow;
        if (diff < 0)
        {
            diff += RADIX;
            borrow = 1;
        }
        else
            borrow = 0;
        result->digits[i] = diff;
    }
    bigint_trim(result);
    return result;
}

static BigInt *bigint_from_byte(unsigned char value)
{
    BigInt *n = bigint_alloc(1);
    n->digits[0] = value;
    return n;
}

BigInt *bigint_from_str(const char *str)
{
    if (!str || !*str)
        return bigint_from_byte(0);
    int sign = 1;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
        str++;
    while (*str == '0' && str[1])
        str++;

    BigInt *result = bigint_from_byte(0);
    BigInt *ten = bigint_from_byte(10);
    for (; *str; str++)
    {
        if (!isdigit((unsigned char)*str))
            break;
        BigInt *tmp = bigint_mul(result, ten);
        bigint_free(result);
        BigInt *digit = bigint_from_byte((unsigned char)(*str - '0'));
        result = bigint_add(tmp, digit);
        bigint_free(tmp);
        bigint_free(digit);
    }
    bigint_free(ten);
    result->sign = sign;
    bigint_trim(result);
    return result;
}

BigInt *bigint_copy(const BigInt *a)
{
    BigInt *n = bigint_alloc(a->length);
    memcpy(n->digits, a->digits, a->length);
    n->sign = a->sign;
    return n;
}

void bigint_free(BigInt *a)
{
    if (!a)
        return;
    free(a->digits);
    free(a);
}

char *bigint_to_str(const BigInt *a)
{
    BigInt *tmp = bigint_copy(a);
    tmp->sign = 1;
    BigInt *ten = bigint_from_byte(10);
    BigInt *zero = bigint_from_byte(0);
    char *buf = malloc(a->length * 3 + 4);
    int pos = 0;
    while (bigint_cmp(tmp, zero) != 0)
    {
        BigInt *rem = NULL;
        BigInt *q = bigint_div(tmp, ten, &rem);
        buf[pos++] = '0' + rem->digits[0];
        bigint_free(rem);
        bigint_free(tmp);
        tmp = q;
    }
    bigint_free(tmp);
    bigint_free(ten);
    bigint_free(zero);
    if (pos == 0)
        buf[pos++] = '0';
    if (a->sign == -1)
        buf[pos++] = '-';
    buf[pos] = '\0';
    for (int i = 0, j = pos - 1; i < j; i++, j--)
    {
        char c = buf[i];
        buf[i] = buf[j];
        buf[j] = c;
    }
    return buf;
}

void bigint_print(const BigInt *a)
{
    char *s = bigint_to_str(a);
    printf("%s", s);
    free(s);
}

int bigint_cmp(const BigInt *a, const BigInt *b)
{
    if (a->sign != b->sign)
        return a->sign > b->sign ? 1 : -1;
    int c = cmp_abs(a, b);
    return a->sign == 1 ? c : -c;
}

BigInt *bigint_add(const BigInt *a, const BigInt *b)
{
    if (!a || !b)
        return NULL;
    BigInt *result;
    if (a->sign == b->sign)
    {
        result = add_abs(a, b);
        result->sign = a->sign;
    }
    else
    {
        int c = cmp_abs(a, b);
        if (c == 0)
            return bigint_from_byte(0);
        result = c > 0 ? sub_abs(a, b) : sub_abs(b, a);
        result->sign = c > 0 ? a->sign : b->sign;
    }
    bigint_trim(result);
    return result;
}

BigInt *bigint_sub(const BigInt *a, const BigInt *b)
{
    if (!a || !b)
        return NULL;
    BigInt *neg_b = bigint_copy(b);
    neg_b->sign = (b->length == 1 && b->digits[0] == 0) ? 1 : -b->sign;
    BigInt *result = bigint_add(a, neg_b);
    bigint_free(neg_b);
    return result;
}

BigInt *bigint_mul(const BigInt *a, const BigInt *b)
{
    if (!a || !b)
        return NULL;
    BigInt *result = bigint_alloc(a->length + b->length);
    for (int i = 0; i < a->length; i++)
    {
        int carry = 0;
        for (int j = 0; j < b->length; j++)
        {
            int prod = a->digits[i] * b->digits[j] + result->digits[i + j] + carry;
            result->digits[i + j] = prod % RADIX;
            carry = prod / RADIX;
        }
        result->digits[i + b->length] += carry;
    }
    result->sign = (a->sign == b->sign) ? 1 : -1;
    bigint_trim(result);
    return result;
}

BigInt *bigint_div(const BigInt *a, const BigInt *b, BigInt **remainder)
{
    if (!a || !b)
        return NULL;
    if (b->length == 1 && b->digits[0] == 0)
        return NULL;

    if (cmp_abs(a, b) < 0)
    {
        if (remainder)
            *remainder = bigint_copy(a);
        return bigint_from_byte(0);
    }

    BigInt *quot = bigint_alloc(a->length);
    BigInt *rem = bigint_from_byte(0);

    for (int i = a->length - 1; i >= 0; i--)
    {

        BigInt *next = bigint_alloc(rem->length + 1);
        for (int k = 0; k < rem->length; k++)
            next->digits[k + 1] = rem->digits[k];
        next->digits[0] = a->digits[i];
        bigint_free(rem);
        bigint_trim(next);
        rem = next;

        int qd = 0;
        while (cmp_abs(rem, b) >= 0)
        {
            BigInt *new_rem = sub_abs(rem, b);
            bigint_free(rem);
            rem = new_rem;
            qd++;
        }
        quot->digits[i] = qd;
    }

    bigint_trim(quot);
    quot->sign = (a->sign == b->sign) ? 1 : -1;
    rem->sign = a->sign;
    if (quot->length == 1 && quot->digits[0] == 0)
        quot->sign = 1;
    if (rem->length == 1 && rem->digits[0] == 0)
        rem->sign = 1;

    if (remainder)
        *remainder = rem;
    else
        bigint_free(rem);
    return quot;
}

int bigint_count(const BigInt *a)
{
    return a->length;
}

int bigint_is_empty(const BigInt *a)
{
    return a->length == 0;
}

int bigint_is_full(const BigInt *a)
{
    (void)a;
    return 0;
}

char *bigint_to_string(const BigInt *a)
{
    return bigint_to_str(a);
}

BigInt *bigint_clone(const BigInt *a)
{
    return bigint_copy(a);
}

void bigint_make_empty(BigInt *a)
{
    free(a->digits);
    a->digits = NULL;
    a->length = 0;
    a->sign = 1;
}