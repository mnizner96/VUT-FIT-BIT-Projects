Popis programu:
	Program implementuje jednoduch� LDAP server, ktor� pr�jma a analyzuje spr�vy zo strany klienta a na z�klade t�chto spr�v potom generuje a odosiela ptorebn� odpove� pre klienta.

Obmedzenia programu:
	Program negeneruje polo�ku ObjectName pri generovan� spr�vy SearchResEntry(), posiela iba cn re�azec a mail re�azec.
	Pokia� nie je zadan� parameter size, tak program neuva�uje �iadne size obmedzenie a odo�le klientovi v�ekty vyhovuj�ce z�znamy.
	Program podporuje iba datab�zy, ktor� maj� k�dovanie UTF-8.
	Program nepodporuje ��adne roz��renia.

Pr�klad spustenia:
	./myldap -p <port> -f <soubor>

Odovzdan� s�bory:
	myldap.c
	Makefile
	manual.pdf
	README.txt
