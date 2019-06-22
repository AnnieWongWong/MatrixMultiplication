#ifndef MATRIX_H_
#define MATRIX_H_
#include <fstream>
#include <iomanip>

const int MATRIX_SIZE = 18;

struct node { 
    float element;   // value of the entry 
    int row;   // row of the entry
    int column;   // column of the entry
    node *next_in_row;  // pointer to node contains next non-zero entry in row
    node *next_in_column;// pointer to node contains next non-zero entry in col
};

struct sparse_matrix {
    node *first_in_row[MATRIX_SIZE]; // pointers to first entry in every row 
    node *first_in_column[MATRIX_SIZE]; // pointers to first entry in every col
};

sparse_matrix *create(int m_size);
void read_matrix(sparse_matrix *m, int m_size, std::ifstream & inputFile);
void write_matrix(sparse_matrix *m, int m_size);
void multiply(sparse_matrix *m1, sparse_matrix *m2, sparse_matrix *m, int m_size);

#endif