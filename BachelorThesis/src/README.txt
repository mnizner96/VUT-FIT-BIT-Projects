Hlavn� skript m� n�zov Ruian.php a cel� �as na �tandartn� v�stup informuje o aktu�lnom priebehu.
Pred spusten�m skriptu je potrebn� nakonfigurova� datab�zu a prepoji� ju so skriptom: 
	1. Ur��me si server, na ktorom budeme chcie� datab�zu zalo�i� (napr�klad lok�lny MySql server 
	  pomocou n�stroja Xampp - https://www.apachefriends.org/download.html
	2. Na danom serveri zalo��me nov� datab�zu, do ktorej budeme chice� uklada� d�ta R�IAN.
	3. Nad zalo�enou datab�zou spust�me sql {\bf sql/db.sql} (cesta z kore�ov�ho adres�ra skriptu).
	4. V s�bore RuianDatabase.php, ktor� je v kore�ovom adres�ri skriptu vypln�me kon�tanty 
	   HOST, DBNAME, USER a PASS pod�a konfigur�cie serveru, na ktorom sme vytvorili datab�zu. 
           Od tohto momentu je PHP skript prepojen� s datab�zou.

Skript m� dve mo�nosti spustenia: 
	1. php Ruian.php - pre zmazanie existuj�cej datab�zy a jej naplnenie d�tami z posledn�ho
			  mesa�n�ho stavov�ho s�boru
	2. php Ruian.php --update - pre aktualiz�ciu existuj�cej datab�zy pomocou denn�ch zmenov�ch
                                   s�borov vygenerovan�ch od posledn�ho d�tumu aktualiz�cie datab�zy
 				   do s��asn�ho d�tumu.
	                
V pr�pade spustenia skriptu vo Windows je mo�n� pou�i� php verziu v zlo�ke php:
Ak m�me napr�klad prilo�en� zlo�ku php na syst�movom disku C tak spust�me n�sledovn�m sp�sobom:
	1. cd C://php
	2. php ruian/Ruian.php alebo php ruian/Ruian.php --update