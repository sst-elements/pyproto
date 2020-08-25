#!/usr/bin/env python
#
# Copyright 2009-2019 NTESS. Under the terms
# of Contract DE-NA0003525 with NTESS, the U.S.
# Government retains certain rights in this software.
#
# Copyright (c) 2009-2019, NTESS
# All rights reserved.
#
# Portions are copyright of other developers:
# See the file CONTRIBUTORS.TXT in the top level directory
# the distribution for more information.
#
# This file is part of the SST software package. For license
# information, see the LICENSE file in the top level directory of the
# distribution.


class PyEvent():
    def __init__(self):
        pass


class PyLink():
    def __init__(self, sstLink, latency, callback):
        pass

    def recv(self):
        pass

    def send(self, ev):
        pass


class PyProto():
    def __init__(self, name):
        print(name)
        pass

    def addLink(self, link, latency, callback=None):
        pass

    def addClock(self, callback, rate):
        pass

    def construct(self):
        pass

    def init(self, phase):
        pass

    def setup(self):
        pass

    def finish(self):
        pass
