#ifndef MATLIB_H
#define MATLIB_H

#include "main.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "arm_math.h"

typedef struct _mat{
    int row, col;
    float* val;
    arm_matrix_instance_f32 M;
} mat;
/**
 * @brief Initialice matrix without any value 
 * @param  m Matrix (struct mat)
 * @param  row  Number of row (int)
 * @param col Number of columns (int)
*/
void matInit(mat* m, int row, int col);
/**
 * @brief Multiplication of 2 matrix  
 * @param  a (struct mat)
 * @param  b (struct mat)
 * @param ans Value of matrix multiplication (struct mat)
*/
void matMult(mat* ans, mat* a, mat* b);
/**
 * @brief Transform quaternion values to Roll Pitch Yaw  
 * @param q1 element of quaternion (float)
 * @param q2 element of quaternion (float) 
 * @param q3 element of quaternion (float) 
 * @param q4 element of quaternion (float)
 * @param R Roll Pitch Yaw column matrix(struct mat) 
*/
void quaternionToR(mat* R, float q1, float q2, float q3, float q4);
/**
 * @brief Q to floating value   
 * @param q (int)
*/
float q2float(int q);
/**
 * @brief floating value to q  
 * @param d (float)
*/
int float2q(float d);
/**
 * @brief Fill matrix wit floating numbers   
 * @param i Row (int)
 * @param j Column (int)
 * @param value Floating value for a certain column and row (float)
 * @param m Matrix (struct mat) 
*/
void setMatVal(mat* m, int i, int j, float value);
/**
 * @brief Get a number for a given column and row of the matrix   
 * @param i Row (int)
 * @param j Column (int)
 * @param m Matrix (struct mat) 
*/
float getMatVal(mat* m, int i, int j);
/**
 * @brief Delete matrix to save memory    
 * @param i Row (int)
 * @param j Column (int)
*/
void matDestruct(mat* m);
/**
 * @brief Multiply a matrix with real value    
 * @param alpha Scle (float)
 * @param a Matrix (struct mat)
 * @param ans  Scaled matrix (struct mat) 
*/
void matScale(mat* ans, mat* a, float alpha);
/**
 * @brief Add two matrices    
 * @param a Matrix 1 (struct mat) 
 * @param b Matrix 2 (struct mat) 
 * @param ans  Sum of two matrices (struct mat) 
*/
void matAdd(mat* ans, mat* a, mat* b);
/**
 * @brief Difference between two matrices    
 * @param a Matrix 1 (struct mat) 
 * @param b Matrix 2 (struct mat) 
 * @param ans  Substraction of two matrices (struct mat) 
*/
void matSubs(mat* ans, mat* a, mat* b);
/**
 * @brief Inverse of a 3x3 matrix   
 * @param R Matrix (struct mat)
 * @param Rinv Inversed matrix (struct mat) 
*/
void matInv(mat* Rinv, mat* R);
/**
 * @brief Transpose of a matrix   
 * @param R Matrix (struct mat)
 * @param Rt Transposed matrix (struct mat) 
*/
void matTrans(mat* Rt,mat* R);
/**
 * @brief Copy values from source matrix to destination matrix    
 * @param src Source matrix (struct mat)
 * @param des Destination matrix (struct mat)
*/
void matCopy(mat* des, mat* src);
/**
 * @brief Transform Euler's angles to rotation matrix    
 * @param roll (float)
 * @param pitch (float)
 * @param yaw (float)
 * @param R Rotation matrix (struct mat) 
*/
void rpyToR(mat* R, float roll, float pitch, float yaw);
/**
 * @brief  Gauss Elimination of 3x3 matrix    
 * @param a Matrix (struct mat) 
 * @param b Matrix (struct mat) 
 * @param ans Matrix (struct mat) 
*/
void gaussElimination3x3(mat* a, mat* b, mat* ans);

/**
 * @brief Get the Determinant of a matrix     
 * @param A Matrix (struct mat)
 * @return Determinant of a matrix (float)
*/
float matDet(mat* A);

#endif
