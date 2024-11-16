#include<stdio.h>

#define N 5

int main(){
	int Arr[N];
	
	for(int i=0; i<N; i++){ 
		scanf("%d", &Arr[i]);
	}
	
	printf("Input: ");
	for(int i=0; i<N; i++){ 
		printf("%d", Arr[i]);
	}	
	printf("\n");
	
	printf("Output: ");
	for(int i=N-1; i>=0; i--){ 
		printf("%d", Arr[i]);
	}
	
}