# BigInt — Ilgas sveikas skaičius

## Aprašymas

ADT realizacija didelių sveikųjų skaičių aritmetikai C kalba.
Skaičiai saugomi kaip radix=256 (2^8) pagrindo skaitmenų dinaminis masyvas.

## Failai

- `bigint.h` — ADT aprašymas
- `bigint.c` — realizacija
- `main.c` — testavimo programa

## Kompiliavimas

```bash
make
arba 
mingw32-make
```

arba rankiniu būdu:

```bash
gcc -o test.exe main.c bigint.c
```

## Realizuotos užklausos

- `bigint_count(a)` — grąžina skaitmenų skaičių
- `bigint_is_empty(a)` — patikrina ar count() == 0
- `bigint_to_string(a)` — grąžina ADT atitinkantį tekstą (reikia `free()`)

## Realizuotos komandos

- `bigint_clone(a)` — sukuria ADT kopiją, originalas lieka nepakitęs
- `bigint_make_empty(a)` — išvalo ADT

## Aritmetika

- `bigint_add(a, b, &out)` — sudėtis
- `bigint_sub(a, b, &out)` — atimtis
- `bigint_mul(a, b, &out)` — daugyba
- `bigint_div(a, b, &out, &rem)` — dalyba su liekana