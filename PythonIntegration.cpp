// PythonIntegration.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"

//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <Python.h>
#include <arrayobject.h>
#include <iostream>



/*PyObject *makearray(int arrayr[], size_t size) {
    npy_int dim = size;
    return PyArray_SimpleNewFromData(1, &dim, NPY_INT,(void *)arrayr);
}*/

PyObject *makearray(double inputarray[], size_t size)
{
	PyObject *list = PyList_New(size);
	 for (size_t i = 0; i < size; ++i) {
        PyList_SetItem(list, i, PyFloat_FromDouble(inputarray[i]));
    }
    return list;
}

int _tmain(int argc, _TCHAR* argv[])
{
	PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pValue2, *pArgs,*returnValue,*objectsRepresentation;

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString("py_function");

	if (pName)
	{
		std::cout << "pName read fine" << std::endl;
		PyObject* objectsRepresentation = PyObject_Repr(pName);
		PyObject* pyStr = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8","Error ~");
		const char *strExcType = PyBytes_AS_STRING(pyStr);
		std::cout << strExcType << std::endl;
		Py_XDECREF(objectsRepresentation);
		Py_XDECREF(pyStr);
	}

    // Load the module object
    pModule = PyImport_Import(pName);

	if (pModule)
	{
		std::cout << "pModule read fine" << std::endl;
	}
	else
	{
		PyErr_Print();
		system("pause");
		std::exit(1);
	}

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, "mult");

    if (PyCallable_Check(pFunc)) 
    {
		PyObject * image;
		pArgs = PyTuple_New(2);
		pValue = PyLong_FromLong(4);
		if (!pValue)
		{
			PyErr_Print();
			return -1;
		}
		pValue2 = PyLong_FromLong(5);
		if (pValue2 == NULL)
		{
			PyErr_Print();
			return -1;
		}
		double mylistarray[4] = {1,2,3,62.5645};
		PyObject* mylist = makearray(mylistarray,4);
		PyTuple_SetItem(pArgs, 0, mylist);
		PyTuple_SetItem(pArgs, 1, pValue2);
        image = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pValue);
		Py_DECREF(pValue2);
		Py_DECREF(pArgs);
		if (image == NULL || !PyList_Check(image))
		{
			PyErr_Print();
            return -1; /* Not a sequence, or other failure */
		}
		else
		{

			returnValue = PyList_GetItem(image,3);
			double d = PyFloat_AsDouble(returnValue);
			std::cout << "Returned value: " << d << std::endl;
			
		}

		 Py_DECREF(pModule);
    } else 
    {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();

	system("pause");

    return 0;
}

