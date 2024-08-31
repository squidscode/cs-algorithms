#include <util.hpp>
#include <stdio.h>
#include <cstddef>

#define tt template<class T>

tt class Vector {
public:
    Vector();
    ~Vector();
    void push_back(T);
    T pop_back();
    void insert(size_t idx, T);
    size_t size();
    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

private:
    size_t capacity;
    size_t sz;
    T* buffer;
    void resize();
};

C_CONSTRUCTOR(Vector<int>, Vector, default);
C_DESTRUCTOR(Vector<int>*, Vector, default);

C_VOID_METHOD(Vector<int>*, Vector, push_back, int);
C_METHOD(Vector<int>*, int, Vector, pop_back);
C_VOID_METHOD(Vector<int>*, Vector, insert, size_t, int);
C_METHOD(Vector<int>*, size_t, Vector, size);
extern "C" {
    int C_METHOD_NAME(Vector, get)(void* v, size_t idx) {
        return ((Vector<int>*) v)->operator[](idx);
    }
};

tt Vector<T>::Vector() {
    // printf("[Vector] this: %p\n", this);
    this->capacity = 1;
    this->sz = 0;
    this->buffer = new T[1];
}

tt Vector<T>::~Vector() {
    // printf("[~Vector]\n");
    delete this->buffer;
}

tt void Vector<T>::push_back(T val) {
    // printf("[push_back] this->sz: %zu, this->capacity:%zu\n", ...);
    if (this->sz == this->capacity) {
        this->resize();
    }
    this->buffer[this->sz] = val;
    ++this->sz;
}

tt void Vector<T>::resize() {
    this->capacity <<= 1;
    T* new_buffer = new T[this->capacity];
    for (size_t i = 0; i < this->sz; ++i)
        new_buffer[i] = this->buffer[i];
    delete this->buffer;
    this->buffer = new_buffer;
}

tt T Vector<T>::pop_back() {
    // printf("[pop_back] New size: %zu\n", this->sz-1);
    return this->buffer[--this->sz];
}

tt void Vector<T>::insert(size_t idx, T val) {
    if (this->sz == this->capacity) {
        this->resize();
    }
    ++this->sz;
    for (size_t i = this->sz - 1; i > idx; --i) {
        this->buffer[i] = this->buffer[i-1];
    }
    this->buffer[idx] = val;
}

tt size_t Vector<T>::size() {
    return this->sz;
}

tt T& Vector<T>::operator[](size_t idx) {
    return this->buffer[idx];
}

tt const T& Vector<T>::operator[](size_t idx) const {
    return this->buffer[idx];
}

#undef tt
