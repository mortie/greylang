CC=gcc
STD=c99
PROJECT=grey
PREFIX=/usr/local/bin

FLAGS=-O3
FLAGS-DBG=-g
FLAGS-VALGRIND=--track-origins=yes

WARNINGS=all error
LINKS=m readline dl
