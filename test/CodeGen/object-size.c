// RUN: %clang_cc1 -triple x86_64-apple-darwin -emit-llvm %s -o - 2>&1 | FileCheck %s

#define strcpy(dest, src) \
  ((__builtin_object_size(dest, 0) != -1ULL) \
   ? __builtin___strcpy_chk (dest, src, __builtin_object_size(dest, 1)) \
   : __inline_strcpy_chk(dest, src))

static char *__inline_strcpy_chk (char *dest, const char *src) {
  return __builtin___strcpy_chk(dest, src, __builtin_object_size(dest, 1));
}

char gbuf[63];
char *gp;
int gi, gj;

// CHECK-LABEL: define void @test1
void test1() {
  // CHECK:     = call i8* @__strcpy_chk(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @gbuf, i32 0, i64 4), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i64 59)
  strcpy(&gbuf[4], "Hi there");
}

// CHECK-LABEL: define void @test2
void test2() {
  // CHECK:     = call i8* @__strcpy_chk(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @gbuf, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i64 63)
  strcpy(gbuf, "Hi there");
}

// CHECK-LABEL: define void @test3
void test3() {
  // CHECK:     = call i8* @__strcpy_chk(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @gbuf, i64 1, i64 37), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i64 0)
  strcpy(&gbuf[100], "Hi there");
}

// CHECK-LABEL: define void @test4
void test4() {
  // CHECK:     = call i8* @__strcpy_chk(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @gbuf, i32 0, i64 -1), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i64 0)
  strcpy((char*)(void*)&gbuf[-1], "Hi there");
}

// CHECK-LABEL: define void @test5
void test5() {
  // CHECK:     = load i8*, i8** @gp
  // CHECK-NEXT:= call i64 @llvm.objectsize.i64.p0i8(i8* %{{.*}}, i1 false)
  strcpy(gp, "Hi there");
}

// CHECK-LABEL: define void @test6
void test6() {
  char buf[57];

  // CHECK:       = call i8* @__strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i64 53)
  strcpy(&buf[4], "Hi there");
}

// CHECK-LABEL: define void @test7
void test7() {
  int i;
  // Ensure we only evaluate the side-effect once.
  // CHECK:     = add
  // CHECK-NOT: = add
  // CHECK:     = call i8* @__strcpy_chk(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @gbuf, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i64 63)
  strcpy((++i, gbuf), "Hi there");
}

// CHECK-LABEL: define void @test8
void test8() {
  char *buf[50];
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(buf[++gi], "Hi there");
}

// CHECK-LABEL: define void @test9
void test9() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy((char *)((++gi) + gj), "Hi there");
}

// CHECK-LABEL: define void @test10
char **p;
void test10() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(*(++p), "Hi there");
}

// CHECK-LABEL: define void @test11
void test11() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* getelementptr inbounds ([63 x i8], [63 x i8]* @gbuf, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(gp = gbuf, "Hi there");
}

// CHECK-LABEL: define void @test12
void test12() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(++gp, "Hi there");
}

// CHECK-LABEL: define void @test13
void test13() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(gp++, "Hi there");
}

// CHECK-LABEL: define void @test14
void test14() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(--gp, "Hi there");
}

// CHECK-LABEL: define void @test15
void test15() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{..*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(gp--, "Hi there");
}

// CHECK-LABEL: define void @test16
void test16() {
  // CHECK-NOT:   __strcpy_chk
  // CHECK:       = call i8* @__inline_strcpy_chk(i8* %{{.*}}, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0))
  strcpy(gp += 1, "Hi there");
}

// CHECK: @test17
void test17() {
  // CHECK: store i32 -1
  gi = __builtin_object_size(gp++, 0);
  // CHECK: store i32 -1
  gi = __builtin_object_size(gp++, 1);
  // CHECK: store i32 0
  gi = __builtin_object_size(gp++, 2);
  // CHECK: store i32 0
  gi = __builtin_object_size(gp++, 3);
}

// CHECK: @test18
unsigned test18(int cond) {
  int a[4], b[4];
  // CHECK: phi i32*
  // CHECK: call i64 @llvm.objectsize.i64
  return __builtin_object_size(cond ? a : b, 0);
}

