#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <err.h>
#include <stdbool.h>
#include <fcntl.h>
#include <wchar.h>
#include <locale.h>
#define QUEUE (2)

struct sockaddr_in s_addr;    //deklaracia globalnych premennych
struct sockaddr_in r_addr;
socklen_t r_len;
pid_t pid;
long p;
struct sigaction sa;
int msg_size;
int w_size;
int counter;
int while_parser_end = 0;     //indikator ukoncenia cyklu pre spracovanie spravy
int state = 0;                //pocitatocny stav automatu pre spracovanie spravy
int basic_flag = 0;
int scope_flag = 0;          
int single_l_flag = 0;
int send_flag = 0;
int size_set = 0;
int size_exceeded = 0;
unsigned int size = 0;
bool types = false;   //indikuje, ci sprava obsahovala types
unsigned char size_buf[2048];    
unsigned int  words_number = 0;   //uchovava pocet doteraz odoslanych searchResEntrỵ()
unsigned int tmp_d;   //pomocna premenna pre uchovavanie parametru LL
unsigned int time_limit = 0;   //pomocna premenna pre uchovanie time_limit ak bol zadany
unsigned char filter[10];     //pomocna premenna pre uchovanie typu filtra(uid, cn, mail)
unsigned char name_to_filter[2048];   //pomocna premenna pre uchovanie hodnoty filtru
char line[1024];    //uklada jeden cely riadok zo skumaneho suboru
char *send_uid;     //uklada uid hodnotu, ktora sa ma odoslat
char *send_cn;      //uklada cn hodnotu, ktora sa ma odoslat
char *send_mail;    //uklada mail hodnotu, ktora sa ma odoslat
int attributes_size = 0;     //velkost atributov prijatej SearchRequest spravy
int mail_size = 0;           //velkost mail hodnoty
int cn_size = 0;             //velkost cn hodnoty
int uid_size = 0;            //velkost uid hodnoty
int send_response_size = 0;  //celkova velkost odosielanej spravy
int search_response_size = 0; //celkova velkost protokolu searchResEntry() 
int number_of_subnames = 0;   //pocet podvyrazov pri filtri Substrings (napr *a*b* ma podvyrazy a,b)
int size_of_subname = 0;      //velkost jedneho podvyrazu pri filtri Substrings
int i = 0;                   //index pre znak v prijatom bufferi
char whole_length[2048];     //celkova velkost odosielanej spravy
char cn_length[2048];        //velkost parametru cn odosielanej spravy
char mail_length[2048];      //velkost parametru mail odosielanej spravy
char search_length[2048];    //celkova velkost protokolu searchResEntry()
char attributes_length[2048]; //celkova velkost odosielanych atributov
int substr_array_filter[100]; //pomocne pole pre typ podvyrazov u substrings (0- initial, 1- any, 2- final)
int logical_analysis(unsigned char *buffer, char *line, unsigned int length, int logical_flag); //prototyp funkcie pre analyzu logickych vyrazov 
void hex_transform(char *hex_tmp, int num);  //prorotyp funkcie pre transformaciu int na hexadecimalne cislo vo formate char  
                                             //tieto prorotypy su potrebne s dovodu pouzitia danych funkcii v tele inych funkcii


