// RUN: %clang_cc1 -ffreestanding %s -O2 -triple=x86_64-apple-darwin -target-cpu skylake-avx512 -emit-llvm -o - -Wall -Werror | FileCheck %s

#include <immintrin.h>

long long test_mm512_reduce_add_epi64(__m512i __W){
  // CHECK: %shuffle.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %add.i = add <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %add.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %add.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %add4.i = add <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %add4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %add7.i = add <2 x i64> %shuffle6.i, %add4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %add7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_reduce_add_epi64(__W);
}

long long test_mm512_reduce_mul_epi64(__m512i __W){
  // CHECK: %shuffle.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %mul.i = mul <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %mul.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %mul.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %mul4.i = mul <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %mul4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %mul7.i = mul <2 x i64> %shuffle6.i, %mul4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %mul7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_reduce_mul_epi64(__W); 
}

long long test_mm512_reduce_or_epi64(__m512i __W){
  // CHECK: %shuffle.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %or.i = or <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %or.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %or.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %or4.i = or <2 x i64> %shuffle2.i, %shuffle3.i 
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %or4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %or7.i = or <2 x i64> %shuffle6.i, %or4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %or7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_reduce_or_epi64(__W); 
}

long long test_mm512_reduce_and_epi64(__m512i __W){
  // CHECK: %shuffle.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> %__W, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %and.i = and <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %and.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %and.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %and4.i = and <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %and4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %and7.i = and <2 x i64> %shuffle6.i, %and4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %and7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_reduce_and_epi64(__W);
}

long long test_mm512_mask_reduce_add_epi64(__mmask8 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast i8 %__M to <8 x i1>
  // CHECK: {{.*}} = select <8 x i1> {{.*}}, <8 x i64> %__W, <8 x i64> zeroinitializer
  // CHECK: %shuffle.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %add.i = add <4 x i64> %shuffle.i, %shuffle1.i 
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %add.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %add.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %add4.i = add <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %add4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %add7.i = add <2 x i64> %shuffle6.i, %add4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %add7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_mask_reduce_add_epi64(__M, __W); 
}

long long test_mm512_mask_reduce_mul_epi64(__mmask8 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast i8 %__M to <8 x i1>
  // CHECK: {{.*}} = select <8 x i1> {{.*}}, <8 x i64> %__W, <8 x i64> <i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1, i64 1>
  // CHECK: %shuffle.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %mul.i = mul <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %mul.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %mul.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %mul4.i = mul <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %mul4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %mul7.i = mul <2 x i64> %shuffle6.i, %mul4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %mul7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_mask_reduce_mul_epi64(__M, __W); 
}

long long test_mm512_mask_reduce_and_epi64(__mmask8 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast i8 %__M to <8 x i1>
  // CHECK: {{.*}} = select <8 x i1> {{.*}}, <8 x i64> %__W, <8 x i64> <i64 -1, i64 -1, i64 -1, i64 -1, i64 -1, i64 -1, i64 -1, i64 -1>
  // CHECK: %shuffle.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %and.i = and <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %and.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %and.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %and4.i = and <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %and4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %and7.i = and <2 x i64> %shuffle6.i, %and4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %and7.i, i32 0
  return _mm512_mask_reduce_and_epi64(__M, __W); 
}

long long test_mm512_mask_reduce_or_epi64(__mmask8 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast i8 %__M to <8 x i1>
  // CHECK: {{.*}} = select <8 x i1> {{.*}}, <8 x i64> %__W, <8 x i64> zeroinitializer
  // CHECK: %shuffle.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x i64> {{.*}}, <8 x i64> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %or.i = or <4 x i64> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x i64> %or.i, <4 x i64> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x i64> %or.i, <4 x i64> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %or4.i = or <2 x i64> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x i64> %or4.i, <2 x i64> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %or7.i = or <2 x i64> %shuffle6.i, %or4.i
  // CHECK: %vecext.i = extractelement <2 x i64> %or7.i, i32 0
  // CHECK: ret i64 %vecext.i
  return _mm512_mask_reduce_or_epi64(__M, __W); 
}

