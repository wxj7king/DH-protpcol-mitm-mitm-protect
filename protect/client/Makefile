#!/bin/bash
AES_GCM=aes_gcm.c aes_gcm.h
DH=AES_DH.c AES_DH.h
C:DHClient.c ${DH} ${AES_GCM}
	gcc DHClient.c ${DH} ${AES_GCM} -o DHC -lcrypto
clean:
	rm -f DHC 
