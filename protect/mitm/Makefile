#!/bin/bash
AES=aes_gcm.c aes_gcm.h
DH=DH.c DH.h
all:mitm.c DHServer.c DHClient.c ${AES} ${DH}
#	gcc mitm.c ${AES} ${DH} -o mitm -lcrypto
	gcc DHServer.c ${AES} ${DH} -o DHS -lcrypto
	gcc DHClient.c ${AES} ${DH} -o DHC -lcrypto	
S:DHServer.c ${AES} ${DH}
	gcc DHServer.c ${AES} ${DH} -o DHS -lcrypto
C:DHClient.c ${AES} ${DH}
	gcc DHClient.c ${AES} ${DH} -o DHC -lcrypto
#M:mitm.c ${AES} ${DH}
#	gcc mitm.c ${AES} ${DH} -o mitm -lcrypto
M:mitm.c
	gcc mitm.c -o mitm -lcrypto
clean:
	rm -f mitm
	rm -f DHC
	rm -f DHS  
