Popis programu:
	Program implementuje jednoduchı LDAP server, ktorı príjma a analyzuje správy zo strany klienta a na základe tıchto správ potom generuje a odosiela ptorebnú odpoveï pre klienta.

Obmedzenia programu:
	Program negeneruje poloku ObjectName pri generovaní správy SearchResEntry(), posiela iba cn reazec a mail reazec.
	Pokia¾ nie je zadanı parameter size, tak program neuvauje iadne size obmedzenie a odošle klientovi všekty vyhovujúce záznamy.
	Program podporuje iba databázy, ktoré majú kódovanie UTF-8.
	Program nepodporuje íadne rozšírenia.

Príklad spustenia:
	./myldap -p <port> -f <soubor>

Odovzdané súbory:
	myldap.c
	Makefile
	manual.pdf
	README.txt
