#!/bin/sh
rm *.pb.*
protoc *.proto --cpp_out=.
