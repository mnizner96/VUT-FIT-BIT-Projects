/*
*	Author: Martin Nizner
*	Project: IPK Proj1 : Klient server pro vyhledani uzivatelu VUT FIT Brno
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
#define QUEUE (2)
char buffer[2000];
struct sigaction sa;
char line[2000];

char* getfield(char *line, int num)     //funkcia pre ziskanie konkretneho stlcpa v riadku
{
    char* tok;
    for (tok = strtok(line, ":");
            tok && *tok;
            tok = strtok(NULL, ":\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
int main(int argc, char **argv) {
	FILE *f;
	int port = 0;
	
	int port_flag = 0;    //pomocne premenne pre informaciu, ze bol zadany prepinac
	int whole_name_flag = 0;
	int directory_flag = 0;
	int print_all_flag = 0;
	int print_prefix_flag = 0;
	int p_flag = 0;
	
	int listenfd;      //deklaracia socketov
	int connectfd;
	
	struct sockaddr_in6 server;   //struktura pre nastavenie serveru
	struct hostent *hostent;
	struct servent *servent;
	
	pid_t pid;
	long p;
	int msg_size = 0;
	int w_size = 0;
	int file;
	int state = 0;
	struct stat file_stat;
	char *send_name;
	char name_to_find[2000];
	
	int c; //pomocna premenna pre vyhodnotenie getopt()
	
	while ((c = getopt(argc, argv, "p:")) != -1) {       //parsovanie vstupnych argumentov
		switch(c) {
			case 'p':
				port = atoi(optarg);
				p_flag = 1;
				break;
			default:
				fprintf(stderr, "Error, wrong type of argument\n");
				return 1;
		}
	}
	
	if (p_flag == 0) {             //kontrola, ci bol zadany povinny parameter -p <port>
		fprintf(stderr,"Error with arguments\n");
		return 1;
	}
	
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		err(1, "sigaction()");
	
	memset(&server, 0, sizeof(server));             
	server.sin6_family = AF_INET6;    //nastavovanie serveru(IPv6, cislo portu)
	server.sin6_port = htons(port);
	server.sin6_addr = in6addr_any;
	if((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) == -1) {         //vytovrenie socketu pre zachytavanie poziadavkov
		perror("socket");
		return 1;
	}
	if (bind(listenfd, (struct sockaddr *) &server, sizeof(server)) == -1) {      
		perror("bind");
		return 1;
	}

	if (listen(listenfd, QUEUE) == -1) {         //zachytavanie poziadavkov od klienta
		perror("listen");
		return 1;
	}

	while (1) {
		if ((connectfd = accept(listenfd, NULL, NULL)) == -1) {  //ustavenie spojenia s klientom
			perror("accept");
			return -1;
		}
		if ((pid = fork()) > 0) {   //vytvorenie noveho procesu serveru pre obsluzenie daneho klienta
			close(connectfd);
		}
		else if (pid == 0) {
			p = (long) getpid();
			close(listenfd);     //uzavretie socketu pre ustavenie spojenia a zapocatie prijmania sprav od klienta
			memset(&buffer, 0, sizeof(buffer));
			while((msg_size = read(connectfd, buffer, sizeof(buffer) - 1)) > 0) {   //prijatie 1 spravy od klienta
				switch(state) {
					case 0:   //state pre zistenie informacie o tom, aka sprava bola prijata
						if ((strcmp(buffer, "SearchWholeNameEntry")) == 0) {        
							whole_name_flag = 1; 
						}	
						else if((strcmp(buffer, "SearchDirectoryEntry")) == 0) {
							directory_flag = 1;
						}
						else if((strcmp(buffer, "AllUsersEntry")) == 0) {
							print_all_flag = 1;
						}
						else if ((strcmp(buffer, "SearchPrefixEntry")) == 0) {
							print_prefix_flag = 1;
						}		
						state = 1;
						memset(&buffer, 0, sizeof(buffer));
						break;
					case 1:   //state pre vyfiltrovanie udajov zo suboru na zaklade prijatej spravy
						strcpy(name_to_find, buffer);
						//printf("1\n");
						if ((f = fopen("/etc/passwd","r")) == NULL) {     //otvorenie pozadovaneho suboru
							fprintf(stderr, "Error with opening file\n");
							return 1;
						}
						while(fgets(line, 1024, f)) {  //nacitanie 1 riadku suboru
							memset(&buffer, 0, sizeof(buffer));
							char* tmp = strdup(line);
							send_name = getfield(tmp, 1);
							if ((strcmp(send_name, name_to_find)) == 0 && print_all_flag == 0) {
								tmp = strdup(line);
								if (whole_name_flag == 1) {    //filtrovanie z daneho riadku na zaklade nastaveneho prepinaca s predchadzaujuceho stavu
									send_name = getfield(tmp, 5);
								}
								else if (directory_flag == 1) {
									send_name = getfield(tmp, 6);
								}
								strcpy(buffer, send_name);    //odoslanie odpovedi spat klientovi
								//printf("sending\n");
								if ((w_size=write(connectfd,buffer, sizeof(buffer) - 1)) == -1) {
									perror("error write\n");
									exit(1);
								}
							}
							else if (print_all_flag == 1){
								strcpy(buffer, send_name);
								if ((w_size=write(connectfd,buffer, sizeof(buffer) - 1)) == -1) {
									perror("error write\n");
									exit(1);
								}
							}
							else if (print_prefix_flag == 1) {
								if (send_name[0] == name_to_find[0]) {
									int i = 1;
									int match = 1;
									while (name_to_find[i] != '\0') {
										if (name_to_find[i] != send_name[i])
											match = 0;
										i++;
									}
									if (match == 1) {
										strcpy(buffer, send_name);
										if ((w_size=write(connectfd,buffer, sizeof(buffer) - 1)) == -1) {
											perror("error write\n");
											exit(1);
										}
									}
								}
							}
						}
						memset(&buffer, 0, sizeof(buffer));
						strcpy(buffer,"SearchResDone");   //odoslanie informacie o ukonceni vyhladavania spat klientovi
						if ((w_size=write(connectfd,buffer, sizeof(buffer) - 1)) == -1) {
							perror("error write\n");
							exit(1);
						}
						state = 2;
						break;
					case 2: //stav pre uzavretie spojenia s klientom
						if ((strcmp(buffer, "CloseConnection")) == 0) {
							close(connectfd);
							exit(0);
						}
					default:
						fprintf(stderr,"Error wrong type of message\n");
						exit(1);		
				}

			}
			close(connectfd);    //ukoncenie procesu pre obsluhu daneho klienta
			exit(0);
		}
		else {
      		err(1, "fork() failed\n");
    	} 
	}
	close(listenfd);
	//printf("Server\n");
	exit(0);
	return 0;
}
