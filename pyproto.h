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

#ifndef COMPONENTS_PYPROTO_PYPROTO_H
#define COMPONENTS_PYPROTO_PYPROTO_H

#include <cinttypes>
#include <atomic>
#include <sst/core/component.h>
#include <sst/core/link.h>
#include <sst/core/event.h>

extern "C" struct PyEvent_t;
extern "C" struct PyProto_t;
extern "C" typedef struct _object PyObject;

namespace SST {
namespace PyProtoNS {

class PyEvent : public SST::Event {
  public:
    explicit PyEvent(PyEvent_t *);

    ~PyEvent() override;

    PyEvent_t *getPyObj() { return pyE; }

  private:
    PyEvent_t *pyE{};

    PyEvent() = default; // For serialization only

  public:
    void serialize_order(SST::Core::Serialization::serializer &ser) override {
        Event::serialize_order(ser);
        // TODO:  Serialize pyE
    }

  ImplementSerializable(SST::PyProtoNS::PyEvent);

};

class PyProto : public SST::Component {
  public:

    SST_ELI_REGISTER_COMPONENT(
        PyProto,
        "pyproto",
        "PyProto",
        SST_ELI_ELEMENT_VERSION(1, 0, 0),
        "Python Prototyping Component",
        COMPONENT_CATEGORY_UNCATEGORIZED
    )

    SST_ELI_DOCUMENT_PORTS(
        { "port%d", "Link to another object", {}}
    )

    PyProto(SST::ComponentId_t, SST::Params &);

    ~PyProto() override;

    void init(unsigned int) override;

    void setup() override;

    void finish() override;

    static void addComponent(PyProto_t *);

    PyEvent_t *doLinkRecv(size_t);

    void doLinkSend(size_t, PyEvent_t *);

  protected:
    bool clock(SST::Cycle_t, PyObject *);

    void linkAction(Event *, size_t);

  private:
    PyProto_t *that; /* The Python-space representation of this */
    std::vector<SST::Link *> links;

    static std::vector<PyProto_t *> pyObjects;
    static std::atomic<size_t> pyObjIdx;
};

}
}

#endif
