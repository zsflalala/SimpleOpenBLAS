#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <cblas.h>

struct Vec4
{
    float x, y, z, w;

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct Mat4
{
    float m[4][4];

    Mat4()
	{
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) 
                m[i][j] = 0;
    }

    static Mat4 ortho(float left, float right, float bottom, float top, float near, float far)
	{
        Mat4 matrix;
        matrix.m[0][0] = 2.0f / (right - left);
        matrix.m[1][1] = 2.0f / (top - bottom);
        matrix.m[2][2] = -2.0f / (far - near);
        matrix.m[0][3] = -(right + left) / (right - left);
        matrix.m[1][3] = -(top + bottom) / (top - bottom);
        matrix.m[2][3] = -(far + near) / (far - near);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }
};

// 生成测试数据
std::vector<Vec4> GenerateTestData(float min, float max, int count)
{
    std::vector<Vec4> test(count);

    float minBounds[4] = { min, min, min, min };
    float maxBounds[4] = { max, max, max, max };

    int numPerAxis = static_cast<int>(std::round(std::pow(count, 1.0f / 3.0f)));
    int index = 0;

    for (int i = 0; i < numPerAxis; ++i)
    {
        for (int j = 0; j < numPerAxis; ++j) 
        {
            for (int k = 0; k < numPerAxis; ++k)
            {
                if (index >= count) break;
                float x = minBounds[0] + (maxBounds[0] - minBounds[0]) * (static_cast<float>(i) / (numPerAxis - 1));
                float y = minBounds[1] + (maxBounds[1] - minBounds[1]) * (static_cast<float>(j) / (numPerAxis - 1));
                float z = minBounds[2] + (maxBounds[2] - minBounds[2]) * (static_cast<float>(k) / (numPerAxis - 1));
                test[index] = Vec4(x, y, z, 1.0f);
                index++;
            }
            if (index >= count) break;
        }
        if (index >= count) break;
    }
    return test;
}

void TestPerformance(int numPoints, const Mat4& transformMatrix)
{
    std::vector<Vec4> worldPoints = GenerateTestData(-1000.0f, 1000.0f, numPoints);

    // OpenBLAS: 转换矩阵到列主序（这是cblas_sgemv所需的格式）
    std::vector<float> transformMatrixFlat(16);
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++)
        {
            transformMatrixFlat[i * 4 + j] = transformMatrix.m[i][j];
        }
    }

    float sp[4] = { 0 };
    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) 
    {
        for (int i = 0; i < numPoints; ++i) 
        {
            const float wp[4] = { worldPoints[i].x, worldPoints[i].y, worldPoints[i].z, worldPoints[i].w };

            cblas_sgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0f,
                transformMatrixFlat.data(), 4, wp, 1, 0.0f, sp, 1);
        }
        iterations++;
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - start;
        if (elapsed.count() >= 5.0) break;
    }

    std::cout << "Compute in 1s: " << iterations * numPoints / 5 << std::endl;
}

int main() {
    // 设置变换矩阵，投影矩阵（例如：正交投影矩阵）
    Mat4 transformMatrix = Mat4::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 0.1f, 1000.0f);

    // 测试性能：在 1 秒钟内进行多少次矩阵运算
    for (int i = 0;i < 5;i++)
    {
        TestPerformance(100000, transformMatrix);
    }

    return 0;
}
