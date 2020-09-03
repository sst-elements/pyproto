// Copyright 2009-2019 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2019, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"

#include <Python.h>

#pragma clang diagnostic pop

#include <structmember.h>

#include <vector>

#include <sst/core/output.h>

#include "pymodule.h"
#include "pyproto.h"

extern "C" {

static PyMethodDef moduleMethods[] = {
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "sst.pyproto",
    nullptr,
    1,
    moduleMethods,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static int pyEvent_Init(PyEvent_t *, PyObject *, PyObject *);
static void pyEvent_Dealloc(PyEvent_t *);
static PyMethodDef pyEventMethods[] = {
    {nullptr, nullptr, 0, nullptr}
};
static PyMemberDef pyEventMembers[] = {
    {(char *) "type", T_OBJECT, offsetof(PyEvent_t, type), 0, (char *) "Type of Event"},
    {(char *) "sst", T_OBJECT, offsetof(PyEvent_t, dict), 0, (char *) "SST Event members"},
    {nullptr, 0, 0, 0, nullptr}
};

static PyTypeObject PyEventDef = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "sst.pyproto.PyEvent",
    sizeof(PyEvent_t),
    0,
    (destructor) pyEvent_Dealloc,
    0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE),
    "PyProto Event",
    nullptr,
    nullptr,
    nullptr,
    0,
    nullptr,
    nullptr,
    pyEventMethods,
    pyEventMembers,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0,
    (initproc) pyEvent_Init,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0,
    nullptr,
    nullptr
};

static int pyLink_Init(PyLink_t *, PyObject *, PyObject *);
static void pyLink_Dealloc(PyLink_t *);
static PyObject *pyLink_recv(PyObject *, PyObject *);
static PyObject *pyLink_send(PyObject *, PyObject *);

static PyMethodDef pyLinkMethods[] = {
    {"recv", pyLink_recv, METH_NOARGS, "Receive from a link"},
    {"send", pyLink_send, METH_VARARGS, "Send on a link"},
    {nullptr, nullptr, 0, nullptr}
};

static PyTypeObject PyLinkDef = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "sst.pyproto.PyLink",
    sizeof(PyLink_t),
    0,
    (destructor) pyLink_Dealloc,
    0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE),
    "PyProto Link",
    nullptr,
    nullptr,
    nullptr,
    0,
    nullptr,
    nullptr,
    pyLinkMethods,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0,
    (initproc) pyLink_Init,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0,
    nullptr,
    nullptr
};

static int pyProto_Init(PyProto_t *, PyObject *, PyObject *);
static void pyProto_Dealloc(PyProto_t *);
static PyObject *pyProto_addLink(PyObject *, PyObject *);
static PyObject *pyProto_addClock(PyObject *, PyObject *);
static PyObject *pyProto_construct(PyObject *, PyObject *);
static PyObject *pyProto_init(PyObject *, PyObject *);
static PyObject *pyProto_setup(PyObject *, PyObject *);
static PyObject *pyProto_finish(PyObject *, PyObject *);

static PyMethodDef pyProtoMethods[] = {
    {"addLink", pyProto_addLink, METH_VARARGS, "Add a Link"},
    {"addClock", pyProto_addClock, METH_VARARGS, "Add a clock handler"},
    {"construct", pyProto_construct, METH_NOARGS, "Called during Construction"},
    {"init", pyProto_init, METH_O, "Called during init"},
    {"setup", pyProto_setup, METH_NOARGS, "Called during setup"},
    {"finish", pyProto_finish, METH_NOARGS, "Called during finish"},
    {nullptr, nullptr, 0, nullptr}
};

static PyTypeObject PyProtoDef = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "sst.pyproto.PyProto",
    sizeof(PyProto_t),
    0,
    (destructor) pyProto_Dealloc,
    0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE),
    "PyProto Prototype",
    nullptr,
    nullptr,
    nullptr,
    0,
    nullptr,
    nullptr,
    pyProtoMethods,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0,
    (initproc) pyProto_Init,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0,
    nullptr,
    nullptr
};

void *genPyProtoPyModule(void) {
    // Must return a PyObject

    /* Initialize Types */
    PyEventDef.tp_new = PyType_GenericNew;
    PyLinkDef.tp_new = PyType_GenericNew;
    PyProtoDef.tp_new = PyType_GenericNew;

    if ((PyType_Ready(&PyEventDef) < 0) || (PyType_Ready(&PyLinkDef) < 0) || (PyType_Ready(&PyProtoDef) < 0)) {
        SST::Output::getDefaultObject().fatal(CALL_INFO, -1,
                                              "Failed to instantiate PyProto Python Module");
    }

    PyObject *module = PyModule_Create(&moduledef);
    if (!module)
        SST::Output::getDefaultObject().fatal(CALL_INFO, -1,
                                              "Failed to instantiate PyProto Python Module");

#if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
    Py_INCREF(&PyEventDef);
    Py_INCREF(&PyLinkDef);
    Py_INCREF(&PyProtoDef);
#if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#endif

    PyModule_AddObject(module, "PyEvent", (PyObject *) &PyEventDef);
    PyModule_AddObject(module, "PyLink", (PyObject *) &PyLinkDef);
    PyModule_AddObject(module, "PyProto", (PyObject *) &PyProtoDef);

    Py_INCREF(module); /* Want to return a NEW reference */
    return module;
}

/*****      PyEvent      *****/

static int pyEvent_Init(PyEvent_t *self, PyObject *, PyObject *) {
    self->type = PyUnicode_FromString("Python");
    self->dict = PyDict_New();
    PyErr_Print();
    return 0;
}

