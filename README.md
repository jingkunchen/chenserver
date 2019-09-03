HTTPServer http://github.com/snowdroper/chenserver
TCP server, Written in C++, A lightweight server (TCP, UDP, ICMP), written in C++, The server is built with libevent and thread pool and is used to develop network applications. It is an asynchronous multi-thread high concurrency framework.
Motivation: * To allow running high concurrency server service on a development computer without needing to install any third party Library or software.
Dependencies: * libevent-dev * libpthread-dev
Features: * Works on Linux as well as POSIX systems. * Realizes a high-performance asynchronous multi threads processing model for TCP data streams * Does not require any extensions.
Caveats: * Should NOT be used as a production web server open to untrusted traffic. * Not very robust (e.g. no connection limit or timeouts) * May have security flaws
How to use: See examples/example at test.txt.
License: See LICENSE.txt