int test_mm512_reduce_add_epi32(__m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle.i to <4 x i64>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle1.i to <4 x i64>
  // CHECK: %add.i = add <4 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <4 x i64> %add.i to <8 x i32>
  // CHECK: %shuffle2.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle2.i to <2 x i64>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle3.i to <2 x i64>
  // CHECK: %add4.i = add <2 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <2 x i64> %add4.i to <4 x i32>
  // CHECK: %shuffle6.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle6.i to <2 x i64>
  // CHECK: %add7.i = add <2 x i64> {{.*}}, %add4.i
  // CHECK: {{.*}} = bitcast <2 x i64> %add7.i to <4 x i32>
  // CHECK: %shuffle9.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle9.i to <2 x i64>
  // CHECK: %add10.i = add <2 x i64> {{.*}}, %add7.i
  // CHECK: %vecext.i = extractelement <2 x i64> %add10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_reduce_add_epi32(__W); 
}

int test_mm512_reduce_mul_epi32(__m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle.i to <4 x i64>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle1.i to <4 x i64>
  // CHECK: %mul.i = mul <4 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <4 x i64> %mul.i to <8 x i32>
  // CHECK: %shuffle2.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle2.i to <2 x i64>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle3.i to <2 x i64>
  // CHECK: %mul4.i = mul <2 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <2 x i64> %mul4.i to <4 x i32>
  // CHECK: %shuffle6.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle6.i to <2 x i64>
  // CHECK: %mul7.i = mul <2 x i64> {{.*}}, %mul4.i
  // CHECK: {{.*}} = bitcast <2 x i64> %mul7.i to <4 x i32>
  // CHECK: %shuffle9.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle9.i to <2 x i64>
  // CHECK: %mul10.i = mul <2 x i64> {{.*}}, %mul7.i
  // CHECK: %vecext.i = extractelement <2 x i64> %mul10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_reduce_mul_epi32(__W); 
}

int test_mm512_reduce_or_epi32(__m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %or27.i = or <8 x i32> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x i32> %or27.i, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> %or27.i, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %or428.i = or <4 x i32> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x i32> %or428.i, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %or729.i = or <4 x i32> %shuffle6.i, %or428.i
  // CHECK: %shuffle9.i = shufflevector <4 x i32> %or729.i, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %or1030.i = or <4 x i32> %shuffle9.i, %or729.i
  // CHECK: %or10.i = bitcast <4 x i32> %or1030.i to <2 x i64>
  // CHECK: %vecext.i = extractelement <2 x i64> %or10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_reduce_or_epi32(__W); 
}

int test_mm512_reduce_and_epi32(__m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %and27.i = and <8 x i32> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x i32> %and27.i, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> %and27.i, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %and428.i = and <4 x i32> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x i32> %and428.i, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %and729.i = and <4 x i32> %shuffle6.i, %and428.i
  // CHECK: %shuffle9.i = shufflevector <4 x i32> %and729.i, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %and1030.i = and <4 x i32> %shuffle9.i, %and729.i
  // CHECK: %and10.i = bitcast <4 x i32> %and1030.i to <2 x i64>
  // CHECK: %vecext.i = extractelement <2 x i64> %and10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_reduce_and_epi32(__W); 
}

int test_mm512_mask_reduce_add_epi32(__mmask16 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: {{.*}} = bitcast i16 %__M to <16 x i1>
  // CHECK: {{.*}} = select <16 x i1> {{.*}}, <16 x i32> {{.*}}, <16 x i32> zeroinitializer
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle.i to <4 x i64>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle1.i to <4 x i64>
  // CHECK: %add.i = add <4 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <4 x i64> %add.i to <8 x i32>
  // CHECK: %shuffle2.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle2.i to <2 x i64>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle3.i to <2 x i64>
  // CHECK: %add4.i = add <2 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <2 x i64> %add4.i to <4 x i32>
  // CHECK: %shuffle6.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle6.i to <2 x i64>
  // CHECK: %add7.i = add <2 x i64> {{.*}}, %add4.i
  // CHECK: {{.*}} = bitcast <2 x i64> %add7.i to <4 x i32>
  // CHECK: %shuffle9.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle9.i to <2 x i64>
  // CHECK: %add10.i = add <2 x i64> {{.*}}, %add7.i
  // CHECK: %vecext.i = extractelement <2 x i64> %add10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_mask_reduce_add_epi32(__M, __W); 
}

