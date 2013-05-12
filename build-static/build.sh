#!/bin/bash

g++ ../h3c8021x.cpp  -c
ar rv h3c8021x.a h3c8021x.o
g++ ../main.cpp  -c
g++ ../md5_ctx.cpp -c
g++ h3c8021x.o  main.o  md5_ctx.o /usr/lib/libpcap.a -o h3cc-static



