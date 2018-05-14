/*
*	Author: Martin Nizner
*	Project: IPK Proj1 : Klient server pro vyhledani uzivatelu FIT VUT Brno
*/

#include<arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <err.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

void hex_transform(char *hex_tmp, int num) {   //funkcia pre prevod hodnoty int na hexadecimalnu hodnotu typu char (uvazuje hexadecimalne cislo na max 2 byty)
  int k = 0;
  int i = 0;
  int size = 0;
  char hex_one[2048];   //pomocne pole charov pre ukladanie hexadecimalneho cisla
  while(num) {   //cyklus pre nacitanie hexadecimalneho cisla do pomocneho pola charov
    hex_one[i++] = num % 16;
    num /= 16;
    size++;
  }
  char c;
  i = 0;
  while(size > 0) {
    if (size > 1) {   //ak je hexadecimalne cislo rozlozene na viac ako 1byte je potrebne upravit vkladanie vysledku do vysledneho charu
                    //pomocou bytoveho posunu
      hex_tmp[i++] = hex_one[k] | hex_one[k + 1] << 4;
    }
    else {
      hex_tmp[i++] = hex_one[k];
    }
    hex_one[k + 1] = 0x00;   //vynulovanie hodnot pomocneho pola
    hex_one[k] = 0x00;

  k = k + 2;
  size = size - 2;
  }
}

