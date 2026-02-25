#include "bigint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Sukuria tuščią BigInt su 'length' skaitmenų (visi nulis) */
static BigInt *bigint_alloc(int length) {
    BigInt *n  = malloc(sizeof(BigInt));
    n->digits  = calloc(length, 1);
    n->length  = length;
    n->sign    = 1;
    return n;
}

/* Pašalina vedinius nulius */
static void bigint_trim(BigInt *a) {
    while (a->length > 1 && a->digits[a->length - 1] == 0)
        a->length--;
    if (a->length == 1 && a->digits[0] == 0)
        a->sign = 1;
}

/* Lygina absoliučias reikšmes: grąžina -1, 0, arba 1 */
static int cmp_abs(const BigInt *a, const BigInt *b) {
    if (a->length != b->length)
        return a->length > b->length ? 1 : -1;
    for (int i = a->length - 1; i >= 0; i--)
        if (a->digits[i] != b->digits[i])
            return a->digits[i] > b->digits[i] ? 1 : -1;
    return 0;
}

/* Sudeda absoliučias reikšmes */
static BigInt *add_abs(const BigInt *a, const BigInt *b) {
    int len = (a->length > b->length ? a->length : b->length) + 1;
    BigInt *result = bigint_alloc(len);
    int carry = 0;
    for (int i = 0; i < len; i++) {
        int sum = (i < a->length ? a->digits[i] : 0)
                + (i < b->length ? b->digits[i] : 0)
                + carry;
        result->digits[i] = sum % RADIX;
        carry = sum / RADIX;
    }
    bigint_trim(result);
    return result;
}

/* Atima absoliučias reikšmes, reikalauja |a| >= |b| */
static BigInt *sub_abs(const BigInt *a, const BigInt *b) {
    BigInt *result = bigint_alloc(a->length);
    int borrow = 0;
    for (int i = 0; i < a->length; i++) {
        int diff = a->digits[i]
                 - (i < b->length ? b->digits[i] : 0)
                 - borrow;
        if (diff < 0) { diff += RADIX; borrow = 1; }
        else borrow = 0;
        result->digits[i] = diff;
    }
    bigint_trim(result);
    return result;
}

/* ---- Viešos funkcijos ---- */

BigInt *bigint_from_int(long long value) {
    BigInt *n = bigint_alloc(10);
    n->sign = value < 0 ? -1 : 1;
    unsigned long long u = value < 0 ? (unsigned long long)(-value)
                                     : (unsigned long long)value;
    n->length = 0;
    if (u == 0) { n->length = 1; return n; }
    while (u > 0) {
        n->digits[n->length++] = u % RADIX;
        u /= RADIX;
    }
    return n;
}

BigInt *bigint_from_str(const char *str) {
    if (!str || !*str) return bigint_from_int(0);
    int sign = 1;
    if (*str == '-') { sign = -1; str++; }
    else if (*str == '+') str++;
    while (*str == '0' && str[1]) str++; /* veda nulius */

    BigInt *result = bigint_from_int(0);
    BigInt *ten    = bigint_from_int(10);
    for (; *str; str++) {
        if (!isdigit((unsigned char)*str)) break;
        BigInt *tmp = NULL;
        bigint_mul(result, ten, &tmp);  bigint_free(result);
        BigInt *digit = bigint_from_int(*str - '0');
        bigint_add(tmp, digit, &result);
        bigint_free(tmp); bigint_free(digit);
    }
    bigint_free(ten);
    result->sign = sign;
    bigint_trim(result);
    return result;
}

BigInt *bigint_copy(const BigInt *a) {
    BigInt *n = bigint_alloc(a->length);
    memcpy(n->digits, a->digits, a->length);
    n->sign = a->sign;
    return n;
}

void bigint_free(BigInt *a) {
    if (!a) return;
    free(a->digits);
    free(a);
}

void bigint_print(const BigInt *a) {
    BigInt *tmp  = bigint_copy(a); tmp->sign = 1;
    BigInt *ten  = bigint_from_int(10);
    BigInt *zero = bigint_from_int(0);
    char buf[4096]; int pos = 0;
    while (bigint_cmp(tmp, zero) != 0) {
        BigInt *rem = NULL, *q = NULL;
        bigint_div(tmp, ten, &q, &rem);
        buf[pos++] = '0' + rem->digits[0];
        bigint_free(rem); bigint_free(tmp); tmp = q;
    }
    bigint_free(tmp); bigint_free(ten); bigint_free(zero);
    if (pos == 0) buf[pos++] = '0';
    if (a->sign == -1) buf[pos++] = '-';
    buf[pos] = '\0';
    for (int i = 0, j = pos - 1; i < j; i++, j--) {
        char c = buf[i]; buf[i] = buf[j]; buf[j] = c;
    }
    printf("%s", buf);
}

