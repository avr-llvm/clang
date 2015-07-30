// RUN: %clang_cc1 %s -I%S -triple=x86_64-apple-darwin10 -emit-llvm -o %t
// RUN: %clang_cc1 %s -I%S -triple=x86_64-apple-darwin10 -O2 -disable-llvm-optzns -emit-llvm -o %t.opt
// RUN: FileCheck --check-prefix=CHECK-TEST1 %s < %t
// RUN: FileCheck --check-prefix=CHECK-TEST2 %s < %t
// RUN: FileCheck --check-prefix=CHECK-TEST5 %s < %t
// RUN: FileCheck --check-prefix=CHECK-TEST8 %s < %t.opt
// RUN: FileCheck --check-prefix=CHECK-TEST9 %s < %t.opt
// RUN: FileCheck --check-prefix=CHECK-TEST10 %s < %t.opt
// RUN: FileCheck --check-prefix=CHECK-TEST11 %s < %t.opt

#include <typeinfo>

// CHECK-TEST1: @_ZTVN5Test11AE = external unnamed_addr constant
namespace Test1 {

struct A {
  A();
  virtual void f();
  virtual ~A() { }
};

A::A() { }

void f(A* a) {
  a->f();
};

// CHECK-LABEL: define void @_ZN5Test11gEv
// CHECK: call void @_ZN5Test11A1fEv
void g() {
  A a;
  f(&a);
}

}

// Test2::A's key function (f) is defined in this translation unit, but when
// we're doing codegen for the typeid(A) call, we don't know that yet.
// This tests mainly that the typeinfo and typename constants have their linkage
// updated correctly.

// CHECK-TEST2: @_ZTSN5Test21AE = constant
// CHECK-TEST2: @_ZTIN5Test21AE = constant
// CHECK-TEST2: @_ZTVN5Test21AE = unnamed_addr constant
namespace Test2 {
  struct A {
    virtual void f();
  };

  const std::type_info &g() {
    return typeid(A);
  };

  void A::f() { }
}

// Test that we don't assert on this test.
namespace Test3 {

struct A {
  virtual void f();
  virtual ~A() { }
};

struct B : A {
  B();
  virtual void f();
};

B::B() { }

void g(A* a) {
  a->f();
};

}

// PR9114, test that we don't try to instantiate RefPtr<Node>.
namespace Test4 {

template <class T> struct RefPtr {
  T* p;
  ~RefPtr() {
    p->deref();
  }
};

struct A {
  virtual ~A();
};

struct Node;

struct B : A {
  virtual void deref();
  RefPtr<Node> m;
};

void f() {
  RefPtr<B> b;
}

}

// PR9130, test that we emit a definition of A::f.
// CHECK-TEST5-LABEL: define linkonce_odr void @_ZN5Test51A1fEv
namespace Test5 {

struct A {
  virtual void f() { }
};

struct B : A { 
  virtual ~B();
};

B::~B() { }

}

// Check that we don't assert on this test.
namespace Test6 {

struct A {
  virtual ~A();
  int a;
};

struct B {
  virtual ~B();
  int b;
};

struct C : A, B { 
  C();
};

struct D : C {
  virtual void f();
  D();
};

D::D() { }

}

namespace Test7 {

struct c1 {};
struct c10 : c1{
  virtual void foo ();
};
struct c11 : c10, c1{
  virtual void f6 ();
};
struct c28 : virtual c11{
  void f6 ();
};
}

namespace Test8 {
// CHECK-TEST8: @_ZTVN5Test81YE = available_externally unnamed_addr constant
// vtable for X is not generated because there are no stores here
struct X {
  X();
  virtual void foo();
};
struct Y : X {
  void foo();
};

void g(X* p) { p->foo(); }
void f() {
  Y y;
  g(&y);
  X x;
  g(&x);
}

}  // Test8

namespace Test9 {
// all virtual functions are outline, so we can assume that it will
// be generated in translation unit where foo is defined
// CHECK-TEST9: @_ZTVN5Test91AE = available_externally unnamed_addr constant
// CHECK-TEST9: @_ZTVN5Test91BE = available_externally unnamed_addr constant
struct A {
  virtual void foo();
  virtual void bar();
};
void A::bar() {}

struct B : A {
  void foo();
};

void g() {
  A a;
  a.foo();
  B b;
  b.foo();
}

}  // Test9

namespace Test10 {

// because A's key function is defined here, vtable is generated in this TU
// CHECK-TEST10: @_ZTVN6Test101AE = unnamed_addr constant
struct A {
  virtual void foo();
  virtual void bar();
};
void A::foo() {}

// Because key function is inline we will generate vtable as linkonce_odr
// CHECK-TEST10: @_ZTVN6Test101DE = linkonce_odr unnamed_addr constant
struct D : A {
  void bar();
};
inline void D::bar() {}

// because B has outline key function then we can refer to
// CHECK-TEST10: @_ZTVN6Test101BE = available_externally unnamed_addr constant
struct B : A {
  void foo();
  void bar();
};

// C's key function (car) is outline, but C has inline virtual function so we
// can't guarantee that we will be able to refer to bar from name
// so (at the moment) we can't emit vtable available_externally
// CHECK-TEST10: @_ZTVN6Test101CE = external unnamed_addr constant
struct C : A {
  void bar() {}               // defined in body - not key function
  virtual inline void gar();  // inline in body - not key function
  virtual void car();
};

// no key function, vtable will be generated everywhere it will be used
// CHECK-TEST10: @_ZTVN6Test101EE = linkonce_odr unnamed_addr constant
struct E : A {};

void g(A& a) {
  a.foo();
  a.bar();
}

void f() {
  A a;
  g(a);
  B b;
  g(b);
  C c;
  g(c);
  D d;
  g(d);
  E e;
  g(e);
}

}  // Test10

namespace Test11 {
struct D;
// Can emit C's vtable available_externally.
// CHECK-TEST11: @_ZTVN6Test111CE = available_externally unnamed_addr constant
struct C {
  virtual D& operator=(const D&);
};

// Cannot emit B's vtable available_externally, because we cannot create
// a reference to the inline virtual B::operator= function.
// CHECK-TEST11: @_ZTVN6Test111DE = external unnamed_addr constant
struct D : C {
  virtual void key();
};
D f();

void g(D& a) {
  C c;
  c = a;
  a.key();
  a.key();
}
void g() {
  D d;
  d = f();
  g(d);
}
}  // Test 11