char hex_character (int num) {    //funkcia pre preklad hexa cisel na char
    if (num == 0)
        return '0';
    else if (num == 1)
        return '1';
    else if (num == 2)
        return '2';
    else if (num == 3)
        return '3';
    else if (num == 4)
        return '4';
    else if (num == 5)
        return '5';
    else if (num == 6)
        return '6';
    else if (num == 7)
        return '7';
    else if (num == 8)
        return '8';
    else if (num == 9)
        return '9';
    else if (num == 10)
        return 'a';
    else if (num == 11)
        return 'b';
    else if (num == 12)
        return 'c';
    else if (num == 13)
        return 'd';
    else if (num == 14)
        return 'e';
    else if (num == 15)
        return 'f';

}
int set_type(char *type) {     //funkcia zistujuca typ DNS zaznamu
    if (strcmp(type, "A") == 0) 
        return 1;
    else if (strcmp(type, "AAAA") == 0)
        return 28;
    else if (strcmp(type, "NS") == 0)
        return 2;
    else if (strcmp(type, "PTR") == 0)
        return 12;
    else if (strcmp(type, "CNAME") == 0)
        return 5;
    else return 3;
}
int main(int argc, char *argv[]) {
	char server[2000];     //premenna pre ulozenie serveru
	
    char type[2000];       //premenne pre uloznie typu zaznamu
    char tmp_type[20];     
    char tmp_iterative[20];
	
    int timeout = 5;       //timeout pre vyhladavanie
	struct timeval tv;
    
    int help_flag, timeout_flag, type_flag, iterative_flag, server_flag = 0;  //pomocne premenne pre parsovanie argumentov
	int c;
	int num_of_get = 0;
	
    struct sockaddr_in sa;   //premenne pre komunikaciu s DNS serverom
	int client_socket;
	
    char name[2000];         //premenna pre ulozenie ziskaneho DNS zaznamu
    
    unsigned char recv_buffer[2000];   //premenna pre ulozenie prijatej odpovede
    
    char iterative_pom[2000];        //pomocne premenne vyuzite pri iterativnom vyhladavani 
    char iterative_cat[2000];
    char recv_iterative[2000];
    char iterative_reverse_pom[100];
    int iterative_start = 0;
    int iterative_finish = 0;
    int rec_index_iterative = 0;
    int iterative_movement_counter = 0;
    int first_time_iterative = 0;
    int iterative_counter  = 0;
    int iterative_cat_counter = 0;
    

    int state = 0;       //premenna urcujuca stav konecneho automatu
    
    unsigned short dns_id;   //ientifikator urcujuci ID dns spravy
    
    char hex_pom[10];      //premenna pre ulozenie jedneho hexadecimalneho cisla
    
    int size = 0;         //premenna urcujuca velkost prijmanej spravy
    
    int counter = 0;      //premenne urcujuce polohu konretneho znaku v odoslatej a prijatej sprave 
    int len = 0;
    int rec_index = 0;  
    
    int answer_size = 0;   //pomocna premenna urcujuca velkost urciteho useku prijatej DNS odpovede
    
    int tmp_equal = 0;     //pomocna premenna pre zistenie ci je prijaty zaznam ten, ktory hladame
    
    struct in6_addr v6_addr;  //premenne pre bitove ulozenie IPv4 resp. IPv6 adresy v pripade reverzneho typu zaznamu
    struct in_addr v4_addr;
    

    strcpy(type, "A");
	
    while ((c = getopt(argc, argv, "hs:T:t:i")) != -1) {  //cyklus pre parsovanie argumentov prikazoveho riadku
		switch(c) {
			case 'h':
				help_flag = 1;
				num_of_get++;
				break;
			case 's':
				num_of_get = num_of_get + 2;
				server_flag = 1;
				strcpy(server, optarg);
				break;
			case 'T':
				num_of_get = num_of_get + 2;
				timeout_flag = 1;
				timeout = atoi(optarg);
				break;
			case 't':
				num_of_get = num_of_get + 2;
				type_flag = 1;
				strcpy(type, optarg);
				break;
			case 'i':
				num_of_get++;
				iterative_flag = 1;
				break;
			default:
				fprintf(stderr, "Wrong type of argument\n");
				return 1;
		}				

	}

	if (help_flag == 1 && argc == 2) {        //vypisanie napovedy
		printf("Napoveda pre DNS lookup\n");
		printf("Tento program pracuje s tymito parametrami: \n");
		printf("-h               zobrazi napovedu\n");
		printf("-s <server>		 povinny parameter, DNS server(IPv4), na ktory sa budu odosielat dotazy\n");
		printf("-T <timeout>	 nepovinny parameter, timeout pre dotaz v sekundach\n");
		printf("-t <type>		 nepovinny parameter, typ dotazovaneho zaznamu (A, AAAA, NS...)\n");
		printf("-i               nepovinny parameter, vynutenie iterativneho sposobu rezolucie\n");
		printf("name             prekladane domenove meno\n");
		return 0;
	}
	else if (help_flag == 0){       //kontrola ci boli spravne zadane vstupne argumenty
		if (server_flag == 0) {
			fprintf(stderr, "Wrong arguments\n");
			return 2;
		}
		else if (argc != num_of_get + 2) {
			fprintf(stderr, "Missing DNS name\n");
			return 2;
		}
	}

	strcpy(name, argv[num_of_get + 1]);
	if ((inet_pton(AF_INET, server, &(sa.sin_addr))) == 0) {   //kontrola ci je vstupna adresa typu IPv4
		fprintf(stderr, "Wrong IP adress\n");
		return 2;
	}

	sa.sin_family= AF_INET;       //nastavovanie struktury pre komunikaciu s DNS serverom
	sa.sin_port = htons(53);
    //printf("tu\n");
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {   //vytovrenie socketu pre komunikaciu so serverom
		perror("socket");
		return 1;
	}
    //printf("timeout je %d\n", timeout);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    if ((setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) < 0) {    //nastavenie timeoutu pre dany socket
        perror("setsockopt");
        return 1;
    }
    //printf("here\n");
    int while_break = 0;   //pomocna premenna urcujuca, ci sa ma ukoncit nasledujuci while cyklus
    while(1) {
        if (while_break == 1)
            break;
        switch(state) {    
            case 0:       //stav konecneho autmoatu sluziaci pre odoslanie spravy zadanemu DNS serveru
                size = 12;
                if (set_type(type) == 12) {     //ak sa ma vyhladavat PTR zaznam tak prebehne reverzna transformacia zadanej vstupnej IPv4/IPv6 adresy 
                    //printf("PTR\n");
                    if (iterative_finish != 1) {
                        if ((inet_pton(AF_INET6, name, &v6_addr) == 0)) {
                            if ((inet_pton(AF_INET, name, &v4_addr) == 0)) {
                                fprintf(stderr,"Error with PTR\n");
                                return 1;
                            }
                            else {     //reverzne mapovanie IPv4 adresy
                                char ipv4_pom[100];   
                                char ipv4_reverse[100];
                                int reverse_counter = 0;
                                for (int i = strlen(name) - 1; i >= 0; i--) {
                                    counter = 0;
                                    while (name[i] != '.') {
                                        ipv4_pom[counter] = name[i];
                                        counter++;
                                        if (i == 0)
                                            break;
                                        i--;
                                    }
                                    ipv4_pom[counter] = '\0';
                                    for (int z = strlen(ipv4_pom) - 1; z >= 0; z--) {
                                        ipv4_reverse[reverse_counter] = ipv4_pom[z];
                                        reverse_counter++;
                                    }
                                    if (i != 0) {
                                        ipv4_reverse[reverse_counter++] = '.'; 
                                    }
                                    memset(&ipv4_pom, 0, sizeof(ipv4_pom));
                                }    
                                ipv4_reverse[reverse_counter] = '\0';
                                memset(&name, 0, sizeof(name));
                                strcpy(name, ipv4_reverse);
                                strcat(name, ".in-addr.arpa");
                                memset(&ipv4_reverse, 0, sizeof(ipv4_reverse));

                            }
                    }
                    else {      //reverzne mapovanie IPv6 adresy
                        //printf("Prvy je %x\n", v6_addr.s6_addr[15] & 0x0f);
                        //v6_addr.s6_addr[15] = v6_addr.s6_addr[15] & 0xf0;
                        //printf("Druhy je %x\n", v6_addr.s6_addr[15] >> 4);
                        //counter = 0;
                        memset(&name, 0, sizeof(name));
                        for (int i = 15; i >= 0;i--) {
                            name[counter] = hex_character(v6_addr.s6_addr[i] & 0x0f);
                            //printf ("v name je %c\n", name[counter]);

                            name[counter + 1] = '.';
                            v6_addr.s6_addr[i] = v6_addr.s6_addr[i] & 0xf0;
                            name[counter + 2] =  hex_character(v6_addr.s6_addr[i] >> 4);
                            //printf ("v name je %c\n", name[counter + 2]);
                            name[counter + 3] = '.';
                            counter = counter + 4;
                        }
                        name[counter] = '\0';
                        strcat(name, "ip6.arpa");
                        //printf("Finalny je %s\n", name);
                    }
                }

            }

                if (iterative_flag == 1 && iterative_start == 0) {   //ak sa jedna o iterativne vyhladavanie je potrebne najprv v novom stave upravit odosielany server
                    state = 2;
                    if (name[strlen(name) - 1] == '.') {    //odstranenie '.' na konci odosielaneho serveru
                        name[strlen(name) - 1] = '\0';
                    }
                    break;
                }
                counter = 0;
	            while (name[counter] != '\0') {	  //zistenie velkosti odosielaneho serveru

		            size++;
		            counter++;
	            }
                if (name[counter - 1] == '.') {
                    size--;
                }    
	            size = size + 5;
                
                dns_id = (unsigned short) htons(getpid());    //vygenerovaie unikatneho id odosielanej spravy
                memset(&hex_pom, 0, sizeof(hex_pom));
                hex_transform(hex_pom, (int) dns_id);
                {
                    char send_buffer[size + 1];    //vyplnenie bufferu pre odoslanie
                    send_buffer[0] = hex_pom[0];
                    send_buffer[1] = hex_pom[1];
                    send_buffer[2] = 0x01;
                    send_buffer[3] = 0x00;
                    send_buffer[4] = 0x00;
                    send_buffer[5] = 0x01;
                    send_buffer[6] = 0x00;
                    send_buffer[7] = 0x00;
                    send_buffer[8] = 0x00;
                    send_buffer[9] = 0x00;
                    send_buffer[10] = 0x00;
                    send_buffer[11] = 0x00;
                    int pom_size = 0;
                    counter = 0;
                    int buf_jump = 13;
                    int buf_len = 12;
                    //printf("here\n");
                    while(name[counter] != '\0') {     //uprava a zapisanie mena pre odoslanie do odosielaneho bufferu
                        if (name[counter] == '.' && name[counter + 1] != '\0') {
                            memset(&hex_pom, 0, sizeof(hex_pom));
                            hex_transform(hex_pom, pom_size);
                            //printf("pom size je %d\n", pom_size);
                            send_buffer[buf_len] = hex_pom[0];
                            buf_len = buf_len + pom_size + 1;
                            pom_size = 0;
                            buf_jump = buf_len + 1;
                        }
                    else if (name[counter] != '.'){
                        send_buffer[buf_jump] = name[counter];
                        pom_size++;
                        buf_jump++;
                    }
                    else {
                        name[counter] = '\0';
                    }
                    counter++;
                    }   
                    //printf("here\n");
                    memset(&hex_pom, 0, sizeof(hex_pom));
                    hex_transform(hex_pom, pom_size);
                    send_buffer[buf_len] = hex_pom[0];
                    buf_len = buf_len + pom_size + 1;
                    if (strlen(name) != 0)
                        send_buffer[buf_jump++] = 0x00;
                    send_buffer[buf_jump++] = 0x00;    
                    if ((send_buffer[buf_jump++] = set_type(type)) == 3) {    //kontrola ci je typ v odosielanom bufferi spravny
                        fprintf(stderr,"Error with wrong type\n");
                        return 2;
                    }    
                    send_buffer[buf_jump++] = 0x00;
                    send_buffer[buf_jump++] = 0x01;
                    int i = 0;
                    while (i != buf_jump) {
                       //printf("%x\n", send_buffer[i]);
                       i++;
                    }
                    if ((sendto(client_socket, send_buffer, sizeof(send_buffer), 0, (struct sockaddr *) &sa, sizeof(sa))) < 0) {   //odoslanie vyplneneho bufferu zadanemu DNS serveru
                        perror("sendto");
                        return 1;
                    }
                }
                state = 1;
                break;
            case 1:      //stav konecneho automatu pre parsovanie a vypis prijatej odpovede
                len = sizeof(sa);
                rec_index = 0;
                if ((recvfrom(client_socket, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &sa, (socklen_t *) &len)) < 0) {   //prijatie odpovede od DNS serveru
                    perror("recvfrom");
                    return 1;
                }
                rec_index = rec_index + size + 4;
                if (strlen(name) == 0)
                    rec_index = rec_index - 2;
                //printf("zasraty root je %02x %d", recv_buffer[rec_index + 10], rec_index);
                tmp_equal = 0;
                while (tmp_equal != 1) {   //cyklus pre vypis prijatych DNS zaznamov
                    if (recv_buffer[rec_index] == 0x01) {       //zistenie typu zaznamu v prijatej odpovedi
                        strcpy(tmp_type, "A");
                    }
                    else if  (recv_buffer[rec_index] == 0x02) {
                        strcpy(tmp_type, "NS");
                    }
                    else if (recv_buffer[rec_index] == 0x05) {
                        strcpy(tmp_type, "CNAME");
                    } 
                    else if (recv_buffer[rec_index] == 0x0c) {
                        strcpy(tmp_type, "PTR");
                    } 
                    else if (recv_buffer[rec_index] == 0x1c) {
                        strcpy(tmp_type, "AAAA");
                    }    
                    else {   
                        fprintf(stderr,"Error with getting answer\n");
                        //printf("Je to %x\n", recv_buffer[rec_index - 1]);
                        return 1;
                    }


                    rec_index = rec_index + 7;
                    if (recv_buffer[rec_index] != 0x00) {
                        answer_size = recv_buffer[rec_index] << 8 | recv_buffer[rec_index + 1];
                    }
                    else {
                        answer_size = recv_buffer[rec_index + 1];
                    }
                    rec_index = rec_index + 2;
                    printf ("%s. IN %s ", name, tmp_type);      //vypis casti konstrukcie zaznamu (<name> IN <Type>)
                    if (set_type(tmp_type) == set_type(type))
                        tmp_equal = 1;
                    if (set_type(tmp_type) == 1) {   //vypisovanie A zaznamu
                        for (int i = 0; i< answer_size; i++) {
                            printf("%d", recv_buffer[rec_index++]);
                            if (i + 1 == answer_size)
                                break;
                            else
                                printf(".");
                        }
                    }
                    else if (set_type(tmp_type) == 28) {    //vypisovanie AAAA zaznamu
                       unsigned char ipv6_buffer[2000];
                        int index = 0;
                        for(int i = 0; i< answer_size; i++) {
                            for (int j = 0; j < 2; j++) {
                                if (recv_buffer[rec_index] != 0x00) {
                                    ipv6_buffer[index++] = recv_buffer[rec_index];
                                }
                                rec_index++;
                            }
                            i++;
                            if (i + 1 != answer_size) {
                                if (ipv6_buffer[index - 1] == ':') {
                                    if (ipv6_buffer[index - 2] != ':') {
                                        ipv6_buffer[index++] = ':';
                                    }
                                }
                                else
                                    ipv6_buffer[index++] = ':';
                            }
                            else {
                                ipv6_buffer[index++] = '\0';
                                break;
                            }
                        }
                        index = 0;
                        while (ipv6_buffer[index] != '\0') {
                            if (ipv6_buffer[index] != ':') {
                                if (ipv6_buffer[index - 1] == ':')
                                    printf("%x", ipv6_buffer[index++]);
                                else
                                    printf("%02x", ipv6_buffer[index++]);
                            }
                            else {
                                printf("%c", ipv6_buffer[index++]);
                            }
                        }
                    }    
                    else if (set_type(tmp_type) == 5) {     //vypisovanie CNAME zaznamu
                        char cut_name[2000];
                        cut_name[0] = '\0';
                        int cut_name_index = 0;
                        int size = recv_buffer[rec_index++];
                        if (isprint(recv_buffer[rec_index])) {
                            for (cut_name_index = 0; cut_name_index < size; cut_name_index++) {
                                cut_name[cut_name_index] = recv_buffer[rec_index++];
                            }
                        }
                        else {
                            rec_index = rec_index + size;
                        }
                        rec_index = rec_index + (answer_size - size);
                        int index = 0;
                        while (name[index] != '\0') {
                            if (name[index] == '.') {
                                if (strlen(cut_name) == 0)
                                    index++;
                                while(name[index] != '\0') {

                                    cut_name[cut_name_index++] = name[index];
                                    index++;
                                }
                                cut_name[cut_name_index] = '\0';
                                break;    
                            }
                            index++;
                        }
                        strcpy(name, cut_name);
                        printf("%s.", name);


                    }  
                    else if (set_type(tmp_type) == 0x0c || set_type(tmp_type) == 2) {    //vypisovanie PTR/NS zaznamu
                        int dns_part = 0;
                        int dns_break = 0;
                        if (iterative_flag == 1) {
                            memset(&recv_iterative, 0, sizeof(recv_iterative));
                            rec_index_iterative = 0;
                        }
                        for(int i = 0; i < answer_size;i++) {
                            dns_part = recv_buffer[rec_index++];
                            for (int j = 0; j < dns_part;j++) {
                                if (set_type(tmp_type) == 2) {
                                    if (isprint(recv_buffer[rec_index])) {
                                        printf("%c", recv_buffer[rec_index++]);
                                        if (iterative_flag == 1) {
                                            recv_iterative[rec_index_iterative++] = recv_buffer[rec_index - 1];
                                        }
                                    }
                                    else {
                                        dns_break = 1;
                                        break;
                                    } 
                                }
                                else {
                                   printf("%c", recv_buffer[rec_index++]); 
                                }
                                i++;
                            }
                            if (dns_part != 0) {
                                if (set_type(tmp_type) == 2) {
                                    if (isprint(recv_buffer[rec_index + 1])) {
                                        printf(".");
                                         if (iterative_flag == 1)
                                            recv_iterative[rec_index_iterative++] = '.';
                                    }
                                }
                                else {
                                    printf(".");
                                }    
                            }
                        }
                        if (set_type(tmp_type) == 2) {
                            printf(".");
                            if (iterative_flag == 1) {
                                recv_iterative[rec_index_iterative++] = '.';
                                recv_iterative[rec_index_iterative] = '\0';
                            }
                            if (dns_break == 1) {
                                //printf("Name je %s\n", name);
                                if ((strcmp(name, "in-addr.arpa")) != 0)
                                    printf("%s.", name);
                                else 
                                    printf("arpa.");
                                if (iterative_flag == 1) {
                                    if ((strcmp(name, "in-addr.arpa")) != 0)
                                        strcat(recv_iterative, name);
                                    else 
                                        strcat(recv_iterative, "arpa");
                                }
                            }    
                        }
                        
                    }
                    rec_index = rec_index + 2;
                    printf("\n");
                }                            
                //printf ("\nPrijaty buffer je %02x\n", recv_buffer[rec_index + 1]);
                if (iterative_flag == 1 && set_type(type) == 2) {    //ak bolo zadane iterativne vyhladavanie je potrebne zistit A zaznam posledneho NS zaznamu
                    strcpy(type, "A");
                    memset(&name, 0, sizeof(name));
                    strcpy(name, recv_iterative);
                    //printf("Iterativny je %s\n", recv_iterative);
                    state = 0;
                    break;
                }
                else if (iterative_flag == 1 && iterative_finish == 0) {
                    state = 2;
                    break;
                }    
                else {
                    while_break = 1;
                    break;
                }    
            case 2:   //stav konecneho automatu pre riadenie iterativneho vyhladavania
                if (iterative_start == 0) {   //ak sa jedna o prvu iteraciu je potrebne nastavit typ na NS a meno na <root>
                    //printf("zde\n");
                    strcpy(iterative_pom, name);
                    strcpy(tmp_iterative, type);
                    memset(&type, 0, sizeof(type));
                    strcpy(type, "NS");
                    iterative_counter = strlen(name) - 1;
                    memset(&name, sizeof(name), 0);  
                    iterative_start = 1;
                    name[0] = '.';
                    name[1] = '\0';
                    state = 0;
                    break;
                }    
                else if (iterative_finish == 0) {      
                    memset(&iterative_reverse_pom, 0, sizeof(iterative_reverse_pom));
                    memset(&name, 0, sizeof(name));
                    counter = 0;
                    while(iterative_pom[iterative_counter] != '.') {      //skopriovanie casti celeho mena do pomocnej premennej
                        iterative_reverse_pom[counter] = iterative_pom[iterative_counter];
                        counter++;    
                        if (iterative_counter == 0) {
                            iterative_finish = 1;
                            break;    
                        }
                        iterative_counter--;
                    }
                    //printf("tady\n");
                    iterative_counter--;            
                    iterative_reverse_pom[counter] = '\0';
                    if (first_time_iterative != 0) {     //ak sa nejedna o prvu iteraciu, je potrebne vykonat posunutie znakov, aby sa na zaciatok mohla vlozit nova cast serveru
                        iterative_movement_counter = strlen(iterative_cat);
                        iterative_cat[iterative_movement_counter + counter + 1] = iterative_cat[iterative_movement_counter];
                        iterative_movement_counter--;
                        while(isprint(iterative_cat[iterative_movement_counter])) {
                            iterative_cat[iterative_movement_counter + counter + 1] = iterative_cat[iterative_movement_counter];
                            iterative_movement_counter--;
                        }
                        iterative_cat[iterative_movement_counter + counter] = '\0';
                        //printf("Iteracia je %s\n", iterative_cat);
                    }
                    iterative_cat_counter = 0;   //pridanie novej casti mena na zaciatok pomocneho stringu
                    for (int z = strlen(iterative_reverse_pom) - 1; z >= 0; z--) {
                        iterative_cat[iterative_cat_counter] = iterative_reverse_pom[z];
                        iterative_cat_counter++;
                    }
                    iterative_cat[iterative_cat_counter++] = '.';
                    if (first_time_iterative == 0) {  // ak sa jedna o prvu iteraciu, musime ukoncit pomocny string pomocou '\0'
                        iterative_cat[iterative_cat_counter] = '\0';
                        first_time_iterative = 1;
                    }    
                    strcpy(name, iterative_cat);
                    if (strcmp(name, "arpa.") == 0) {
                        state = 2;
                        break;
                    }
                    if (iterative_finish == 1) {
                        strcpy(type, tmp_iterative);
                    }
                    else {   
                        strcpy(type, "NS");
                    } 
                    //printf ("Iterative name je %s\n", name);

                    state = 0;
                    break;
                }
                while_break = 1;
                break;
            default:
                fprintf(stderr, "Error, unknown statement\n");
                return 1;    
                    
        }
    }
        return 0;


}	
