#!/bin/bash

# This script is used to build on platforms without custom instructions.

set -e

main()
{
	mkdir --parents build
	cd build
	cmake ..
	make package
}

main;
