#include <iostream>
#include "matrix.h"
using namespace std;

sparse_matrix *create(int m_size) {
    sparse_matrix* y = new sparse_matrix;                                       // dynamically allocate memory for a new sparse matrix
    for (int i = 0; i < m_size; i++) {                                          // loop to initialize sparse matrix to NULL (point to nothing)
        y->first_in_row[i] = NULL;
        y->first_in_column[i]= NULL;
    }
    return y;
}

void write_matrix(sparse_matrix *m, int m_size) {                               // function to print out matrix
    for (int i = 0; i < m_size; i++) {                                          // loop to go through each row of matrix
        if (m->first_in_row[i] == NULL) {                                       // check if row is empty
            for (int p = 0; p < m_size; p++) {                                  // if row is empty print out as many 0s as needed to take up a row
                cout << fixed << setw(8) << "0.00"; 
            }
            cout << endl;                                                       // newline to start printing on next row    
        }
        else {                                                                  // if row is not empty
            node* k = m->first_in_row[i];                                       // node pointer to first node in row
            for (int j = 0; j < m_size; j++) {                                  // for loop to go through columns
                if (j == k->column) {                                           // if column matches then a node place is found
                    cout << fixed << setprecision(2) << setw(8) << k->element;  // print out node data 
                    if (k->next_in_row != NULL) {                               // if there is a next node in row the set node pointer to the next node
                        k = k->next_in_row;                                     
                    }
                } 
                else {
                    cout << fixed << setw(8) << "0.00";                         // if node not found in column j then node data does not exist so print out 0
                }
            }
        cout << endl;                                                           // newline to start next row
        }
    }
    return;
}

void read_matrix(sparse_matrix *m, int m_size, ifstream& inputFile) {           // function to read sparse matrix from a file
    double g = 0;                                                               // g used to temporarity store data from file to check if it is 0 or not, if it is not 0 then program will create a new node to store it
    int f = 0;                                                                  // f acts as a flag to determine if first in row[i] is taken
    node *x = NULL;                                                             // x to dynamically create and edit/build new nodes
    node *y = NULL;                                                             // y to dynamically create new node after first in row is taken
    node *p = NULL;                                                             // p to find the last added node on column 
    node *u = NULL;                                                             // u to hold edit the last added node in column/ add next in column to it
    for (int i = 0; i < m_size; i++) {                                          // loop to hold value of what row we are in
        f = 0;                                                                  // reset flag to 0 whenever moving rows
        for (int j = 0; j < m_size; j++) {                                      // loop to hold value of what column we are in
            g = 0;                                                              // reset g to 0
            if (!inputFile.eof()){                                              // check if reached the end of data file
                inputFile >> g;
            }
            if (g != 0.0) {                                                     // if data retreived from file isnt 0 create a node to store it in
                if (f == 0) {                                                   // if-else to create nodes and store them in rows
                    m->first_in_row[i] = new node;
                    x = m->first_in_row[i];
                    x->element = g;
                    x->row = i;
                    x->column = j;
                    f = 1;
                } 
                else {
                    y = new node;
                    x->next_in_row = y;
                    x = y;
                    x->element = g;
                    x->row = i;
                    x->column = j;
                }
                if (m->first_in_column[j] == NULL) {                            // set columns
                    m->first_in_column[j] = x;
                }
                else {
                    p = m->first_in_column[j];
                    while (p!=NULL) {
                        u = p;
                        p = p->next_in_column;
                    }
                    u->next_in_column = x;
                }
            }
        }
    }
    return;
}

void multiply(sparse_matrix *m1, sparse_matrix *m2, sparse_matrix *m, int m_size) {
    node* x = NULL;                                                             // used to fill matrix 1 array with column indexs of nodes
    node* y = NULL;                                                             // used to fill matrix 2 array with row indeces of nodes
    node* g = NULL;                                                             // used to find element of m1 when row data index matches with column data index of m2
    node* f = NULL;                                                             // used to find element of m2 when column data index matches with row data indec of m1
    node* r = NULL;                                                             // used to dynamically allocate a new node for resulting matrix
    node* q = NULL;                                                             // used to find the last added node in row of new matrix to add a next node
    node* p = NULL;                                                             // used to set first in column and next in columns of result matrix
    node* u = NULL;                                                             // used to set first in column and next in columns of result matrix
    int m1a[MATRIX_SIZE];                                                       // used to store indexs of matrix 1 row to compare to indexs column of matrix 2 
    int m2a[MATRIX_SIZE];                                                       // used to store indexs of matrix 2 columns
    int a, b;                                                                   // used to set row and column of resulting matrix
    double h = 0;                                                               // used to store computation of dot product of row . column
    for ( int i = 0; i < m_size; i++ ) {                                        // for loop to go through rows of m1
        for (int z = 0; z < m_size; z++) {                                      // for loop to empty indexs from array
            m1a[z] = -1;
        }
        if (m1->first_in_row[i] != NULL) {                                      // if row[i] in m1 isnt empty, do multiplication to create column[z] in matrix m
            a = i;                                                              // save index i in a to set as node row in matrix m later
            x = m1->first_in_row[i];                                            
            m1a[x->column] = x->column;                                         // save column index of first in row in array
            while (x->next_in_row != NULL) {                                    // save column indexs of rest of nodes in row into array
                x = x->next_in_row;
                m1a[x->column] = x->column;
            }
            for (int z = 0; z < m_size; z++) {                                  // loop to go through column z of m2
                for (int i = 0; i < m_size; i++) {                              // loop to empty row indexs from array
                    m2a[i] = -1;
                }
                if (m2->first_in_column[z] != NULL) {                           // if and while to store row indexs of current column into array
                    b = z;
                    y = m2->first_in_column[z];
                    m2a[y->row] = y->row;
                    while (y->next_in_column != NULL) {
                        y = y->next_in_column;
                        m2a[y->row] = y->row;
                    }
                    h = 0;                                                      // reset to 0 after calculating one row[i] . column[j]  
                    for (int s = 0; s < m_size; s++) {                          // for loop to go through arrays to check for matching indexs 
                        if (m1a[s] == m2a[s] && m1a[s] != -1) {                 // if indexs match move g and f to nodes where they match and compute new node for matrix m
                            g = m1->first_in_row[a];
                            f = m2->first_in_column[b];
                            while (g->column != s){
                                g = g->next_in_row;
                            }
                            while (f->row != s) {
                                f = f->next_in_column;
                            }
                            h += (g->element * f->element);
                        }
                    }
                    if (h != 0) {                                               // create new node for matrix m if h holds a value after computation
                        if (m->first_in_row[a] == NULL) {
                            r = new node;
                            m->first_in_row[a] = r;
                            r->element = h;
                            r->row= a;
                            r->column = b;
                        }
                        else {
                            q = m->first_in_row[a];
                            while (q->next_in_row != NULL) {
                                q = q->next_in_row;
                            }
                            r = new node;
                            q->next_in_row = r;
                            r->element = h;
                            r->row = a;
                            r->column = b;
                        }
                        if (m->first_in_column[b] == NULL) {                    // set first in column and next in column for new matrix
                            m->first_in_column[b] = r;
                        }
                        else {
                            p = m->first_in_column[b];
                            while (p!=NULL) {
                                u = p;
                                p = p->next_in_column;
                            }
                            u->next_in_column = r;
                        }
                    }
                }
            }
        }
    }
    return;
}