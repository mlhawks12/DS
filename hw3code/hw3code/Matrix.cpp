//
//  Matrix.cpp
//  hw3code
//
//  Created by mike Levine on 2/13/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include "Matrix.h"
#include <iostream>
typedef unsigned int size_type;
//default constructor
Matrix::Matrix()
{
    rows = 0;
    cols = 0;
    fill = 0;
    arr = NULL;
}
//constructor
Matrix::Matrix(size_type aNum_rows, size_type aNum_cols, double aFill)
{
    
    rows = aNum_rows;
    cols = aNum_cols;
    fill = aFill;
    if(rows == 0 || cols == 0)
    {
        Matrix();
    }
    else
    {
    arr = new double*[rows];
    for(size_type x = 0; x< rows; x++)
    {
        arr[x] = new double[cols];
        for(size_type y = 0; y< cols; y++)
            arr[x][y] = fill;
    }
    }
}
//copy constructor
Matrix::Matrix(Matrix const &m)
{
    if(m.rows == 0 || m.cols == 0)
    {
        rows = 0;
        cols = 0;
        fill = 0;
        arr = NULL;
    }
    else
    {
        this->rows = m.rows;
        this->cols = m.cols;
        arr = new double*[rows];
        for(size_type x = 0; x< rows; x++)
        {
            arr[x] = new double[cols];
            for(size_type y = 0; y< cols; y++)
                arr[x][y] = m.arr[x][y];
        }
    }
}
//deletes all memory dynamically allocated in the heap
void Matrix::clear()
{
    if(arr == NULL)
        return;
    for(size_type x = 0; x< rows; x++)
    {
        delete [] arr[x];
    }
    delete [] arr;
    rows = 0;
    cols = 0;
    arr = NULL;
}

