#include <atomic>
#include <iostream>

using std::atomic;
using std::cin;
using std::cout;
using std::endl;

struct RefCount {
    atomic<size_t> cnt;
    void incre() {
        ++cnt;
    }
    void decre() {
        --cnt;
    }
    size_t cnt() const {
        return cnt.load();
    }

    RefCount() : cnt(1) {
    }
};

template <typename T>
class SharedPtr {
    T* data;
    RefCount* refcnt;

   public:
    explicit SharedPtr() : data(nullptr) {
        this->refcnt = new RefCount;
    }
    explicit SharedPtr(T* p) : data(p) {
        this->refcnt = new RefCount;
    }
    SharedPtr(const SharedPtr& sp) {
        sp->refcnt->incre();
        this->refcnt = sp->refcnt;
        this->data   = sp->data;
    }
    SharedPtr& operator=(const SharedPtr& sp) {
        this->clean();
        sp->refcnt->incre();
        this->refcnt = sp->refcnt;
        this->data   = sp->data;
        return *this;
    }
    SharedPtr(SharedPtr&& sp) {
        this->refcnt = sp->refcnt;
        this->data   = sp->data;
        sp->refcnt   = nullptr;
        sp->data     = nullptr;
    }
    SharedPtr& operator=(SharedPtr&& sp) {
        this->clean();
        this->refcnt = sp->refcnt;
        this->data   = sp->data;
        sp->refcnt   = nullptr;
        sp->data     = nullptr;
        return *this;
    }
    ~SharedPtr() {
        this->clean();
    }

    size_t cnt() const {
        return this->refcnt->cnt();
    }
    T* get() const {
        return this->data;
    }
    T* operator->() const {
        return this->data;
    }
    T& operator*() const {
        return *(this->data);
    }
    operator bool() const {
        return this->data;
    }

   private:
    void clean() {
        this->refcnt->decre();
        if (this->refcnt->cnt() == 0) {
            if (this->data) {
                delete this->data;
            }
            delete this->refcnt;
        }
    }
};