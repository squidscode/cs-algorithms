#include <util.hpp>
#include <cstddef>

#define tt template<class T>

tt class Vector {
public:
    Vector();
    ~Vector();
    void push_back(T&);
    T pop_back();
    void insert(size_t idx, T&);
    size_t size();
    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

private:
    size_t capacity;
    size_t sz;
    T* buffer;
    void resize();
};

C_CONSTRUCTOR(new Vector<int>, Vector, default);
C_DESTRUCTOR(Vector<int>*, Vector, default);

tt Vector<T>::Vector() {
    this->capacity = 1;
    this->sz = 0;
    this->buffer = new T[1];
}

tt Vector<T>::~Vector() {
    delete this->buffer;
}

tt void Vector<T>::push_back(T& val) {
    if (this->sz == this->capacity) {
        this->resize();
    }
    this->buffer[this->sz] = val;
    ++this->sz;
}

tt void Vector<T>::resize() {
    delete this->buffer;
    this->capacity <<= 1;
    this->buffer = new T[this->capacity];
}

tt T Vector<T>::pop_back() {
    return this->buffer[--this->sz];
}

tt void Vector<T>::insert(size_t idx, T& val) {
    if (this->sz == this->capacity) {
        this->resize();
    }
    ++this->size;
    for (size_t i = this->size - 1; i > idx; --i) {
        this->buffer[i] = this->buffer[i-1];
    }
    this->buffer[idx] = val;
}

tt size_t Vector<T>::size() {
    return this->size;
}

tt T& Vector<T>::operator[](size_t idx) {
    return this->buffer[idx];
}

tt const T& Vector<T>::operator[](size_t idx) const {
    return this->buffer[idx];
}

#undef tt
