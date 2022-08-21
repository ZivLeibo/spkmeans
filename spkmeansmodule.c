/*
 Created by Ziv on 10/10/2021.
*/
#define PY_SSIZE_T_CLEAN
#define ALLOCATION_ERROR                                 \
        {                                                \
        printf("Error allocating memory; Bailing out!"); \
        assert(-1);                                      \
        }
#include <Python.h>
#include "spkmeans.h"
#include "spkmeans.c"

/* functions declaration: */
static PyObject* fit(PyObject *self, PyObject *args);
static PyObject* pass_to_spkmeans(PyObject *py_obj_dots,PyObject *py_obj_matrix, int goal);


/* c-python-API components */
static PyMethodDef capiMethods[] = {
        {"fit",
                (PyCFunction) fit,
                     METH_VARARGS,
                PyDoc_STR("Implementing the requested algorithem - wam(2), ddg(3), lnorm(4), jacobi(5), kmeans(6)")},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        NULL,
        -1,
        capiMethods
};

PyMODINIT_FUNC
PyInit_spkmeansmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}

/*-------------------------------- fit() function: ------------------------------------------------------*/

static PyObject* fit(PyObject *self, PyObject *args)
{
    int  goal;
    PyObject *py_obj_dots,*py_obj_matrix;

    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if(!PyArg_ParseTuple(args, "OOi",&py_obj_dots, &py_obj_matrix, &goal)) {
        return NULL;
    }

    if (!PyList_Check(py_obj_matrix) || !PyList_Check(py_obj_dots))
        return NULL;

    return Py_BuildValue("O", pass_to_spkmeans(py_obj_dots, py_obj_matrix, goal));
}

/*-------------------------------- pass_to_kmeans----------------------------------------------------------*/
/* extract the primitive C data from the pyObjects  */

static PyObject* pass_to_spkmeans(PyObject *py_obj_dots,PyObject *py_obj_matrix, int goal){

    /* variables declaration */
    double **dots, **matrix;
    double *array;
    PyObject *final_list, *temp_list, *temp_dot;
    Py_ssize_t i, j, n, k, d=0, n_plus_one;
    n = PyList_Size(py_obj_dots);
    n_plus_one = n+1;
    k = PyList_Size(py_obj_matrix);
    dots = (double**)malloc(n*sizeof(double*));
    array = (double*) calloc(n,sizeof(double));
    matrix = (goal==6)? malloc(k*sizeof(double*)):malloc(n*sizeof(double*));
    if(!dots || !matrix || !array) ALLOCATION_ERROR

    /* assigns py_objects in c dynamic arrays */
    /* in dots*/
    for(i=0; i<n; i++){
        temp_dot = PyList_GetItem(py_obj_dots, i);
        if(i==0)
            d=PyList_Size(temp_dot);
        dots[i] = (double*) calloc(d, sizeof(double));
        if(!dots[i]) ALLOCATION_ERROR
        for(j=0; j<d; j++)
            dots[i][j] = PyFloat_AsDouble(PyList_GetItem(temp_dot, j));
    }

    /* in matrix */
    if(goal == 6)
        for (i = 0; i < k; i++) {
            temp_dot = PyList_GetItem(py_obj_matrix, i);
            matrix[i] = (double *) calloc(d, sizeof(double));
            if (!matrix[i]) ALLOCATION_ERROR
            for (j = 0; j < d; j++)
                matrix[i][j] = PyFloat_AsDouble(PyList_GetItem(temp_dot, j));
        }

    else
        for(i=0; i<n; i++){
            matrix[i] = calloc(n, sizeof(n));
            if(!matrix[i]) ALLOCATION_ERROR
        }

    /* pass the data to the requested function */
    if(goal == 6)
        kmeans_calc(dots, matrix, array, n, d, k);
    else
        apply_requested_algo(goal,dots,matrix,array, n,d,k,0);
    if (goal==3)
        convert_diagonal_to_matrix(matrix,array,n);

    /* assign the final matrix to return to python in the py_object matrix */
    if(goal == 6) {
        final_list = PyList_New(k);
        for (i = 0; i < k; i++) {
            temp_list = PyList_New(d);
            for (j = 0; j < d; j++)
                PyList_SetItem(temp_list, j, PyFloat_FromDouble(matrix[i][j]));
            PyList_SetItem(final_list, i, temp_list);
        }
        for(i=0; i<k; i++)
            free(matrix[i]);
    }
    else if (goal != 5){
        final_list = PyList_New(n);
        for (i = 0; i < n; i++) {
            temp_list = PyList_New(n);
            for (j = 0; j < n; j++)
                PyList_SetItem(temp_list, j, PyFloat_FromDouble(matrix[i][j]));
            PyList_SetItem(final_list, i, temp_list);
        }
        for(i=0; i<n; i++)
            free(matrix[i]);
    }
    else{
        final_list = PyList_New(n_plus_one);
        for (i = 0; i < n_plus_one; i++) {
            temp_list = PyList_New(n);
            for (j = 0; j < n; j++)
                if(i==0)
                    PyList_SetItem(temp_list, j, PyFloat_FromDouble(array[j]));
                else
                    PyList_SetItem(temp_list, j, PyFloat_FromDouble(matrix[i-1][j]));
            PyList_SetItem(final_list, i, temp_list);
        }
        for(i=0; i<n; i++)
            free(matrix[i]);
    }

    /* frees */
    for(i=0; i<n; i++)
        free(dots[i]);
    free(dots);
    free(matrix);

    return final_list;
}

