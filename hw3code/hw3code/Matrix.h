//
//  Matrix.h
//  hw3code
//
//  Created by mike Levine on 2/13/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//
#ifndef __hw3code__Matrix__
#define __hw3code__Matrix__
#endif /* defined(__hw3code__Matrix__) */
#include <stdio.h>
#include <iostream>
#include <fstream>
typedef unsigned int size_type;
class Matrix
{
public:
    //CONSTRUCTORS, ASSIGNMNENT OPERATOR, & DESTRUCTOR
    Matrix();
    Matrix(size_type aNum_rows, size_type aNum_cols, double aFill);
    Matrix(Matrix const &m); //copy constructor
    ~Matrix() {this->clear(); }
    bool operator==(const Matrix& m);
    bool operator!=(const Matrix & m);
    Matrix operator=(const Matrix& m);
    
    //ACCESSORS
    size_type num_rows() const {return rows; }
    size_type num_cols() const {return cols; }
    double** get_array() const { return arr; }
    bool get (size_type aRow, size_type aCol, double& nums) const;
    double* get_row(size_type aRow) const;
    double* get_col(size_type aCol) const;
    
    //Modifers
    void clear();
    bool set(size_type aRow, size_type aCol, double nums);
    void multiply_by_coefficient(double num);
    void swap_row(size_type row1, size_type row2);
    bool add(Matrix &m);
    bool subtract(Matrix &m);
    void transpose();
    Matrix* quarter();
    void resize(size_type aRow, size_type aCol, double fill);
private:
    //PRIVATE VARIABLES
    size_type rows;
    size_type cols;
    double fill;
    double** arr;
    
    
};
std::ostream& operator<< (std::ostream& out, const Matrix& m);