int test_mm512_mask_reduce_mul_epi32(__mmask16 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: {{.*}} = bitcast i16 %__M to <16 x i1>
  // CHECK: {{.*}} = select <16 x i1> {{.*}}, <16 x i32> {{.*}}, <16 x i32> <i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle.i to <4 x i64>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: {{.*}} = bitcast <8 x i32> %shuffle1.i to <4 x i64>
  // CHECK: %mul.i = mul <4 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <4 x i64> %mul.i to <8 x i32>
  // CHECK: %shuffle2.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle2.i to <2 x i64>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> {{.*}}, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle3.i to <2 x i64>
  // CHECK: %mul4.i = mul <2 x i64> {{.*}}, {{.*}}
  // CHECK: {{.*}} = bitcast <2 x i64> %mul4.i to <4 x i32>
  // CHECK: %shuffle6.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle6.i to <2 x i64>
  // CHECK: %mul7.i = mul <2 x i64> {{.*}}, %mul4.i
  // CHECK: {{.*}} = bitcast <2 x i64> %mul7.i to <4 x i32>
  // CHECK: %shuffle9.i = shufflevector <4 x i32> {{.*}}, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: {{.*}} = bitcast <4 x i32> %shuffle9.i to <2 x i64>
  // CHECK: %mul10.i = mul <2 x i64> {{.*}}, %mul7.i
  // CHECK: %vecext.i = extractelement <2 x i64> %mul10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_mask_reduce_mul_epi32(__M, __W); 
}

int test_mm512_mask_reduce_and_epi32(__mmask16 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: {{.*}} = bitcast i16 %__M to <16 x i1>
  // CHECK: {{.*}} = select <16 x i1> {{.*}}, <16 x i32> {{.*}}, <16 x i32> <i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1, i32 -1>
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %and28.i = and <8 x i32> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x i32> %and28.i, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> %and28.i, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %and429.i = and <4 x i32> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x i32> %and429.i, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %and730.i = and <4 x i32> %shuffle6.i, %and429.i
  // CHECK: %shuffle9.i = shufflevector <4 x i32> %and730.i, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %and1031.i = and <4 x i32> %shuffle9.i, %and730.i
  // CHECK: %and10.i = bitcast <4 x i32> %and1031.i to <2 x i64>
  // CHECK: %vecext.i = extractelement <2 x i64> %and10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_mask_reduce_and_epi32(__M, __W); 
}

int test_mm512_mask_reduce_or_epi32(__mmask16 __M, __m512i __W){
  // CHECK: {{.*}} = bitcast <8 x i64> %__W to <16 x i32>
  // CHECK: {{.*}} = bitcast i16 %__M to <16 x i1>
  // CHECK: {{.*}} = select <16 x i1> {{.*}}, <16 x i32> {{.*}}, <16 x i32> zeroinitializer
  // CHECK: %shuffle.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x i32> {{.*}}, <16 x i32> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %or28.i = or <8 x i32> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x i32> %or28.i, <8 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x i32> %or28.i, <8 x i32> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %or429.i = or <4 x i32> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x i32> %or429.i, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %or730.i = or <4 x i32> %shuffle6.i, %or429.i
  // CHECK: %shuffle9.i = shufflevector <4 x i32> %or730.i, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %or1031.i = or <4 x i32> %shuffle9.i, %or730.i
  // CHECK: %or10.i = bitcast <4 x i32> %or1031.i to <2 x i64>
  // CHECK: %vecext.i = extractelement <2 x i64> %or10.i, i32 0
  // CHECK: %conv.i = trunc i64 %vecext.i to i32
  // CHECK: ret i32 %conv.i
  return _mm512_mask_reduce_or_epi32(__M, __W); 
}

