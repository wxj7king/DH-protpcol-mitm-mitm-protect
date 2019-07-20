#!/bin/bash

all:DHServer.c DHClient.c
	gcc DHServer.c -o DHS -lcrypto
	gcc DHClient.c -o DHC -lcrypto	
S:DHServer.c
	gcc DHServer.c -o DHS -lcrypto
C:DHClient.c
	gcc DHClient.c -o DHC -lcrypto
clean:
	rm -f DHC
	rm -f DHS  
