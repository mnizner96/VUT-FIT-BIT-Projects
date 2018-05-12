Popis programu:
	Program implementuje ako klientskú, tak aj serverovú aplikáciu v C realizujúcu sprostredkovanie informácií o uívate¾och

Obmedzenia programu:
  Na serveri eva.fit.vutbr.cz klient nedokáe nadviaza spojenie so serverom, nako¾ko server pracuje ako IPv6 server a preto
  je potrebné zada -h <host> ako -h eva6.fit.vutbr.cz namiesto eva.fit.vutbr.cz

Príklad spustenia:
	./ipk-client -h <host> -p <port> [-n|-f|-l] <login>
  ./ipk-server -p <port>

Odovzdané súbory:
	ipk-client.c
  ipk-server.c
	Makefile
	dokumentace.pdf
	readme.txt