double test_mm512_reduce_add_pd(__m512d __W){
  // CHECK: %shuffle.i = shufflevector <8 x double> %__W, <8 x double> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x double> %__W, <8 x double> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %add.i = fadd <4 x double> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x double> %add.i, <4 x double> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x double> %add.i, <4 x double> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %add4.i = fadd <2 x double> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x double> %add4.i, <2 x double> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %add7.i = fadd <2 x double> %add4.i, %shuffle6.i
  // CHECK: %vecext.i = extractelement <2 x double> %add7.i, i32 0
  // CHECK: ret double %vecext.i
  return _mm512_reduce_add_pd(__W); 
}

double test_mm512_reduce_mul_pd(__m512d __W){
  // CHECK: %shuffle.i = shufflevector <8 x double> %__W, <8 x double> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x double> %__W, <8 x double> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %mul.i = fmul <4 x double> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x double> %mul.i, <4 x double> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x double> %mul.i, <4 x double> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %mul4.i = fmul <2 x double> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x double> %mul4.i, <2 x double> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %mul7.i = fmul <2 x double> %mul4.i, %shuffle6.i
  // CHECK: %vecext.i = extractelement <2 x double> %mul7.i, i32 0
  // CHECK: ret double %vecext.i
  return _mm512_reduce_mul_pd(__W); 
}

float test_mm512_reduce_add_ps(__m512 __W){
  // CHECK: %shuffle.i = shufflevector <16 x float> %__W, <16 x float> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x float> %__W, <16 x float> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %add.i = fadd <8 x float> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x float> %add.i, <8 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x float> %add.i, <8 x float> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %add4.i = fadd <4 x float> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x float> %add4.i, <4 x float> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %add7.i = fadd <4 x float> %add4.i, %shuffle6.i
  // CHECK: %shuffle9.i = shufflevector <4 x float> %add7.i, <4 x float> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %add10.i = fadd <4 x float> %add7.i, %shuffle9.i
  // CHECK: %vecext.i = extractelement <4 x float> %add10.i, i32 0
  // CHECK: ret float %vecext.i
  return _mm512_reduce_add_ps(__W); 
}

float test_mm512_reduce_mul_ps(__m512 __W){
  // CHECK: %shuffle.i = shufflevector <16 x float> %__W, <16 x float> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x float> %__W, <16 x float> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %mul.i = fmul <8 x float> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x float> %mul.i, <8 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x float> %mul.i, <8 x float> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %mul4.i = fmul <4 x float> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x float> %mul4.i, <4 x float> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %mul7.i = fmul <4 x float> %mul4.i, %shuffle6.i
  // CHECK: %shuffle9.i = shufflevector <4 x float> %mul7.i, <4 x float> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %mul10.i = fmul <4 x float> %mul7.i, %shuffle9.i
  // CHECK: %vecext.i = extractelement <4 x float> %mul10.i, i32 0
  // CHECK: ret float %vecext.i
  return _mm512_reduce_mul_ps(__W); 
}

double test_mm512_mask_reduce_add_pd(__mmask8 __M, __m512d __W){
  // CHECK: {{.*}} = bitcast i8 %__M to <8 x i1>
  // CHECK: {{.*}} = select <8 x i1> {{.*}}, <8 x double> %__W, <8 x double> zeroinitializer
  // CHECK: %shuffle.i = shufflevector <8 x double> {{.*}}, <8 x double> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x double> {{.*}}, <8 x double> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %add.i = fadd <4 x double> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x double> %add.i, <4 x double> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x double> %add.i, <4 x double> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %add4.i = fadd <2 x double> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x double> %add4.i, <2 x double> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %add7.i = fadd <2 x double> %add4.i, %shuffle6.i
  // CHECK: %vecext.i = extractelement <2 x double> %add7.i, i32 0
  // CHECK: ret double %vecext.i
  return _mm512_mask_reduce_add_pd(__M, __W); 
}

