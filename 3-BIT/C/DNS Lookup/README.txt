Popis programu:
	Program implementuje klientsk� aplik�ciu v C, ktor� sa dotazuje syst�mu DNS a realizuje preklad dom�nov�ch mien a IP adries

Obmedzenia programu:
  Program pri niektor�ch typoch iterat�vneho dotazu nezobraz� po ceste k v�sledku v�etky NS a A z�znamy, ktor� sa na danej 
  ceste nach�dzaj� (napr. po vyp�san� z�znamu 147.in-addr.arpa. IN NS r.arin.net. program vypisuje ako �al�� z�znam 
  r.arin.net. IN A 199.180.180.6 a ignoruje z�znamy, ktor� sa nach�dzaj� medzi t�mito dvoma z�znamami.

Pr�klad spustenia:
	./ipk-lookup -s 8.8.8.8 www.fit.vutbr.cz
  ./ipk-lookup -s 8.8.8.8 -t CNAME www4.fit.vutbr.cz.
  ./ipk-lookup -s 8.8.8.8 -t PTR 2001:67c:1220:8b0::93e5:b013
  ./ipk-lookup -s 8.8.8.8 -t PTR -i 147.229.13.238

Odovzdan� s�bory:
	ipk-lookup.c
	Makefile
	dokumentace.pdf
	README.txt