//set a specified spot in the matrix to a specified value
bool Matrix::set(size_type aRow, size_type aCol, double nums)
{
    if(aRow < rows && aCol < cols)
    {
        arr[aRow][aCol] = nums;
        return true;
    }
    return false;
}
//gets a value from the matrix from a specified location and sets num equal to it which is passed by reference
//returns ture if it is able to get the num and false otherwise
bool Matrix::get(size_type aRow, size_type aCol, double& nums) const
{
    if(aRow < rows && aCol < cols)
    {
        nums =  arr[aRow][aCol];
        return true;
    }
    return false;
}
//overloads == operator
//returns true if it is equal and false othersise
bool Matrix::operator==(const Matrix &m)
{
    if(this->rows != m.rows || this->cols != m.cols)
        return false;
    for(size_type x = 0; x< this->rows; x++)
    {
        for(size_type y = 0; y< this->cols; y++)
        {
            if(this->arr[x][y] != m.arr[x][y])
                return false;
        }
    }
    return true;
}
//overloads == operator
//returns false if it is equal and true othersise
bool Matrix::operator!=(const Matrix &m)
{
    if(this->rows != m.rows || this->cols != m.cols)
        return true;
    for(size_type x = 0; x< this->rows; x++)
    {
        for(size_type y = 0; y< this->cols; y++)
        {
            if(this->arr[x][y] != m.arr[x][y])
                return true;
        }
    }
    return false;
}
//overloads = operator
//copys the matrix
Matrix Matrix::operator=(const Matrix& m)
{
    if(m.rows == 0 || m.cols == 0)
    {
        rows = 0;
        cols = 0;
        fill = 0;
        arr = NULL;
    }
    else
    {
        this->rows = m.rows;
        this->cols = m.cols;
        arr = new double*[rows];
        for(size_type x = 0; x< rows; x++)
        {
            arr[x] = new double[cols];
            for(size_type y = 0; y< cols; y++)
                arr[x][y] = m.arr[x][y];
        }
    }
    return *this;
}
//adds one matrix by another
//returns true if able to and false otherwise
bool Matrix::add(Matrix &m)
{
    if(m.rows == this->rows && m.cols == this->cols)
    {
        for(size_type x = 0; x< this->rows; x++)
        {
            for(size_type y = 0; y< this->cols; y++)
                this->arr[x][y]+=m.arr[x][y];
        }
        return true;
    }
    return false;
}
//subtracts one matrixby another
//returns true if able to and false otherwise
bool Matrix::subtract(Matrix &m)
{
    if(m.rows == this->rows && m.cols == this->cols)
    {
        for(size_type x = 0; x< this->rows; x++)
        {
            for(size_type y = 0; y< this->cols; y++)
                this->arr[x][y]-=m.arr[x][y];
        }
        return true;
    }
    return false;
}
//returns a pointer to a copy of a row in the matrix
double* Matrix::get_row(size_type aRow) const
{
    if(aRow > rows || arr == NULL)
    {
        double* t = NULL;
        return t;
    }
    double* temp = new double[cols];
    for(size_type x = 0; x<cols; x++)
        temp[x] = *(arr[aRow]+x);
    return temp;
}
//returns a copy of a column in the matrix
double* Matrix::get_col(size_type aCol) const
{
    if(aCol > cols || arr == NULL)
    {
        double* t = NULL;
        return t;
    }
    double* temp = new double[rows];
    for(size_type x = 0; x<rows; x++)
        temp[x] = *(arr[x]+aCol);
    return temp;
}
//swaps two rows in a matrix and deletes the temp array made
void Matrix::swap_row(size_type row1, size_type row2)
{
    if(arr != NULL)
    {
    double* temp = new double[cols];
    temp = arr[row1];
    arr[row1] = arr[row2];
    arr[row2] = temp;
    delete [] temp;
    }
}
//multiply the matrix by a number
void Matrix::multiply_by_coefficient(double num)
{
        for(size_type x = 0; x< rows; x++)
        {
            for(size_type y = 0; y< cols; y++)
                arr[x][y]*= num;
        }
}
//makes an array of 4 matrixs to each have quarter of the Matrix
Matrix* Matrix::quarter()
{
    Matrix* q = new Matrix[4];
    size_type new_row = (rows+1)/2;
    size_type new_col = (cols+1)/2;
    for(size_type x = 0; x<4; x++)
        q[x] = Matrix(new_row,new_col,0);
    size_type r;
    size_type c;
    for(size_type x = 0; x < 4; x++)
    {
        r = 0;
        c = 0;
        if((x+1)%2 == 0)//when in matrix LL and LR offset the place you are taking values from in the big matrix
            c+=cols/2;
        if(x>1)
            r+=rows/2;//when in UR and LR offset the place you are taking values from in the big matrix
        for(size_type y = 0; y < new_row; y++)
        {
            for(size_type z = 0; z < new_col; z++)
            {
                q[x].arr[y][z] = this->arr[y+r][z+c];
            }
        }
    }
    return q;//return a pointer to the array of matrixs
}
//transposes the matrix by creating a new matrix with the rows and cols switch, then flips the for loop variable to place in corresponding places
void Matrix::transpose()
{
    Matrix m = Matrix(cols, rows, 0);
    if(arr != NULL)
    {
    for(size_type x = 0; x < rows; x++)
    {
        for(size_type y = 0; y < cols; y++)
        {
            m.arr[y][x] = this->arr[x][y];
        }
    }
    //deletes all newly allocated memory in the heap
    for(size_type x = 0; x< rows; x++)
        delete [] arr[x];
    delete [] arr;
    }
    *this = Matrix(m);
}
//resizes the matrix to a specified size and fills all values will a specified fill value if it is in the new part of the matrix
void Matrix::resize(size_type aRow, size_type aCol, double fill)
{
    
    Matrix m = Matrix(aRow, aCol, fill);
    if(arr!= NULL)
    {
    for(size_type x = 0; x < rows; x++)
    {
        for(size_type y = 0; y < cols; y++)
        {
            m.arr[x][y] = this->arr[x][y];
        }
    }
    //deletes all newly allocated memory in the heap
    for(size_type x = 0; x< rows; x++)
        delete [] arr[x];
    delete [] arr;
    }
    *this = Matrix(m);
}
//overloads the << operator to print a Matrix object out
//writes to a file and then returns the reference to it to be printed out
std::ostream& operator<< (std::ostream& out, const Matrix& m)
{
    out <<std::endl<< m.num_rows() << " x " << m.num_cols() << " matrix:" <<std::endl << "[ ";
    for(size_type x = 0; x<m.num_rows(); x++)
    {
        for(size_type y = 0; y<m.num_cols(); y++)
        {
            out << m.get_array()[x][y] << " ";
        }
        
        if(x == m.num_rows()-1)
        {
            out<< "]";
        }
        else
            out<<std::endl << "  ";
    }
    if(m.num_rows() == 0 && m.num_rows() == 0)
        out << "]";
    out<<std::endl;
    return out;
}









