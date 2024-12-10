#include <iostream>
#include <chrono>
#include <cblas.h>

// 假设世界坐标到屏幕坐标的变换使用 4x4 矩阵乘法
void multiplyMatrix(const float* vMatrixA, const float* vMatrixB, float* pResultMatrixC, int vSize)
{
    cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
        vSize, vSize, vSize, 1.0f, vMatrixA, vSize, vMatrixB, vSize, 0.0f, pResultMatrixC, vSize);
}

int main()
{
    const int MatrixSize = 4;       // 使用 4x4 矩阵
    const int Iterations = 1000000; // 每秒进行 100 万次变换

    // 创建 4x4 矩阵，初始化为单位矩阵
    float pMatrixA[MatrixSize * MatrixSize] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float pMatrixB[MatrixSize * MatrixSize] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float pResultMatrix[MatrixSize * MatrixSize]; // 结果矩阵

    auto StartTime = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < Iterations; ++i) 
    {
        multiplyMatrix(pMatrixA, pMatrixB, pResultMatrix, MatrixSize); // 进行矩阵乘法运算
    }
    auto EndTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> Duration = EndTime - StartTime;

    double TransformsPerSecond = Iterations / Duration.count();
    std::cout << "每秒完成的变换次数: " << TransformsPerSecond << std::endl;
    return 0;
}