int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->sign != b->sign) return a->sign > b->sign ? 1 : -1;
    int c = cmp_abs(a, b);
    return a->sign == 1 ? c : -c;
}

int bigint_add(const BigInt *a, const BigInt *b, BigInt **out) {
    if (!a || !b || !out) return BIGINT_ERR_NULL;
    BigInt *result;
    if (a->sign == b->sign) {
        result = add_abs(a, b);
        result->sign = a->sign;
    } else {
        int c = cmp_abs(a, b);
        if (c == 0) { *out = bigint_from_int(0); return BIGINT_OK; }
        result = c > 0 ? sub_abs(a, b) : sub_abs(b, a);
        result->sign = c > 0 ? a->sign : b->sign;
    }
    bigint_trim(result);
    *out = result;
    return BIGINT_OK;
}

int bigint_sub(const BigInt *a, const BigInt *b, BigInt **out) {
    if (!a || !b || !out) return BIGINT_ERR_NULL;
    BigInt *neg_b = bigint_copy(b);
    neg_b->sign = (b->length == 1 && b->digits[0] == 0) ? 1 : -b->sign;
    int err = bigint_add(a, neg_b, out);
    bigint_free(neg_b);
    return err;
}

int bigint_mul(const BigInt *a, const BigInt *b, BigInt **out) {
    if (!a || !b || !out) return BIGINT_ERR_NULL;
    BigInt *result = bigint_alloc(a->length + b->length);
    for (int i = 0; i < a->length; i++) {
        int carry = 0;
        for (int j = 0; j < b->length; j++) {
            int prod = a->digits[i] * b->digits[j]
                     + result->digits[i + j] + carry;
            result->digits[i + j] = prod % RADIX;
            carry = prod / RADIX;
        }
        result->digits[i + b->length] += carry;
    }
    result->sign = (a->sign == b->sign) ? 1 : -1;
    bigint_trim(result);
    *out = result;
    return BIGINT_OK;
}

int bigint_div(const BigInt *a, const BigInt *b, BigInt **out, BigInt **remainder) {
    if (!a || !b || !out) return BIGINT_ERR_NULL;
    if (b->length == 1 && b->digits[0] == 0) return BIGINT_ERR_DIV0;

    if (cmp_abs(a, b) < 0) {
        if (remainder) *remainder = bigint_copy(a);
        *out = bigint_from_int(0);
        return BIGINT_OK;
    }

    BigInt *quot = bigint_alloc(a->length);
    BigInt *rem  = bigint_from_int(0);

    for (int i = a->length - 1; i >= 0; i--) {
        /* rem = rem * RADIX + a->digits[i] */
        BigInt *shifted = bigint_alloc(rem->length + 1);
        for (int k = 0; k < rem->length; k++)
            shifted->digits[k + 1] = rem->digits[k];
        shifted->digits[0] = a->digits[i];
        bigint_free(rem); bigint_trim(shifted); rem = shifted;

        /* Binarinė paieška: kiek kartų b telpa į rem */
        int lo = 0, hi = RADIX - 1, qd = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            BigInt *m = bigint_from_int(mid);
            BigInt *p; bigint_mul(b, m, &p); p->sign = 1; bigint_free(m);
            if (cmp_abs(p, rem) <= 0) { qd = mid; lo = mid + 1; }
            else hi = mid - 1;
            bigint_free(p);
        }
        quot->digits[i] = qd;

        /* rem = rem - qd * b */
        BigInt *qb = bigint_from_int(qd);
        BigInt *sub; bigint_mul(b, qb, &sub); sub->sign = 1; bigint_free(qb);
        BigInt *new_rem = sub_abs(rem, sub);
        bigint_free(sub); bigint_free(rem); rem = new_rem;
    }

    bigint_trim(quot);
    quot->sign = (a->sign == b->sign) ? 1 : -1;
    rem->sign  = a->sign;
    if (quot->length == 1 && quot->digits[0] == 0) quot->sign = 1;
    if (rem->length  == 1 && rem->digits[0]  == 0) rem->sign  = 1;

    *out = quot;
    if (remainder) *remainder = rem; else bigint_free(rem);
    return BIGINT_OK;
}