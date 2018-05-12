Popis programu:
	Program implementuje klientskú aplikáciu v C, ktorá sa dotazuje systému DNS a realizuje preklad doménových mien a IP adries

Obmedzenia programu:
  Program pri niektorých typoch iteratívneho dotazu nezobrazí po ceste k výsledku všetky NS a A záznamy, ktoré sa na danej 
  ceste nachádzajú (napr. po vypísaní záznamu 147.in-addr.arpa. IN NS r.arin.net. program vypisuje ako ïalší záznam 
  r.arin.net. IN A 199.180.180.6 a ignoruje záznamy, ktoré sa nachádzajú medzi týmito dvoma záznamami.

Príklad spustenia:
	./ipk-lookup -s 8.8.8.8 www.fit.vutbr.cz
  ./ipk-lookup -s 8.8.8.8 -t CNAME www4.fit.vutbr.cz.
  ./ipk-lookup -s 8.8.8.8 -t PTR 2001:67c:1220:8b0::93e5:b013
  ./ipk-lookup -s 8.8.8.8 -t PTR -i 147.229.13.238

Odovzdané súbory:
	ipk-lookup.c
	Makefile
	dokumentace.pdf
	README.txt
