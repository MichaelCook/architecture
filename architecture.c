/*
  Show information about the computer processor architecture.

  gcc -std=c99 -Wall -Werror architecture.c && ./a.out
*/

/*
 This software is distributed under the "Simplified BSD license":

 Copyright Michael Cook <michael@waxrat.com>. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

static void show_endian(const char *label, void *p, size_t n, uint8_t lsb)
{
  uint8_t *q = p;
  printf("%s is %s endian (", label, *q == lsb ? "little" : "big");
  for (size_t i = 0; i < n; ++i)
    printf(" %02X" + (i == 0), *q++);
  printf(")\n");
}

static bool stack_grows_down_helper(int *p)
{
  int i;
  //printf("from %p to %p added %td\n", p, &i, &i - p);
  return &i < p;
}

static bool stack_grows_down(void)
{
  int i;
  return stack_grows_down_helper(&i);
}

int main()
{
  uint16_t h = 0x1234;
  show_endian("uint16_t", &h, sizeof(h), h);

  uint32_t l = 0x12345678L;
  show_endian("uint32_t", &l, sizeof(l), l);

  uint64_t q = 0x1234567890abcdef;
  show_endian("uint64_t", &q, sizeof(q), q);

  /* We assume the stack grows either up or down.  The C standards don't
     require either, strictly speaking. */
  printf("stack grows %s\n", stack_grows_down() ?
         "down (to lower addresses)" : "up (to higher addresses)");

  char c = ~0;
  int i = c;
  printf("char is %s\n", i < 0 ? "signed" : "unsigned");

  size_t bits_per_char = 0;
  while (c != 0) {
    c &= c - 1;
    ++bits_per_char;
  }
  #define SHOW_TYPE(T) do {                                             \
    struct { char a; T b; } x;                                          \
    printf("%s is %zu bits with %td-byte alignment\n",                  \
           #T, sizeof(T) * bits_per_char, (char*) &x.b - &x.a);         \
  } while (0)
  SHOW_TYPE(char);
  SHOW_TYPE(short);
  SHOW_TYPE(int);
  SHOW_TYPE(long);
  SHOW_TYPE(long long);
  SHOW_TYPE(float);
  SHOW_TYPE(double);
  SHOW_TYPE(long double);
  printf("sizeof(size_t)=%zu\n", sizeof(size_t));

  printf("FLT_ROUNDS %d\n", FLT_ROUNDS);
  printf("FLT_RADIX %d\n", FLT_RADIX);
  printf("MANT_DIG   %6d %6d %6d\n", FLT_MANT_DIG, DBL_MANT_DIG, LDBL_MANT_DIG);
  printf("DIG        %6d %6d %6d\n", FLT_DIG, DBL_DIG, LDBL_DIG);
  printf("MIN_EXP    %6d %6d %6d\n", FLT_MIN_EXP, DBL_MIN_EXP, LDBL_MIN_EXP);
  printf("MIN_10_EXP %6d %6d %6d\n", FLT_MIN_10_EXP, DBL_MIN_10_EXP, LDBL_MIN_10_EXP);
  printf("MAX_EXP    %6d %6d %6d\n", FLT_MAX_EXP, DBL_MAX_EXP, LDBL_MAX_EXP);
  printf("MIN        %g %g %Lg\n", FLT_MIN, DBL_MIN, LDBL_MIN);
  printf("MAX        %g %g %Lg\n", FLT_MAX, DBL_MAX, LDBL_MAX);
  printf("EPSILON    %g %g %Lg\n", FLT_EPSILON, DBL_EPSILON, LDBL_EPSILON);
  return 0;
}
