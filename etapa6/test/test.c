#include <stdio.h>

int main(){

    int var1 = 1;
    char var2 = 'C';
    char var3[] = "HELLO WORLD TESTANDO UMA STRING AINDA MAIOR";
    float var4 = 4.5;
    int var5[] = {1, 2, 3};
    float var6[] = {1.2, 2.4, 3.6};
    char var7[] = {'A', 'B', 'C'};

    printf("5");
    printf("A");
    printf("STR");
    printf("STR\t");
    printf("STR\n");
    printf("\n");
    printf("\t");
    printf("\\");
    printf("\"\n\t");
    printf("%d", 55);
    printf("%d \n", 55);
    printf("%d", var1);
    printf("%d \n", var1);
    printf("%c", var2);
    printf("%c \n", var2);
    printf("%s", var3);
    printf("%s \n", var3);
    printf("%f", var4);
    printf("%f \n", var4);
    printf("%d", var5[1]);
    printf("%d \n", var5[1]);
    printf("%f", var6[1]);
    printf("%f \n", var6[1]);
    printf("%c", var7[1]);
    printf("%s \n", var7[1]);

    return 0;
}