double test_mm512_mask_reduce_mul_pd(__mmask8 __M, __m512d __W){
  // CHECK: {{.*}} = bitcast i8 %__M to <8 x i1>
  // CHECK: {{.*}} = select <8 x i1> {{.*}}, <8 x double> %__W, <8 x double> <double 4.940656e-324, double 4.940656e-324, double 4.940656e-324, double 4.940656e-324, double 4.940656e-324, double 4.940656e-324, double 4.940656e-324, double 4.940656e-324>
  // CHECK: %shuffle.i = shufflevector <8 x double> {{.*}}, <8 x double> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle1.i = shufflevector <8 x double> {{.*}}, <8 x double> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %mul.i = fmul <4 x double> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <4 x double> %mul.i, <4 x double> undef, <2 x i32> <i32 0, i32 1>
  // CHECK: %shuffle3.i = shufflevector <4 x double> %mul.i, <4 x double> undef, <2 x i32> <i32 2, i32 3>
  // CHECK: %mul4.i = fmul <2 x double> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <2 x double> %mul4.i, <2 x double> undef, <2 x i32> <i32 1, i32 undef>
  // CHECK: %mul7.i = fmul <2 x double> %mul4.i, %shuffle6.i
  // CHECK: %vecext.i = extractelement <2 x double> %mul7.i, i32 0
  // CHECK: ret double %vecext.i
  return _mm512_mask_reduce_mul_pd(__M, __W); 
}

float test_mm512_mask_reduce_add_ps(__mmask16 __M, __m512 __W){
  // CHECK: {{.*}} = bitcast i16 %__M to <16 x i1>
  // CHECK: {{.*}} = select <16 x i1> {{.*}}, <16 x float> %__W, <16 x float> zeroinitializer
  // CHECK: %shuffle.i = shufflevector <16 x float> {{.*}}, <16 x float> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x float> {{.*}}, <16 x float> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %add.i = fadd <8 x float> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x float> %add.i, <8 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x float> %add.i, <8 x float> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %add4.i = fadd <4 x float> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x float> %add4.i, <4 x float> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %add7.i = fadd <4 x float> %add4.i, %shuffle6.i
  // CHECK: %shuffle9.i = shufflevector <4 x float> %add7.i, <4 x float> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %add10.i = fadd <4 x float> %add7.i, %shuffle9.i
  // CHECK: %vecext.i = extractelement <4 x float> %add10.i, i32 0
  // CHECK: ret float %vecext.i
  return _mm512_mask_reduce_add_ps(__M, __W); 
}

float test_mm512_mask_reduce_mul_ps(__mmask16 __M, __m512 __W){
  // CHECK: {{.*}} = bitcast i16 %__M to <16 x i1>
  // CHECK: {{.*}} = select <16 x i1> {{.*}}, <16 x float> %__W, <16 x float> <float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float                 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000, float      0x36A0000000000000, float 0x36A0000000000000, float 0x36A0000000000000>
  // CHECK: %shuffle.i = shufflevector <16 x float> {{.*}}, <16 x float> undef, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  // CHECK: %shuffle1.i = shufflevector <16 x float> {{.*}}, <16 x float> undef, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  // CHECK: %mul.i = fmul <8 x float> %shuffle.i, %shuffle1.i
  // CHECK: %shuffle2.i = shufflevector <8 x float> %mul.i, <8 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  // CHECK: %shuffle3.i = shufflevector <8 x float> %mul.i, <8 x float> undef, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
  // CHECK: %mul4.i = fmul <4 x float> %shuffle2.i, %shuffle3.i
  // CHECK: %shuffle6.i = shufflevector <4 x float> %mul4.i, <4 x float> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  // CHECK: %mul7.i = fmul <4 x float> %mul4.i, %shuffle6.i
  // CHECK: %shuffle9.i = shufflevector <4 x float> %mul7.i, <4 x float> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  // CHECK: %mul10.i = fmul <4 x float> %mul7.i, %shuffle9.i
  // CHECK: %vecext.i = extractelement <4 x float> %mul10.i, i32 0
  // CHECK: ret float %vecext.i
  return _mm512_mask_reduce_mul_ps(__M, __W); 
}