static void pyEvent_Dealloc(PyEvent_t *self) {
    Py_XDECREF(self->type);
    Py_XDECREF(self->dict);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

/*****      PyLink      *****/

static int pyLink_Init(PyLink_t *self, PyObject *args, PyObject *) {
    char *p;
    if (!PyArg_ParseTuple(args, "Osn", &self->object, &p, &self->portNumber)) {
        return -1;
    }

    self->portName = strdup(p);
    Py_XINCREF(self->object);

    return 0;
}

static void pyLink_Dealloc(PyLink_t *self) {
    Py_XDECREF(self->object);
    free(self->portName);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *pyLink_recv(PyObject *self, PyObject *) {
    auto *l = (PyLink_t *) self;
    auto *p = (SST::PyProtoNS::PyProto *) ((PyProto_t *) l->object)->component;
    PyEvent_t *e = p->doLinkRecv(l->portNumber);
    if (e) {
        return (PyObject *) e;
    } else {
        Py_XINCREF(Py_None);
        return Py_None;
    }
}

static PyObject *pyLink_send(PyObject *self, PyObject *args) {
    auto *l = (PyLink_t *) self;
    auto *p = (SST::PyProtoNS::PyProto *) ((PyProto_t *) l->object)->component;

    PyObject *event = nullptr;
    if (!PyArg_ParseTuple(args, "O!", &PyEventDef, &event)) {
        return nullptr;
    }

    Py_XINCREF(event);
    p->doLinkSend(l->portNumber, (PyEvent_t *) event);
    return PyLong_FromLong(0);
}

/*****      PyProto      *****/

static int pyProto_Init(PyProto_t *self, PyObject *args, PyObject *) {
    char *name;
    if (!PyArg_ParseTuple(args, "s", &name)) {
        return -1;
    }

    std::cout << "lol\n";

    self->name = strdup(name);
    self->clocks = new PyProto_t::clockArray_t();
    self->links = new PyProto_t::linkArray_t();
    self->constructed = false;

    PyObject *sys_mod_dict = PyImport_GetModuleDict();
    PyObject *sst_mod = PyMapping_GetItemString(sys_mod_dict, (char *) "sst");
    self->tcomponent = PyObject_CallMethod(sst_mod, (char *) "Component",
                                           (char *) "ss", self->name, (char *) "pyproto.PyProto");
    if (!self->tcomponent) {
        SST::Output::getDefaultObject().fatal(CALL_INFO, -1,
                                              "Failed to load sst.Component\n");
    }
    SST::PyProtoNS::PyProto::addComponent(self);

    return 0;
}

static void pyProto_Dealloc(PyProto_t *self) {
    Py_XDECREF((PyObject *) self->tcomponent);
    delete self->links;
    delete self->clocks;
    free(self->name);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *pyProto_addLink(PyObject *self, PyObject *args) {
    auto *pself = (PyProto_t *) self;
    if (pself->constructed) {
        SST::Output::getDefaultObject().fatal(CALL_INFO, -1,
                                              "Cannot add a Link once construction complete.");
    }

    PyObject *slink = nullptr;
    char *lat = nullptr;
    PyObject *cb = nullptr;
    if (!PyArg_ParseTuple(args, "Os|O", &slink, &lat, &cb)) {
        return nullptr;
    }

    size_t pnum = pself->links->size();
    char port[16] = {0};
    snprintf(port, 15, "port%zu", pnum);
    PyObject_CallMethod(pself->tcomponent, (char *) "addLink",
                        (char *) "Oss", slink, port, lat);

    /* Push the callback (or nullptr) onto the stack */
    Py_XINCREF(cb);
    pself->links->push_back(std::make_pair(port, cb));
    if ((pnum + 1) != pself->links->size())
        SST::Output::getDefaultObject().fatal(CALL_INFO, -1,
                                              "Looks like a threading bug!\n");


    /* Allocate new Python-facing object */
    PyObject *sys_mod_dict = PyImport_GetModuleDict();
    PyObject *module = PyMapping_GetItemString(sys_mod_dict, (char *) "sst.pyproto");
    PyObject *nlink = PyObject_CallMethod(module, (char *) "PyLink",
                                          (char *) "Osn", pself, port, pnum);

    return nlink;
}

static PyObject *pyProto_addClock(PyObject *self, PyObject *args) {
    auto *pself = (PyProto_t *) self;
    if (pself->constructed) {
        SST::Output::getDefaultObject().fatal(CALL_INFO, -1,
                                              "Cannot add a clock once construction complete.");
    }

    PyObject *cb = nullptr;
    char *freq = nullptr;

    if (!PyArg_ParseTuple(args, "Os", &cb, &freq)) {
        SST::Output::getDefaultObject().output("Bad arguments for function PyProto.addClock()\n");
        return nullptr;
    }

    Py_INCREF(cb);
    pself->clocks->push_back(std::make_pair(cb, freq));

    return PyLong_FromLong(0);
}

static PyObject *pyProto_construct(PyObject *, PyObject *) {
    return PyLong_FromLong(0);
}

static PyObject *pyProto_init(PyObject *, PyObject *) {
    return PyLong_FromLong(0);
}

static PyObject *pyProto_setup(PyObject *, PyObject *) {
    return PyLong_FromLong(0);
}

static PyObject *pyProto_finish(PyObject *, PyObject *) {
    return PyLong_FromLong(0);
}

} /* extern "C" */


namespace SST {
namespace PyProtoNS {
PyTypeObject *getEventObject() { return &PyEventDef; }

PyTypeObject *getPyProtoObject() { return &PyProtoDef; }

PyTypeObject *getPyLinkObject() { return &PyLinkDef; }
}
}
