Popis programu:
	Program implementuje ako klientsk�, tak aj serverov� aplik�ciu v C realizuj�cu sprostredkovanie inform�ci� o u��vate�och

Obmedzenia programu:
  Na serveri eva.fit.vutbr.cz klient nedok�e nadviaza� spojenie so serverom, nako�ko server pracuje ako IPv6 server a preto
  je potrebn� zada� -h <host> ako -h eva6.fit.vutbr.cz namiesto eva.fit.vutbr.cz

Pr�klad spustenia:
	./ipk-client -h <host> -p <port> [-n|-f|-l] <login>
  ./ipk-server -p <port>

Odovzdan� s�bory:
	ipk-client.c
  ipk-server.c
	Makefile
	dokumentace.pdf
	readme.txt
