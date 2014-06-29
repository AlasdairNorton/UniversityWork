# Makefile for COMP20003 Assignment 2

helpme: graph.o heap.o emergencyresponse.o
	gcc -o helpme graph.o heap.o emergencyresponse.o

graph.o: graph.c graph.h heap.h
	gcc -c -Wall graph.c

heap.o: heap.c heap.h
	gcc -c -Wall heap.c

emergencyResponse.o: emergencyresponse.c heap.h graph.h
	gcc -c -Wall emergencyresponse.c
