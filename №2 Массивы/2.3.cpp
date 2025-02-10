#include<stdio.h>

#define N 5

int main(){
	int Arr[N][N];
	int count = N-1;
	for(int i=0; i<N; i++){ 
		for(int j=0; j<N; j++){
			
			if(j<count) Arr[i][j] = 0;
			else Arr[i][j] = 1;
					
			printf("%d", Arr[i][j]);
			
		}
		count--;
		printf("\n");
	}
}