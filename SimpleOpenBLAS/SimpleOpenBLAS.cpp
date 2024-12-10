#include <iostream>
#include <chrono>
#include <cblas.h>

// 矩阵向量乘法：4x4 矩阵与 4x1 向量
void transformCoordinates(const double* vMatrix, const double* vVector, double* vResultMatrix) {
    // 使用 cblas_dgemv 进行矩阵-向量乘法
    // CBLAS_ROW_MAJOR 表示矩阵按行存储
    cblas_dgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0, vMatrix, 4, vVector, 1, 0.0, vResultMatrix, 1);
}

int main()
{
    double pTransformMatrix[16] = {
        1.0, 0.3, 0.4, 5.0,
        2.0, 1.4, 4.0, 5.0,
        0.2, 4.0, 1.0, -1.0,
        0.0, 0.0, 0.0, 1.0
    };

    double pWorldCoordinate[4] = { 1.0, 2.0, 3.0, 1.0 };
    double pScreenCoordinate[4] = { 0.0, 0.0, 0.0, 0.0 };
    transformCoordinates(pTransformMatrix, pWorldCoordinate, pScreenCoordinate);

    auto Start = std::chrono::high_resolution_clock::now();
    long long TransformCount = 0;

    while (true) 
    {
        transformCoordinates(pTransformMatrix, pWorldCoordinate, pScreenCoordinate);
        TransformCount++;
        auto Now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> Elapsed = Now - Start;
        if (Elapsed.count() >= 1.0) 
            break;
    }

    std::cout << "Transforms per second: " << TransformCount << '\n';
    return 0;
}