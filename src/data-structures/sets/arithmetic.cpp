#include <util.hpp>
#include <cstddef>

/** 
* The idea is that an arithmetic set uses an arithmetic function to
* find the next value in the set. 
*
* For example, if the set was 3, 9, and, 13, we could define the set as 
* f(x) = 3*x (mod 14), begin=3, end=11, sz=3
*       3   == 3
*     f(3)  == 9
*   f(f(3)) == 13 
*
* The difficult part of this representation is finding function `f` that
* satisfies the requirements above. 
*/

template<class T>
class ArithmeticSet {
public:
    ArithmeticSet();
    void insert(T &value);
    bool remove(T &value);
    size_t length();
    T* begin();
    T* end();
private:
    size_t size;
    T* next(T &val);
};

extern "C" {
    ArithmeticSet<size_t> new_ArithmeticSet() {return ArithmeticSet<size_t>();}
}

template<class T>
ArithmeticSet<T>::ArithmeticSet() {
    this->size = 0;
}

template<class T>
void ArithmeticSet<T>::insert(T &value) {
    return;
}

template<class T>
bool ArithmeticSet<T>::remove(T &value) {
    return false;
}

template<class T>
size_t ArithmeticSet<T>::length() {
    return this->size;
}

template<class T>
T* ArithmeticSet<T>::begin() {
    return NULL;
}

template<class T>
T* ArithmeticSet<T>::end() {
    return NULL;
}

