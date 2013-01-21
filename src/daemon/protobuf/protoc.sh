#!/bin/sh
rm *.pb.*
protoc --cpp_out=. *.proto
