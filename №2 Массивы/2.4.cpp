#include<stdio.h>


#define N 5

int main(){
 int a[N][N] = {0};
    
    int Ibeg = 0, Ifin = 0, Jbeg = 0, Jfin = 0;
    
    int k = 1;
    int i = 0;
    int j = 0;

    while (k <= N * N){
        a[i][j] = k;
        if (i == Ibeg && j < N - Jfin - 1)
            ++j;
        else if (j == N - Jfin - 1 && i < N - Ifin - 1)
            ++i;
        else if (i == N - Ifin - 1 && j > Jbeg)
            --j;
        else
            --i;

        if ((i == Ibeg + 1) && (j == Jbeg) && (Jbeg != N - Jfin - 1)){
            ++Ibeg;
            ++Ifin;
            ++Jbeg;
            ++Jfin;
        }
        ++k;
    }
    
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j)
            printf("%3d", a[i][j]);
        printf("\n");
    }
}
