#!/bin/sh
rm *.pb.*
~/DevEnv/root/bin/protoc *.proto --cpp_out=.
