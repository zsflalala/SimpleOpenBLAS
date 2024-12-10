#include <iostream>
#include <cblas.h>

static void updateMatrixWithOuterProduct()
{
    double x[2] = { 1.0, 2.0 };
    double y[3] = { 2.0, 1.0, 3.0 };
    double A[6] = { 0 };
    blasint rows  = 2, cols  = 3;
    blasint inc_x = 1, inc_y = 1;
    blasint lda   = 2;
    double  alpha = 10;
    //矩阵按列优先存储
    //A <== alpha*x*y' + A （y'表示y的转置）
    cblas_dger(CblasColMajor, rows, cols, alpha, x, inc_x, y, inc_y, A, lda);
    for (double i : A)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

int main()
{
    updateMatrixWithOuterProduct();
}
