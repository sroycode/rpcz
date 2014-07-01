rpcz
====

RPCZ: Protocol Buffer RPC transport

Forked from https://code.google.com/p/rpcz/

1. Added changes to compile on OSX in CMakeLists.txt for boost_system
2. Added changes to find standard zeromq without zmq.hpp which is included in include
3. Assume GLOG is there for testing
4. Added two examples of client and server coexisting

Compile
=======

To Compile and install with headers in /my/location/
<pre>
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/my/location -Drpcz_install_headers=1 -Drpcz_build_examples=1
make
make install
</pre>
