#include<stdio.h>

#define N 3

int main(){
	int Arr[N][N];
	int count = 0;
	for(int i=0; i<N; i++){ 
		for(int j=0; j<N; j++){
			Arr[i][j] = count;
			printf("%d", Arr[i][j]);
			count++;
		}
		printf("\n");
	}
}