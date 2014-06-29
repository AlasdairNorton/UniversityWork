# Basic Makfile for comp20003 AlgDS assignment 1, semester 2, 2013
# Makes two dictionaries: listdict, based on linked list, skipdict, based on
#           skiplist
# Assumes two separate files with main() functions-- dict.c for the linked list
#           implementation and dict2.c for the skiplist
# Also assumes list functions are in list.c, which includes list.h
#        and skiplist functions are in skiplist.c, which includes skiplist.h
# Adapted from the provided makefile by removing reference to dict.h


listdict: dict.o list.o 
	gcc -o listdict dict.o list.o 

skipdict: dict2.o skiplist.o
	gcc -o skipdict dict2.o skiplist.o 

list.o: list.c list.h
	gcc -c -Wall list.c

skiplist.o: skiplist.c skiplist.h
	gcc -c -Wall skiplist.c

dict.o: dict.c list.h
	gcc -c -Wall dict.c

dict2.o: dict2.c skiplist.h
	gcc -c -Wall dict2.c

