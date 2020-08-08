#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool zoro(int row, int col, int m, int n, int a[m][n])
{
    a[row][col] = 0;
    return true;
}
static void foreach(int m, int n, int a[m][n], bool (*cback)(int row, int col, int m, int n, int a[m][n]))
{
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if (false == cback(i, j, m, n, a)) return;
        }
    }
}

static void print(int m, int n, int a[m][n])
{
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("%3d ", a[i][j]);
        }
        printf("\n");
    }
}

static void multiply(int m, int n, int p, int a[m][n], int b[n][p], int c[m][p])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            c[i][j] = 0;
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int fundamental_mx()
{
    enum { r1 = 3, c1 = 4, r2 = c1, c2 = r1 };

    //   int (*mat1)[r1][c1] = malloc( sizeof(int[r1][c1]) ) ;
    //   int (*mat2)[r2][c2] = malloc( sizeof(int[r2][c2]) ) ;
    //   int (*ans )[r1][c2] = malloc( sizeof(int[r1][c2]) ) ;

    if (r2 != c1) {
        printf("The matrices are incompatible.\n");
        exit(EXIT_FAILURE);
    }

    int mat1[r1][c1] = {
        {1,2,3,4},
        {1,2,3,4},
        {1,2,3,4}
    },
        mat2[r2][c2] = {
            {1,2,3},
            {1,2,3},
            {1,2,3},
            {1,2,3}
    },
        ans[r1][c2] = {};
    printf("The elements of the first matrix are :\n");
    print(r1, c1, mat1);
    printf("The elements of the second matrix are :\n");
    print(r2, c2, mat2);

    foreach(r1, c1, mat1, zoro);
    printf("The elements of the first matrix are :\n");
    print(r1, c1, mat1);

    foreach(r2, c2, mat2, zoro);
    printf("The elements of the second matrix are :\n");
    print(r2, c2, mat2);

    multiply(r1, r2, c2, mat1, mat2, ans);
    printf("The product is :\n");
    print(r1, c2, ans);

    return EXIT_SUCCESS;
}