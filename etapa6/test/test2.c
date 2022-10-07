#include <stdio.h>
    int a = 6;
    int b = 8;
void main(){
    a = 1 + 2;
    b = a + 5;
    b = 5 + a;
    a = a + b;
    printf("%d", a);
}
