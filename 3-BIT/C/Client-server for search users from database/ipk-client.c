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
int main(int argc, char **argv) {
	char host[2000]; //premenna pre ulozenie IP/DNS adresy serveru
	char buffer[2000]; //premenna pre posielanie sprav medzi serverom a klientom
	char login[2000];  //premenna pre ulozenie loginu , ktory ma server vyhladat
	
	struct sockaddr_in sa; //struktura pre IPv4 adresu servera
	struct sockaddr_in6 sa6;  //strukuta pre IPv6 adresu servera
	struct hostent *he;
	struct in_addr **addr_list;  //struktury pre IPv4 DNS name serveru
	struct addrinfo hints, *servinfo, *p; //struktury pre IPv6 DNS name serveru 
	
	int i;  //pomocna premenna pre cykly for
	
	int ipv4flag = 0;   //pomocne premenne pre informaciu o tom aka bola zadana IP adresa (IPv4, IPv6)
	int ipv6flag = 0;
	
	int h_flag = 0;    //pomocne premenne pre spracovanie argumentov
	int p_flag = 0;
	int whole_name = 0;
	int directory = 0;
	int userID = 0;
	int login_flag = 0;
	
	int connectfd;   //deklaracia socketu pre komunikaciu so serverom

	int found = 0;
	int port = 0;
	int w_size = 0;
	int msg_size = 0;
	int addr_info;
	
	int c;   //pomocna premenna pre vyhodnotenie getopt()
	servinfo = NULL;
	while ((c = getopt(argc, argv, "h:p:n:f:l")) != -1) {
		switch(c) {
			case 'h':
				strcpy(host, optarg);
				h_flag = 1;
				break;
			case 'p':
				port = atoi(optarg);
				p_flag = 1;
				break;
			case 'n':
				strcpy(login, optarg);
				login_flag = 1;
				whole_name = 1;
				break;
			case 'f':
				strcpy(login, optarg);
				login_flag = 1;
				directory = 1;
				break;
			case 'l':
				if (argc == 7) {
					strcpy(login, argv[6]);
					userID = 2;
				}
				else {
					strcpy(login,"*");
					//printf("L je %s\n", login);
					userID = 1;
				}
				login_flag = 1;
				break;

			default:
				fprintf(stderr, "Error, wrong type of argument\n");
				return 1;
		}
	}
	if (login_flag == 0 || p_flag == 0 || h_flag == 0) {  //kontrola ci boli zadane vsetky povinne parametre
		fprintf(stderr, "Error, missing arguments\n");
		return 1;
	}
	else if (argc != 7 && argc != 6) {        //kontrola poctu parametrov
		fprintf(stderr,"Error with arguments\n");
		return 1;
	}
	if ((inet_pton(AF_INET, host, &(sa.sin_addr))) == 0) {  //kontrola ci je adresa serveru v tvare IPv4
		if ((inet_pton(AF_INET6, host, &(sa6.sin6_addr))) == 0) {   //kontrola ci je adresa serveru v tvare IPv6
			//printf("DNS\n");
			if ((he = gethostbyname(host)) == NULL) {  //kontrola ci je adresa serveru v tvare DNS pre IPv4
					memset(&hints, 0, sizeof hints);
					hints.ai_family = AF_INET6; 
					hints.ai_socktype = SOCK_STREAM;
					char port_str[20];
					sprintf(port_str, "%d", port);
					if ((addr_info = getaddrinfo(host, port_str, &hints, &servinfo)) != 0) {   //kontrola ci je adresa serveru v tvare DNS pre IPv6
    					fprintf(stderr, "Wrong Host Name\n");
    					return 1;
					}
					ipv6flag = 1;


			} 
			else {
				addr_list = (struct in_addr **) he->h_addr_list;  //preklad domenovej adresz na IPv4 adresu
				strcpy(host, inet_ntoa(*addr_list[0]));
				if ((inet_pton(AF_INET, host, &(sa.sin_addr))) == 0) {   
					fprintf(stderr,"Error this host isnt exists\n");
					return 1;	
				}
				ipv4flag = 1;
		  	}
		}
		else {
			ipv6flag = 1;
		}
	}
	else {
		ipv4flag = 1;
		//printf("IPV4\n");
	}
	if (ipv4flag == 1) {    //nastavenie parametrov socketu pre IPv4
		//printf("start\n");
		sa.sin_family = AF_INET;
		sa.sin_port = htons(port);
		memset(sa.sin_zero, '\0', sizeof(sa.sin_zero));
		if ((connectfd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {   //vytvorenie socketu pre IPv4
			perror("ERROR: socket");
			return 1;
		}
		if (connect(connectfd, (struct sockaddr *) &sa, sizeof(sa)) != 0) {   //naviazanie spojenia so serverom
			perror("connect");
			return 1;
			
		}

	}
	else if (ipv6flag == 1) {  //nastavenie parametrov socketu pre IPv4
		sa6.sin6_family = AF_INET6;
		sa6.sin6_port = htons(port);
		if ((connectfd = socket(AF_INET6, SOCK_STREAM, 0)) <= 0) {    //naviazanie spojena so serverom v pripade ze adresa bola vo formate IPv4
			perror("ERROR: socket");
			return 1;
		}

		if (servinfo != NULL) {
			int connect_check = 0;
			for(p = servinfo; p != NULL; p = p->ai_next) {  //naviazanie spojenia so serverom v pripade ze adresa bola vo formate DNS name pre IPv6
				if (connect(connectfd, p->ai_addr, p->ai_addrlen) == -1) {
        			perror("connect");
        			close(connectfd);
        			continue;
    			}
    			connect_check = 1;
    			break;
			}
			if (connect_check == 0)
				return 1;
		}
		else {
			if (connect(connectfd, (struct sockaddr *) &sa6, sizeof(sa6)) == -1) {  //naviazanie spojena so serverom v pripade ze adresa bola vo formate IPv6
				perror("connect");
				return 1;
			}	

		}
	}
	memset(&buffer, 0, sizeof(buffer));   
    if (whole_name == 1) {
    	strcpy(buffer, "SearchWholeNameEntry");
    }
    else if (directory == 1) {
    	//printf("directory\n");
    	strcpy(buffer, "SearchDirectoryEntry");
    }
    else if (userID == 1) {
    	strcpy(buffer, "AllUsersEntry");
    }
    else if (userID == 2) {
    	//printf("prefix\n");
    	strcpy(buffer, "SearchPrefixEntry");
    }
    else {
    	fprintf(stderr, "Wrong message type\n");
    	return 1;
    }
    if ((w_size = write(connectfd, buffer, sizeof(buffer) - 1)) == -1) { //odosielanie uvodnej spravy serveru na zaklade vstupych argumentov
    		perror("error write");
    		return 1;
    }
    memset(&buffer, 0, sizeof(buffer));
    strcpy(buffer, login);
   	if ((w_size = write(connectfd, buffer, sizeof(buffer) - 1)) == -1) { //odosielanie loginu, ktoreho sa uvodna sprava tyka
    	perror("error write");
    	return 1;
    }
  

    while((msg_size = read(connectfd, buffer, sizeof(buffer) - 1)) > 0) {   
    	//printf("tu\n");
    	if ((strcmp(buffer, "SearchResDone")) == 0) {
    		memset(&buffer, 0, sizeof(buffer));
    		strcpy(buffer, "CloseConnection");   //odoslanie poziadavku na ukoncenie spojenia v pripade ze bolo ukoncene vyhladavanie
    		if ((w_size = write(connectfd, buffer, sizeof(buffer))) == -1) {
    			perror("error write");
    			return 1;
   			}
   			if (found == 0) {
   				fprintf(stderr, "No such entries\n");
   				return 1;
   			}
   			break; 
    	}
    	else {
    		found = 1;
    		printf("%s\n", buffer);  //vypisanie prijatej informacie od serveru na standartny vystup
    		memset(&buffer, 0, sizeof(buffer));
    	}
    } 


    	
	close(connectfd);  //uzavretie socketu pre komunikaciu so serverom
	return 0;
}
