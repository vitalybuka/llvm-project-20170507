// RUN: %clang -triple=i386-apple-darwin10 -msse2 -fsyntax-only -verify -std=c89 %s

// PR6658
#include <xmmintrin.h>

