#!/bin/bash
gcc \
	-I/home/chriph/Projects/EE/include/ \
	-L/home/chriph/Projects/EE/bin \
	./include/main.c \
	-lcws \
	-o ./bin/game \
	-g \
	-rdynamic \
	-Wall \
	-ldl \
	-lm