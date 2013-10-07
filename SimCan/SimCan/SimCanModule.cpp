#include "Python.h"
#include <conio.h>
/* they are included in Python.h header
#include <stdlib.h>
#include <stdio.h>
*/
#include "SimCan.h"

#define PY_TRUE  1
#define PY_FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

static PyObject *
ex_CanOpen(PyObject *self, PyObject *args)
{
	const char *CanBus;//if we want to use unicode support, then 
	                   //this project need to be improved
	CanHandle hInterface;


	if (!PyArg_ParseTuple(args, "s", &CanBus))
        return NULL;

	if ((hInterface = CanOpen(CanBus)) == NULL)
	{
        printf( "!!ERROR, %lu when calling \"CanOpen\"\n",
                     GetLastError() );
		return NULL;
	}
	else
	{
		return Py_BuildValue("i", hInterface);
	}
}

static PyObject *
ex_CanClose(PyObject *self, PyObject *args)
{
	CanHandle hInterface;
	
	if (!PyArg_ParseTuple(args, "i", &hInterface))
        return NULL;

    if (!CanClose( hInterface ))
	{
        printf( "!!ERROR, %lu when calling \"CanClose\"\n",
			GetLastError());
		return NULL;
	}
	else
		return Py_BuildValue("i", PY_TRUE);
}

static PyObject *
ex_CanReceive(PyObject *self, PyObject *args)
{
	DWORD dataLength;
	CanHandle hInterface;
	CanMsg receivedCanMsg;
	CanMsgId *CanMsgSel = (CanMsgId *)malloc(sizeof(CanMsgId));
	CanFrameType frameType;
	CanTimeStamp timeStamp;
	PyObject *listObj;
	ULONG listSize;
	ULONG i;
	
	if (!PyArg_ParseTuple(args, "iO!", &hInterface, &PyList_Type, &listObj))
        return NULL;
	
	listSize = PyList_Size(listObj);

	if (listSize > 1)
	{
		printf("Current filter: [ ");
		for (i=0; i < listSize; i++)
		{
			CanMsgSel[i] = (CanMsgId )malloc(sizeof(CanMsgId));
			CanMsgSel[i] = (ULONG )PyLong_AsLong(PyList_GetItem(listObj, i));
			printf("%#x ", CanMsgSel[i]);
		}
		printf("]\n");
	}
	else
		//no filter is set
		*CanMsgSel = NULL;

	if(!CanReceive(
		hInterface,
		&receivedCanMsg,
		&dataLength,
		CanMsgSel,
		CAN_FRAME_STANDARD,
		&frameType,
		500))
	{
		printf( "!!ERROR, %lu when calling \"CanReceiveEx\"\n",
		GetLastError() );
		return NULL;
	}
	else
	{
		CanGetLastTimeStamp( hInterface, &timeStamp );
		printf( "CAN message received\n" );
		printf( "\tTime stamp (ns): %lu%lu\n",
			timeStamp.high, timeStamp.low );
		printf( "\tId: %#x\n", receivedCanMsg.id );
		printf( "\tData:" );

		for (i=0; i < dataLength; i++)
		{
			printf(" %#2x", receivedCanMsg.data[i] );
		}

		printf("\n");
		
		//build result list
		PyObject *dataList = PyList_New(dataLength);
		for (i=0; i < dataLength; i++)
		{
			PyObject *item = PyInt_FromLong(receivedCanMsg.data[i]);
			if (!item) {
				Py_DECREF(dataList);
				return NULL;
			}
			PyList_SET_ITEM(dataList, i, item);
		}
		
		PyObject *resultDict = PyDict_New();
		PyDict_SetItemString(resultDict, "id", PyInt_FromLong(receivedCanMsg.id));//save CAN id
		PyDict_SetItemString(resultDict, "data", dataList);

		return resultDict;
	}
}

static PyMethodDef SimCan_methods[] = {
	{"can_open", ex_CanOpen, METH_VARARGS,
	"CanOpen method."},
	{"can_close", ex_CanClose, METH_VARARGS,
	"CanClose method."},
	{"can_receive", ex_CanReceive, METH_VARARGS,
	"CanReceive method."},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initSimCan(void)
{
	Py_InitModule("SimCan", SimCan_methods);
}

#ifdef __cplusplus
}
#endif
