#pragma once

void Print_Matrix(char s[], double m[], size_t rows, size_t cols);
void New_Matrix(int rows, int cols, double * res);
void Reset_Matrix(int rows, int cols, double * res);
double Get_Matrix(double m[], size_t rows, size_t cols, size_t row, size_t col);
void Matrix_T(double m[], size_t rows, size_t cols, double r[]);
void Add_Matrix(double m1[], double m2[], size_t rows, size_t cols, double r[]);
void Mult_Matrix(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[]);
void Matrix_Sigmoid(double m[], int rows, int cols);
