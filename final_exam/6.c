#include <stdio.h>

int main(int argc, char* argv[]){
    int i;
    
    printf("Queue Order Listing.\n");
    for(i=1; i<argc; i++)//1부터 인자 개수 까지
        printf("%s\n",argv[i]);

    printf("\nStack Order Listing.\n");
    for(i=argc-1; i>0; i--)//인자 개수부터 1까지
        printf("%s\n", argv[i]);
    
    return 0;
}
