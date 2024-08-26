extern "C" {
    int fib_recursive(int);
    int fib_linear(int);
}

int fib_recursive(int n) {
    if (n < 2) {
        return n;
    }
    return fib_recursive(n-1) + fib_recursive(n-2);
}

int fib_linear(int n) {
    if(n < 2) {
         return n;
    }
    int p1 = 0;
    int p2 = 1;
    for(int i = 0; i < n-1; ++i) {
        p2 = p1 + p2;
        p1 = p2 - p1;
    }
    return p2;
}
