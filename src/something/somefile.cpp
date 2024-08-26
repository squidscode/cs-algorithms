#include <string>

extern "C" {
    int fn();
    int add(int a, int b);
}

int fn() {
	return 1;
}

int add(int a, int b) {
    return a + b;
}

int unshared() {
    return 0;
}

int some_other_function(std::string woah) {
    return 1;
}
