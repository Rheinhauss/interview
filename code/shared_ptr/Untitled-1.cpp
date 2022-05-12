#include <memory>
using std::shared_ptr;

class B; // 前置声明
class A {
public:
    shared_ptr<B> ptr;
};
 
class B {
public:
    shared_ptr<A> ptr;
};
 
void test(){
    shared_ptr<A> pa(new A());
    shared_ptr<B> pb(new B());
    pa -> ptr = pb;
    pb -> ptr = pa;
}

int main(){
    test();
}