int substring_analyze (unsigned char *buffer, int i, int counter) {   //funkcia pre parsovanie substringov
  int j = 0;
  if (buffer[i] == 0x80){  //podvyraz je typu initial;
    i = i + 1;
    substr_array_filter[counter] = 0;    //zapis hodnoty initial do pomocneho pola pre podvyrazy
    if (buffer[i] != 0x00) {
      tmp_d = (unsigned int) buffer[i++];
      while (j< tmp_d) {                 //nacitanie hodnoty podvyrazu do premennej pre ukladanie hodnoty filtru
        name_to_filter[size_of_subname] = buffer[i++];   
        size_of_subname++;
        j++;
      }
      name_to_filter[size_of_subname] = 0x00;   //oznacenie konca podvyrazu znakom 0x00 resp '\0'
      size_of_subname = size_of_subname + 1;
      number_of_subnames = number_of_subnames + 1;
    }
    return i;   //vratenie indexu bufferu z prijatou spravou, kde funkcia skoncila
  }
  else if (buffer[i] == 0x81) {  //podvyraz pre typ any
    i = i + 1;
    substr_array_filter[counter] = 1;  //zapis hodnoty any do pomocneho pola pre podvyrazy
    if (buffer[i] != 0x00) {
      tmp_d = (unsigned int) buffer[i++];
      while (j< tmp_d) {               //rovnaky sposob ako pri inital
        name_to_filter[size_of_subname] = buffer[i++];  
        size_of_subname++;
        j++;
      }
      name_to_filter[size_of_subname] = 0x00;
      size_of_subname = size_of_subname + 1;
      number_of_subnames = number_of_subnames + 1;
    }
    return i;
  }
  else if (buffer[i] == 0x82) {      //podvyraz je typu final
    i = i + 1;
     substr_array_filter[counter] = 2;    //zapis hodnoty final do pomocneho pola pre podvyrazy
     if (buffer[i] != 0x00) {   
      tmp_d = (unsigned int) buffer[i++];
      while (j< tmp_d) {      //rovnaky sposob ako pri initial
        name_to_filter[size_of_subname] = buffer[i++];
        size_of_subname++;
        j++;
      }
      name_to_filter[size_of_subname] = 0x00;
      size_of_subname = size_of_subname + 1;
      number_of_subnames = number_of_subnames + 1;
     }
    return i;
  }
  else {
    return -1;
  }
}  
char* getfield(char *line, int num)     //funkcia pre ziskanie konkretneho stlcpa v riadku
{
    char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
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

void search_response(char *line, int newsock) {   //funkcia pre generovanie searchResEntry()
  char *tmp;
  tmp = strdup(line);     
  memset(&send_cn, 0, sizeof(send_cn));
  send_cn = getfield(tmp, 1);   //nacitanie cn hodnoty s tmp
  tmp = strdup(line);
  memset(&send_mail, 0, sizeof(send_mail));
  send_mail = getfield(tmp, 3); //nacitanie mail hodnoty s tmp
  if ((!(isalpha(send_mail[strlen(send_mail) - 1])))) {
    send_mail[strlen(send_mail) -1] = '\0';
  }
  cn_size = strlen(send_cn); //ziskanie velkosti hodnoty cn
  memset(&cn_length, 0, sizeof(cn_length));
  hex_transform(cn_length, cn_size); //transformacia velkosti do hexadecimalnej char hodnoty
  if (strlen(cn_length) > 1) 
    send_response_size = send_response_size + strlen(cn_length) - 1;
  mail_size = strlen(send_mail);   //ziskanie velosti hodnoty mail
  memset(&mail_length, 0, sizeof(mail_length));
  hex_transform(mail_length, mail_size);   
  if (strlen(mail_length) > 1)
    send_response_size  = send_response_size + strlen(mail_length) - 1;   
  send_response_size = 25 + mail_size + cn_size + 2 + 4;   //finalne nastavenie celkovej velkosti odosielanej spravy
  search_response_size = 20 + mail_size + cn_size + 2 + 4; //finalne nastavenie celkovej velosti protokolu searchResEntry
  attributes_size = 16 + mail_size + cn_size + 2 + 4;      //finalne nastavenie celkovej velkosti atributov
  memset(&whole_length, 0, sizeof(whole_length));
  hex_transform(whole_length, send_response_size);  
  memset(&search_length, 0, sizeof(search_length));
  hex_transform(search_length, search_response_size);
  memset(&attributes_length, 0, sizeof(attributes_length));
  hex_transform(attributes_length, attributes_size);
  int size = send_response_size + 2; 
  int i = 0;
  char buffer[size + 1];   //nastavenie celkovej velkosti odosielaneho bufferu
  buffer[i++] = 0x30;      //vyplnanie odosielaneho bufferu jednotlivymi hexadecimalnymi hodnotami po 1 znaku
  buffer[i++] = whole_length[0];
  buffer[i++] = 0x02;
  buffer[i++] = 0x01;
  buffer[i++] = 0x02;
  buffer[i++] = 0x64;
  buffer[i++] = search_length[0];
  buffer[i++] = 0x04;
  buffer[i++] = 0x00;
  buffer[i++] = 0x30;
  buffer[i++] = attributes_length[0];
  buffer[i++] = 0x30;
  buffer[i++] = 0x08 + cn_length[0];
  buffer[i++] = 0x04;
  buffer[i++] = 0x02;
  buffer[i++] = 'c';
  buffer[i++] = 'n';
  buffer[i++] = 0x31;
  buffer[i++] = cn_length[0] + 0x02;
  buffer[i++] = 0x04;
  buffer[i++] = cn_length[0];
  int j = 0;
  while (cn_size != 0) {
    buffer[i] = send_cn[j];
    i++;
    j++;
    cn_size = cn_size - 1;
  }
  buffer[i++] = 0x30;
  buffer[i++] = mail_length[0] + 0x0a;
  buffer[i++] = 0x04;
  buffer[i++] = 0x04;
  buffer[i++] = 'm';
  buffer[i++] = 'a';
  buffer[i++] = 'i';
  buffer[i++] = 'l';
  buffer[i++] = 0x31;
  buffer[i++] = mail_length[0] + 0x02;
  buffer[i++] = 0x04;
  buffer[i++] = mail_length[0];
  j = 0;
  while (mail_size != 0) {
    buffer[i] = send_mail[j];
    i++;
    j++;
    mail_size = mail_size - 1;
  }
  if ( (w_size = write(newsock, buffer, sizeof(buffer) -1)) == -1) {    //odoslanie vyplneneho bufferu
    perror("erorr wirte\n");
  }
}
int substr_control(char *send_uid) {    //funkcia pre analyzu, ci hodnota riadku v subore vyhovuje zadanemu substring filtru
  int substr_counter = 0;      //pomocna hodnota pre indexovanie  jedneho znaku v premennej name_to_filter
  int match = 0;        //hodnota urcujuca, ci hodnota riadku send_uid vyhovuje podvyrazu v premennej name_to_filter
  int match_count = 0;  //hodnota urcujuca kolko substringov v send_uid sa zhoduje s podvyrazom v premennej name_to_filter
  int match_position = -1;    //hodnota urcujuca index v send_uid, na ktorom zacina zhoda s podvyrazom
  for (int j = 0; j< number_of_subnames;j++) {       //cyklus ktory kontroluje zhodu send_uid zo vsetkymi podvyrazmi v premennej name_to_filter
    match = 0;   //pred kazdou kontrolou noveho podvyrazu sa musi vynulovat premenna match
    int substr_len = 0;  //dlzka jedneho podvyrazu
    int char_counter = 0; //index znaku v premennej send_uid
    int tmp_counter = 0;  //index znaku v premennej tmp
    int match_found = 0;  //priznak urcujuci ze send_uid urcite vyhovuje podvyrazu v premennej name_to_filter  
    int number_of_matches = 0; //pocet najdenych zhod send_uid s jednym podvyrazom
    char tmp[100];   //pomocny string pre jeden podvyraz
    while(name_to_filter[substr_counter] != 0x00) { //nacitanie jedneho podvyrazu s name_to_filter do tmp
      tmp[tmp_counter++] = name_to_filter[substr_counter];
      substr_counter++;
    }
    substr_counter++;  
    tmp[tmp_counter] = '\0';
    while(send_uid[char_counter] != '\0') {   //prechadzanie send_uid a hladanie zhody s aktualnym podvyrazom v tmp
      int increment_flag = 1;
      int found_char_position = -1;
      if (send_uid[char_counter] == tmp[0]) {   //ak sa najde zhoda znaku send_uid s prvym znakom tmp tak sa prevedie kontrola zhody zvysnych znakov
        increment_flag = 0;
        match = 1;   //pociatocne nastavenie priznaku zhody na 1
        tmp_counter = 0;
        found_char_position = char_counter;
        while(tmp[tmp_counter] != '\0') {
          if (send_uid[char_counter] != tmp[tmp_counter]) {    //ak niektory zo znakov send_uid nevyhovuje vyzadovanemu znaku s tmp tak sa priznak match zmeni na 0
            match = 0;
            break;
          }
          char_counter++;
          substr_len++;
          tmp_counter++;
        }
        if (match != 0) {
          match_found = 1;   //nastavenie urcitej zhody na 1
          if (match_position != -1) {  // ak sa najde dalsia zhoda, tak sa uvazuje ta ktora bola nacitana skor(match_found sa zmeni na 0)
            if (match_position > found_char_position) {
              match_found = 0;
            }
          }
          if (match_found != 0) {
            number_of_matches++;   //navysenie poctu najdenych zhod
            if (number_of_matches == 1) {    //ulozenie pozicie prvej najdenej zhody 
              match_position = found_char_position;
            }
          }
        }
        if (substr_array_filter[j] == 0) {    //kontrola ci zhoda vyhovuje priznaku initial ak bol zadany
          if (send_uid[0] != tmp[0])
            match_found = 0;
        }
        if (substr_array_filter[j] == 2) { //kontrola ci zhoda vyhovuje priznaku final ak bol zadany
          if (send_uid[strlen(send_uid) - 1] != tmp[tmp_counter - 1]) {
            match_found = 0;
          }
        }
      }
      if (increment_flag == 1) {     
        char_counter++;
      }
    }
    if (match_found != 0) {   //ak send_uid vyhovuje podvyrazu tak sa navysi hodnota celkovych najdenych zhod
      match_count++;
    } 
  }
  if (match_count == number_of_subnames) {    //ak sa hodnota celkvych najdenych zhod zhoduje s poctom podvyrazov tak send_uid vyhovuje zadanemu substringu
    return 1;
  }
  else
    return 0; 
}
int logical_analysis(unsigned char *buffer, char *line, unsigned int length, int logical_flag) {   //funkcia pre analyzu logickych vyrazov
   int pom_array[100];            //pomocne pole pre vyhodnotenie jednotlivych podmienok v jednom logickom vyraze
   int pom_array_counter = 0;     //pomocna hodnota pre pocet hodnot v poli pom_array
   int ret = 1;                   //vysledna hodnota jedneho logickeho vyrazu (1-true 0-false)
   int send_flag = -1;            // hodnota pre urcenie ktory stlpec v jednom riadku sa ma kontrolovat
   int pom_flag = -1;             //pomocna hodnota ktora sa preda ako parameter pri rekurzivnom volani funkcie logical_analysis
   int or_flag = 0;               //pomocna hodnota pre vyhodnocovanie logickeho vyrazu or
   int substr_counter = 0;        //pomocna hodnota pre indexovanie  jedneho znaku v premennej name_to_filter
   unsigned int length_counter = 0;    //pomocna hodnota pre pocitanie dlzky logickeho vyrazu
   while (1) {
    memset(&filter, 0, sizeof(filter));
    memset(&name_to_filter, 0, sizeof(name_to_filter));
    if (!(length_counter < length))      //ak je aktualna spocitana dlzka logickeho vyrazu rovna jeho skutocnej dlzke tak analyza konci
      break;
    if (buffer[i] == 0xa3) {            //analyza equalityMatch v logickom vyraze
      i = i + 2;
      length_counter = length_counter + 2;
      if (buffer[i++] != 0x04) {
        fprintf(stderr,"Error search 13 this isnt LDAP message\n");
        exit(1);
      }
      length_counter++;
      if (buffer[i] != 0x00) {
        tmp_d = (unsigned int) buffer[i++];
        length_counter = length_counter + tmp_d;
        for (int j = 0; j<tmp_d;j++) {
          filter[j] = buffer[i++];
        }
        length_counter++;
        if (buffer[i++] != 0x04) {
          fprintf(stderr,"Error search 14 this isnt LDAP message\n");
          exit(1);
        }
        length_counter++;
        if (buffer[i] != 0x00) {
          tmp_d = (unsigned int) buffer[i++];
          length_counter = length_counter + tmp_d;
          for (int j = 0; j< tmp_d;j++) {
            name_to_filter[j] = buffer[i++];
          }
        }
        length_counter++;
      }
      for(int i = 0; filter[i]; i++){
        filter[i] = tolower(filter[i]);
      }
      for(int i = 0; name_to_filter[i]; i++){
        name_to_filter[i] = tolower(name_to_filter[i]);
      }
      if (strcmp((char*) &filter,"uid") == 0) {
        send_flag = 2;
      }
      else if (strcmp((char*) &filter,"mail") == 0) {
        send_flag = 3;
      }
      else if (strcmp((char*) &filter,"cn") == 0) {
        send_flag = 1;
      } 
      char* tmp = strdup(line);
      send_uid = getfield(tmp, send_flag);
      for(int i = 0; send_uid[i]; i++){
        send_uid[i] = tolower(send_uid[i]);
      }
      if (send_flag == 3 && (!(isalpha(send_uid[strlen(send_uid) - 1])))) {
        send_uid[strlen(send_uid) -1] = '\0';
      }
      if ((strcmp(send_uid, (char *) &name_to_filter)) == 0) { 
        pom_array[pom_array_counter] = 1;    //zapis do pomocneho pola vysledkov pom_array[] podla vyhodnotenia equality match
      }
      else
        pom_array[pom_array_counter] = 0;
      pom_array_counter++; 
      free(tmp);
    } 
    
    else if (buffer[i] == 0xa4) {           //analyza substrings v logickom vyraze
      size_of_subname = 0;                 
      number_of_subnames = 0;
      i = i + 3;
      length_counter = length_counter + 3;
      if (buffer[i] != 0x00) {
        tmp_d = (unsigned int) buffer[i++];
        for (int j = 0; j<tmp_d;j++) {
          filter[j] = buffer[i++];
        }
        length_counter = length_counter + tmp_d;
      }
      length_counter++;
      i = i + 2;
      length_counter = length_counter + 2;
      int counter = 0;
      int j = 0;
      int previous = 0;
      int sub = 0;
      while ((j = substring_analyze(buffer,i, counter++)) != -1) {   //nacitanie jednotlivych podvyrazov substringu do name_to_filter     
        previous = i;
        sub = j - previous ;
        length_counter = length_counter + sub;
        i = j;
      }
      for (int j = 0; j< number_of_subnames;j++) {
        while(name_to_filter[substr_counter] != 0x00) {
          name_to_filter[substr_counter] = tolower(name_to_filter[substr_counter]);
          substr_counter++;
        }
        substr_counter++;
      }  
      substr_counter = 0;    
      if (strcmp((char*) &filter,"uid") == 0) 
        send_flag = 2;
      else if (strcmp((char*) &filter, "cn") == 0)
        send_flag = 1;
      else if (strcmp((char*) &filter, "mail") == 0)
        send_flag = 3;
      char *tmp = strdup(line);
      send_uid = getfield(tmp, send_flag);    //ziskanie hodnoty pre porovnanie s name_to_filter
      for(int i = 0; send_uid[i]; i++){
        send_uid[i] = tolower(send_uid[i]);
      }
      if (send_flag == 3 && (!(isalpha(send_uid[strlen(send_uid) - 1])))) {
        send_uid[strlen(send_uid) -1] = '\0';
      }
      if (substr_control(send_uid)) {        //zapis do pomocneho pola  vysledkov pom_array[] na zaklade vyhodnotenia substrings
        pom_array[pom_array_counter] = 1;
      } 
      else {
        pom_array[pom_array_counter] = 0;
      }
      pom_array_counter++; 
    }
    
    else if (length_counter<length) {        //analyza vnorenych logickych vyrazov
      int pom_flag = -1;
      if (buffer[i] == 0xa0) {     //nastavenie pomocnehj premennej , ktora sa preda ako parameter do rekurzivneho volania logickej funkcie
        pom_flag = 0;
      }  
      else if (buffer[i] == 0xa1)
        pom_flag = 1;
      else if (buffer[i] == 0xa2)
        pom_flag = 2;
      else
        break;      
      i = i + 1;
      unsigned int pom_length = (unsigned int) buffer[i++];
      length_counter = length_counter + 2;
      int res = logical_analysis(buffer, line, pom_length, pom_flag);    //ziskanie a zapis vysledku vnoreneho logickeho vyrazu do pomocmeho pola vysledkov pom_array[] 
      length_counter = length_counter + pom_length;      
      if (res == 1) {
        pom_array[pom_array_counter] = 1;
      }
      else {
        pom_array[pom_array_counter] = 0;
      }
      pom_array_counter++;
    } 
    else
      break;
  }  
  for (int a = 0; a<pom_array_counter;a++) {    //vyhodnotenie celeho pomocneho pola vysledkov na zaklade logickeho flagu(and - 0, or - 1, not - 2) a vratenie navratovej hodnoty
    if (logical_flag == 0) {
      if (pom_array[a] != 1) {
        ret = 0;
      }
    }
    else if (logical_flag == 1) {
      if (pom_array[a] == 1)
        or_flag = 1;
    }
    else if (logical_flag == 2) {
      if (pom_array[a] == 1)
      ret = 0;
    }

  }
if (logical_flag == 1 && or_flag != 1)
  ret = 0;
return ret;
}
int main(int argc, char **argv){    //hlavne telo programu
  FILE *f;
  int flags;
  int help_flag = 0;
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGCHLD, &sa, NULL) == -1) 
		err(1, "sigaction()");

	char path[2048];
	unsigned char buffer[2048];   //buffer pre prijmanie a odosielanie sprav
  int s;                        //socket serveru
	int newsock;                  //socket pre prijmanie sprav medzi klientom a serverom
	int port = 389;               
	char port_number[1000];
  int c;
  int file_flag = 0;
  while ((c = getopt(argc, argv, "p:f:h")) != -1) {   //nacitanie a osetrovanie argumentov
		switch(c) {
		  case 'p':
		    port = atoi(optarg);	
		    break;
		  case 'f':
		    strcpy(path,optarg);
		    file_flag = 1;
		    break;
      case 'h':
        help_flag = 1;
        break;
		  case '?':
		    fprintf(stderr,"Nespravny argument\n");
		    return 1;
		  default:
			  abort();
		}

  }
	memset(&s_addr, 0, sizeof(s_addr));     
	s_addr.sin_family = AF_INET;    //nastavenie struktury s_addr
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (file_flag == 0 && help_flag == 0) {         //osetrovacie podmienky pre vstupne argumentu
    	fprintf(stderr,"Error nebola zadana cesta\n");
    	return 1;
    }
	if (argc > 5 || (argc == 1) || (argc == 2 && (help_flag == 0 && file_flag == 0))) {
    fprintf (stderr, "Nespravne zadane argumenty\n");
    return 1;
  } 
  if (help_flag == 1) {     //vypis napovedy
    printf("Napoveda pre LDAP server\n");
    printf("Tento program pracuje s tymito parametrami\n");
    printf("-h                  zobrazi napovedu\n");
    printf("-p <port>           zmeni implicitne nastaveny port serveru (389) na zadany port\n");
    printf("-f <source_file>    povinny parameter pre  urcenie suboru, ktory bude server prehladavat");
    return 0;
  }



	if ((s = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
		fprintf(stderr,"Error while creating socket\n");
		perror("Error\n");
		return 1;
	}
  if ((flags = fcntl(s,F_GETFL,0)) < 0)    //nastavenie socketu ako neblokujuci
    flags |= O_NONBLOCK;
  if (fcntl(s, F_SETFL, flags) < 0) {
    fprintf(stderr,"error non block\n");
    exit(1);
  }
	if (bind(s, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {     
		perror("Bind");
		fprintf(stderr,"Error binding\n");
		return 1;
	}
	if (listen(s, QUEUE) == -1)
		err(1, "listen()");
	while(1) {
       if ((newsock = accept(s, (struct sockaddr *) &r_addr, (socklen_t*) &r_len)) == -1) { 
       	err (1,"accept() failed");
       }
       if ((pid = fork()) > 0) {
       	p = (long) getpid();
       	close(newsock);
       }
       else if (pid == 0) {
       	p = (long) getpid();
       	close(s);
        i = 0;
        memset(&buffer, 0, sizeof(buffer));
       	while ((msg_size = read(newsock, buffer, sizeof(buffer) - 1)) > 0) {
          i = 0;
          while(1) {    //cyklus pre spracovanie a odosielanie sprav
            switch(state) {
              case 0:      //spracovanie uvodnej hlavicky LDAP Message
                if (buffer[i++] != 0x30) {
                  fprintf(stderr,"Error 1 this is not LDAP message\n");
                  exit(1);
                }
                if (buffer[i++] == 0x00) {
                  fprintf(stderr,"Error 2 this is not LDAP message\n");
                  exit(1);
                }
                if (buffer[i++] != 0x02) {
                  fprintf(stderr,"Error 3 this is not LDAP message\n");
                  exit(1);
                }
                if (!((buffer[i] >= 0x01) && ((int) buffer[i] <= 0x04))) {
                  fprintf(stderr,"Error 4 this is not LDAP message\n");
                  exit(1);
                }
                i = i + 1;
                if (!(buffer[i++] >= 0x00)) {
                  fprintf(stderr,"Error 5 this is not LDAP message\n");
                  exit(1);
                }
                if (!(buffer[i] >= 0x40 && (int) buffer[i] <= 0x7F)) {
                  fprintf(stderr,"Error 6 this isnt LDAP message\n");
                  exit(1);
                }
                else if (buffer[i] == 0x60) {    
                  state = 1;
                  i = i + 1;
                  break;
                }
                else if (buffer[i] == 0x63) {
                  state = 2;
                  i = i + 1;
                  break;
                }
                else if (buffer[i++] == 0x42) {
                  if (buffer[i] == 0x00) {
                    while_parser_end = 1;
                    break;
                  }

                }
                else {
                  while_parser_end = 1;
                  break;
                }

               case 1:      //state pre spracovanie protokolu BindRequest
                 if (buffer[i++] == 0x00) {
                  fprintf(stderr, "Error bind 1 this isnt LDAP message\n");
                  exit(1);
                 } 
                 if (buffer[i++] != 0x02) {
                  fprintf(stderr, "Error bind 2 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i++] != 0x01) {
                  fprintf(stderr,"Error bind 3 this isnt LDAP message\n");
                 }
                 tmp_d = (unsigned int) buffer[i++]; 
                 if (!(tmp_d >= 1 && tmp_d <= 127)) {
                  fprintf(stderr,"Error bind 4 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i++] != 0x04) {
                  fprintf(stderr,"Error bind 5 this isnt LDAP message");
                  exit(1);
                 }
                 if (buffer[i++] != 0x00) {
                  tmp_d = (unsigned int) buffer[i - 1];
                  i = (i + tmp_d) - 1;
                 }
                 if (buffer[i] == 0x80) {
                  state = 5;
                  break;
                 }
                 else if (buffer[i] == 0xA3) {
                  state = 5;
                  break;
                 }
                 else {
                  fprintf (stderr,"Error bind 6 this isnt LDAP message\n");
                  exit(1); 
                 }

                 break;  
              case 2:       //state pre spracovanie protokolu SearchRequest
                 if (buffer[i++] == 0x00) {
                  fprintf(stderr,"Error search 1 this isnt LDAP message\n");
                  exit(1);
                 } 
                 if (buffer[i++] != 0x04) {
                  fprintf(stderr,"Error search 2 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i++] != 0x00) {
                  tmp_d = (unsigned int) buffer[i - 1];
                  i = (i + tmp_d) - 1;
                 }  
                 if (buffer[i++] != 0x0a) {
                  fprintf(stderr,"Error search 3 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i++] != 0x01) {
                  fprintf(stderr,"Error search 4 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i] == 0x00) {
                  basic_flag = 1;
                 } 
                 else if (buffer[i] == 0x01) {
                  single_l_flag = 1;
                 }
                 else if (buffer[i] == 0x02) {
                  scope_flag = 1;
                 }
                 else {
                  fprintf(stderr,"Error search 5 this isnt LDAP message\n");
                  exit(1);
                 }
                 i = i + 1;
                 if (buffer[i++] != 0x0a) {
                  fprintf(stderr,"Error search 6 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i++] != 0x01) {
                  fprintf(stderr,"Error search 7 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i] != 0x00 && buffer[i] != 0x01 && buffer[i] != 0x02 && buffer[i] && 0x03) {
                  fprintf(stderr,"Error search 8 this isnt LDAP message\n");
                  exit(1);
                 }
                 i = i + 1;
                 if (buffer[i++] != 0x02) {
                  fprintf(stderr,"Error search 9 this isnt LDAP message\n");
                  exit(1);
                 } 
                 if (buffer[i] != 0x01 && buffer[i] != 0x02 && buffer[i] != 0x03 && buffer[i] != 0x04) {
                  fprintf(stderr,"Error search 10 this isnt LDAP message \n");
                  exit(1);
                 }
                 tmp_d = (unsigned int) buffer[i];
                 i = i + 1;

                 if (buffer[i] != 0x00) {
                  size_set = 1;                       //nastavenie flagu pre size ak bola zadana
                  memset(&size_buf, 0, sizeof(size_buf));
                  int j = 0;
                  while (j < tmp_d) {               //nacitanie size do pomocneho size_buf[] bufferu
                    size_buf[j] = buffer[i];
                    i++;
                    j++;
                  } 
                  if (tmp_d == 1) 
                    size = size_buf[0];      //konverzia size zo size_buf[] bufferu do premennej size typu int
                  else if (tmp_d == 2) 
                    size = size_buf[0] << 8 | size_buf[1];
                  else if (tmp_d == 3) 
                    size = size_buf[2] << 16 | size_buf[1] << 8 | size_buf[0];    
                  else  
                    size = size_buf[3] << 24 |size_buf[2] << 16 | size_buf[1] << 8| size_buf[0];    
                 }
                 else {
                  i = i + 1;
                 }  
                 if (buffer[i++] != 0x02) {
                  fprintf(stderr,"Error search 11 this isnt LDAP message\n");
                  exit(1);
                 }
                 if (buffer[i] != 0x01 && buffer[i] != 0x02 && buffer[i] != 0x03 && buffer[i] != 0x04) {
                  fprintf(stderr,"Error search 12 this isnt LDAP message\n");
                  exit(1);
                 }
                 tmp_d = (unsigned int) buffer[i];
                 i = i + 1;
                 if (buffer[i] != 0x00) {
                  memset(&size_buf, 0, sizeof(size_buf));
                  int j = 0;
                  while (j < tmp_d) {
                    size_buf[j] = buffer[i];
                    i++;
                    j++;
                  } 
                  if (tmp_d == 1)       //spracovanie a ulozenie time_limit
                    time_limit = size_buf[0];
                  else if (tmp_d == 2) 
                    time_limit = size_buf[0] << 8 | size_buf[1];
                  else if (tmp_d == 3) 
                    time_limit= size_buf[2] << 16 | size_buf[1] << 8 | size_buf[0];    
                  else  
                    time_limit = size_buf[3] << 24 |size_buf[2] << 16 | size_buf[1] << 8| size_buf[0];    
                 }
                 else 
                  i = i + 1;
                 if (buffer[i++] != 0x01){
                  fprintf(stderr,"Error search 13 this isnt LDAP message\n");
                 }
                 if (buffer[i++] != 0x01){
                  fprintf(stderr,"Error search 13 this isnt LDAP message\n");
                 }
                 if (buffer[i++] == 0x01) 
                  types = true;
                 state = 7;
                 break; 
              case 3:          //state pre generovanie a odoslanie spravy SearchResEntry pre 1 equality match, princip rovnaky ako vo funkcii logical_analysis
                if ((f = fopen("isa2017-ldap.csv","r")) == NULL) {
                  fprintf(stderr,"Error with file opening");
                  return(1);
                }
                if (strcmp((char*) &filter,"uid") == 0) {
                  send_flag = 2;
                }
                else if (strcmp((char*) &filter,"mail") == 0) {
                  send_flag = 3;
                }
                else if (strcmp((char*) &filter,"cn") == 0) {
                  send_flag = 1;
                } 
                else {
                  state = 0;
                  while_parser_end = 1;
                  fclose(f);
                  break;
                } 
                while (fgets(line, 1024, f)) {     //spracovanie a kontrola kazdeho riadku v subore
                  char* tmp = strdup(line);
                  send_uid = getfield(tmp, send_flag);
                  for(int i = 0; send_uid[i]; i++){
                    send_uid[i] = tolower(send_uid[i]);
                  }
                  if (send_flag == 3 && (!(isalpha(send_uid[strlen(send_uid) - 1])))) {
                    send_uid[strlen(send_uid) -1] = '\0';
                  }
                  if ((strcmp(send_uid, (char *) &name_to_filter)) == 0) {
                    if (size_set == 1) {
                      if (words_number == size) {
                        state = 4;
                        size_exceeded = 1;
                        break;
                      }
                    }
                    search_response(line, newsock);
                    words_number++;
                  }
                  free(tmp);
                }
                state = 4;      
                fclose(f);
                break;     
            case 4:   //state pre generovanie a odoslanie spravy SearchResDone()
              i = 0;
              {
                char buffer[15];
                buffer[i++] = 0x30;
                buffer[i++] = 0x0c;
                buffer[i++] = 0x02;
                buffer[i++] = 0x01;
                buffer[i++] = 0x02;
                buffer[i++] = 0x65;
                buffer[i++] = 0x07;
                buffer[i++] = 0x0a;
                buffer[i++] = 0x01;
                if (size_exceeded == 1) {
                  buffer[i++] = 0x04;
                }
                else {
                  buffer[i++] = 0x00;
                }  
                buffer[i++] = 0x04;
                buffer[i++] = 0x00;
                buffer[i++] = 0x04;
                buffer[i++] = 0x00; 
                w_size = write(newsock, buffer, sizeof(buffer) -1);
                state = 0;
                while_parser_end = 1;
                break;
              }  
            case 5:      //nastavenie a odoslanie spravy BindResponse
              {
                char buffer[15];  
                buffer[0] = 0x30;
                buffer[1] = 0x0c;
                buffer[2] = 0x02;
                buffer[3] = 0x01;
                buffer[4] = 0x01;
                buffer[5] = 0x61;
                buffer[6] = 0x07;
                buffer[7] = 0x0a;
                buffer[8] = 0x01;
                buffer[9] = 0x00;
                buffer[10] = 0x04;
                buffer[11] = 0x00;
                buffer[12] = 0x04;
                buffer[13] = 0x00;
                w_size = write(newsock, buffer, sizeof(buffer) -1);
              }
              while_parser_end = 1;
              state = 0;
              break;
            case 6:      //state pre generovanie a odoslanie spravy SearchResEntry pre 1 substring, princip rovnaky ako vo funkcii logical_analysis
              if ((f = fopen("isa2017-ldap.csv","r")) == NULL) {
                fprintf(stderr,"Error with file opening");
                return(1);
              }
              if (strcmp((char*) &filter,"uid") == 0) 
                send_flag = 2;
              else if (strcmp((char*) &filter, "cn") == 0)
                send_flag = 1;
              else if (strcmp((char*) &filter, "mail") == 0)
                send_flag = 3;
              while (fgets(line, 1024, f)) {    //spracovanei a kontrola kazdeho riadku v subore ci vyhovuje zadanemu Substringu
                char* tmp = strdup(line);
                send_uid = getfield(tmp, send_flag);
                for(int i = 0; send_uid[i]; i++){
                  send_uid[i] = tolower(send_uid[i]);
                }
                if (send_flag == 3 && (!(isalpha(send_uid[strlen(send_uid) - 1])))) {
                  send_uid[strlen(send_uid) -1] = '\0';
                }
                if (substr_control(send_uid)) {
                  if (size_set == 1) {
                    if (words_number == size) {
                      size_exceeded = 1;
                      state = 4;
                      break;
                    }
                  }
                  search_response(line, newsock);
                  words_number++;
                }
                free(tmp);
              }
              fclose(f);
              state = 4;
              break;    
            case 7:        //state pre vyhodnocovanie ci sa jedna o equalityMatch, Substrings, alebo logicke vyrazy
              memset(&filter, 0, sizeof(filter));
              memset(&name_to_filter, 0, sizeof(name_to_filter));
              if (buffer[i] == 0xa3) {
                i = i + 2;
                if (buffer[i++] != 0x04) {
                  fprintf(stderr,"Error search 13 this isnt LDAP message\n");
                  exit(1);
                }
                if (buffer[i] != 0x00) {
                  tmp_d = (unsigned int) buffer[i++];
                  for (int j = 0; j<tmp_d;j++) {
                    filter[j] = buffer[i++];
                  }
                  if (buffer[i++] != 0x04) {
                    fprintf(stderr,"Error search 14 this isnt LDAP message\n");
                    exit(1);
                  }
                  if (buffer[i] != 0x00) {
                    tmp_d = (unsigned int) buffer[i++];
                    for (int j = 0; j< tmp_d;j++) {
                      name_to_filter[j] = buffer[i++];
                    }
                  }
                }
                for(int i = 0; filter[i]; i++){
                  filter[i] = tolower(filter[i]);
                }
                for(int i = 0; name_to_filter[i]; i++){
                  name_to_filter[i] = tolower(name_to_filter[i]);
                }
                state = 3;
                break;
              }
              else if (buffer[i] == 0xa4) {
                size_of_subname = 0;
                number_of_subnames = 0;
                i = i + 3;
                if (buffer[i] != 0x00) {
                  tmp_d = (unsigned int) buffer[i++];
                  for (int j = 0; j<tmp_d;j++) {
                    filter[j] = buffer[i++];
                  }
                }
                 for(int i = 0; filter[i]; i++){
                  filter[i] = tolower(filter[i]);
                }
                i = i + 2;
                int counter = 0;
                int j = 0;
                while ((j = substring_analyze(buffer,i, counter++)) != -1) {
                  i = j;
                }
                 int substr_counter = 0;
                 for (int j = 0; j< number_of_subnames;j++) {
                   while(name_to_filter[substr_counter] != 0x00) {
                    name_to_filter[substr_counter] = tolower(name_to_filter[substr_counter]);
                    substr_counter++;
                  }
                  substr_counter++;
                }
                state = 6;
                break;
              }
              else if (buffer[i] == 0xa0 || buffer[i] == 0xa1 || buffer[i] == 0xa2) {
                int log_flag = -1;
                if ((f = fopen("isa2017-ldap.csv","r")) == NULL) {
                  fprintf(stderr,"Error with file opening");
                  return(1);
                }
                int counter = i;
                while (fgets(line, 1024, f)) {     //spracovanie a kontrola kazdeho riadku v subore ci vyhovuje zadanemu logickemu filtru
                  int o_k = 1;
                  i = counter;
                  while (buffer[i] != 0x30) {
                    if (buffer[i] == 0xa0) {
                      log_flag = 0;
                    }  
                    else if (buffer[i] == 0xa1) {
                      log_flag = 1;
                    }
                    else if (buffer[i] == 0xa2) {
                      log_flag = 2;
                    }     
                    i = i + 1;
                    unsigned int log_length = (unsigned int) buffer[i++];
                    int result = logical_analysis(buffer, line, log_length, log_flag);
                    if (result == 0)
                      o_k = 0;
                  }
                  if (o_k != 0) {
                    if (size_set == 1) {
                      if (words_number == size) {
                        state = 4;
                        size_exceeded = 1;
                        break;
                      }
                    }
                    search_response(line, newsock);
                    words_number++;
                  }
                }
                state = 4;
                break;
              }
            default:
              fprintf(stderr,"Error 7 this isnt LDAP message\n");
              exit(1);
          }  
          if (while_parser_end == 1) {
            while_parser_end = 0; 
            break;
          }
        }
        i = 0;
        memset(&buffer, 0, sizeof(buffer));
      }
      close(newsock);
      exit(0);
    }
    else {
      err(1, "fork() failed\n");
    }
  }
  close(s);
  return (0);
}