// CHECK: @test19
void test19() {
  struct {
    int a, b;
  } foo;

  // CHECK: store i32 8
  gi = __builtin_object_size(&foo.a, 0);
  // CHECK: store i32 4
  gi = __builtin_object_size(&foo.a, 1);
  // CHECK: store i32 8
  gi = __builtin_object_size(&foo.a, 2);
  // CHECK: store i32 4
  gi = __builtin_object_size(&foo.a, 3);
}

// CHECK: @test20
void test20() {
  struct { int t[10]; } t[10];

  // CHECK: store i32 380
  gi = __builtin_object_size(&t[0].t[5], 0);
  // CHECK: store i32 20
  gi = __builtin_object_size(&t[0].t[5], 1);
  // CHECK: store i32 380
  gi = __builtin_object_size(&t[0].t[5], 2);
  // CHECK: store i32 20
  gi = __builtin_object_size(&t[0].t[5], 3);
}

// CHECK: @test21
void test21() {
  struct { int t; } t;

  // CHECK: store i32 0
  gi = __builtin_object_size(&t + 1, 0);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t + 1, 1);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t + 1, 2);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t + 1, 3);

  // CHECK: store i32 0
  gi = __builtin_object_size(&t.t + 1, 0);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t.t + 1, 1);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t.t + 1, 2);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t.t + 1, 3);
}

// CHECK: @test22
void test22() {
  struct { int t[10]; } t[10];

  // CHECK: store i32 0
  gi = __builtin_object_size(&t[10], 0);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t[10], 1);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t[10], 2);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t[10], 3);

  // CHECK: store i32 0
  gi = __builtin_object_size(&t[9].t[10], 0);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t[9].t[10], 1);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t[9].t[10], 2);
  // CHECK: store i32 0
  gi = __builtin_object_size(&t[9].t[10], 3);
}

struct Test23Ty { int t[10]; };

// CHECK: @test23
void test23(struct Test22Ty *p) {
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* %{{.*}}, i1 false)
  gi = __builtin_object_size(p, 0);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* %{{.*}}, i1 false)
  gi = __builtin_object_size(p, 1);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* %{{.*}}, i1 true)
  gi = __builtin_object_size(p, 2);

  // Note: this is currently fixed at 0 because LLVM doesn't have sufficient
  // data to correctly handle type=3
  // CHECK: store i32 0
  gi = __builtin_object_size(p, 3);
}


// PR24493 -- ICE if __builtin_object_size called with NULL and (Type & 1) != 0
// CHECK @test24
void test24() {
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 false)
  gi = __builtin_object_size((void*)0, 0);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 false)
  gi = __builtin_object_size((void*)0, 1);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 true)
  gi = __builtin_object_size((void*)0, 2);
  // Note: Currently fixed at zero because LLVM can't handle type=3 correctly.
  // Hopefully will be lowered properly in the future.
  // CHECK: store i32 0
  gi = __builtin_object_size((void*)0, 3);
}

// CHECK @test25
void test25() {
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 false)
  gi = __builtin_object_size((void*)0x1000, 0);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 false)
  gi = __builtin_object_size((void*)0x1000, 1);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 true)
  gi = __builtin_object_size((void*)0x1000, 2);
  // Note: Currently fixed at zero because LLVM can't handle type=3 correctly.
  // Hopefully will be lowered properly in the future.
  // CHECK: store i32 0
  gi = __builtin_object_size((void*)0x1000, 3);

  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 false)
  gi = __builtin_object_size((void*)0 + 0x1000, 0);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 false)
  gi = __builtin_object_size((void*)0 + 0x1000, 1);
  // CHECK: call i64 @llvm.objectsize.i64.p0i8(i8* {{.*}}, i1 true)
  gi = __builtin_object_size((void*)0 + 0x1000, 2);
  // Note: Currently fixed at zero because LLVM can't handle type=3 correctly.
  // Hopefully will be lowered properly in the future.
  // CHECK: store i32 0
  gi = __builtin_object_size((void*)0 + 0x1000, 3);
}
