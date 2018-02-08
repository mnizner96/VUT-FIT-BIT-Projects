#include <stdio.h>
#include <limits.h>
#include <stdbool.h> //pre pouzitie premennej typu bool 
#include <ctype.h>   //pre pouzitie funkcie isdigit
#include <stdlib.h>  //pre pouzitie funckie strtol a atol
#include <time.h>    //pre pouzitie funkcie mktime
struct tm time_str;
char daybuf[20];    
int number_word_dateformat(char slovo[100]); //prototyp funkcie pre zistenie, ci sa  jedna o slovo, cislo alebo format datumu v tvare DDDD-DD-DD
int prime(char slovo[100]);                  //prototyp funkcie pre zistenie, ci je cislo prvocislom
int palindrome(char slovo[100]);             //prototyp funkcie pre zistenie, ci je slovo palindromom
int date(char slovo[100]);                   //prototyp funkcie pre zistenie, ci format datumu splna normy spravneho datumu
int main(int argc, char *argv[])             //funkcia main s paramtetramy pocet argumentov a argument
{
  if ((argc > 1) && (argv[1][0] != ' '))                       
       printf ("Program cita slova z textoveho suboru a vyhodnocuje,\n"
               "ci sa jedna o word, palindrome, number, primenumber, date\n");
  else                                     
  {
    char word[101];                //deklaracia slova
    int code = scanf("%s",word);   //deklaracia premennej pre nacitanie slova
 	  while (code == 1)                          
	  { 
      if (word[100] != '\0')   //ak ma slovo viac ako 100 znakov tak program informuje o chybe a skonci
      {
        printf ("Bolo zadane chybne word\n");
        break;
      }
	    if (number_word_dateformat(word) == 0) //ak ide o slovo, urobi sa detekcia,ci sa jedna o palindrom
      {
        if (palindrome(word) == 1)
          printf ("word : %s (palindrome)\n",word);
          else
            printf ("word : %s\n",word);
      }
      if (number_word_dateformat(word) == 1) //ak ide o cislo, urobi sa detekcia, ci sa jedna o prvocislo a ci je cislo v rozsahu int
      {
        if (atol(word) > INT_MAX)           //ak cislo presahuje max velkost int tak program informuje o chybe a skonci
        {
          printf ("Bolo zadane chybne number\n");
            break;
        }
        if (prime(word) == 1)
          printf ("number : %s (prime)\n",word);
        else
          printf ("number : %s\n",word);
      }
      if (number_word_dateformat(word) == 2) //ak ide o format datumu, urobi sa detekcia ci su splnene normy spravneho datumu
      {
		    if (date(word) == 1)
          printf ("Date : %s %s\n",daybuf,word);
        else
          printf ("word : %s\n",word);
		  }
	   code = scanf("%s",word);              
    }
  }
    return 0;
}
   

 int number_word_dateformat(char slovo[101]) //funkcia pre zistenie, ci sa jedna o slovo, cislo, alebo format datumu
    {
     int length = 0;     //deklaracia dlzky slova  
     int poc_cisel = 0;  //deklaracia celkoveho poctu cisel v slove
     int poc_znakov = 0; //deklaracia celkoveho poctu znakov v slove
     while (slovo[length] != '\0')
    {
      if (isdigit(slovo[length]))     
        poc_cisel++;
      else
        poc_znakov++;
      length++;
    }
    if (poc_znakov > 0)            
      {
        if ((slovo[4] == '-') && (slovo[7] == '-') && (poc_cisel==8))
          return 2;                   //je to format datumu
        else
          return 0;                   //je to slovo
      }
     else
        return 1;                   //je to cislo
    }

int palindrome(char slovo[101])      //funkcia pre zistenie, ci je dane slovo palindrom
   {
    int beg,mid,end;                 //deklaracia zaciatku, stredu a konca slova
    int length = 0;                  //deklaracia dlzky slova
    while (slovo[length] != '\0')
      length++;
    end = length - 1;
    mid = length/2;
    beg = 0;
    while (beg<mid)
    {
      if (slovo[beg] != slovo[end])
        break;
      beg++;
      end--;
    }
    if (beg == mid)
      return 1;  //je to palindrom
    else
      return 0;  //je to slovo
   }


int prime(char slovo[101])       //funkcia pre zistenie, ci je cislo prvocislom
   {
    char *p;                     //deklaracia ukazatelu na znak pre vyuzitie funkcie strtol
    bool number_is_prime = 0;    //deklaracia premennej typu bool pre zistenie, ci ide o prvocislo
    int x = strtol(slovo,&p,10); //deklaracia premennej typu int pre prevod slova typu char
    if (x == 2) 
    number_is_prime = 1;
    if (x>2)
      {
        int delitel = 2;          //deklaracia delitela cisla x 
        while (delitel!=x)
        {
          if (x%delitel == 0)
            break;
          else
          {
		        if (delitel == x-1)
              number_is_prime = 1;
            }
          delitel++;
        }
      }
    if (number_is_prime == 1)
      return 1; //je to prvocislo
    else
      return 0; //je to cislo
   }
     

int date(char slovo[101])            //funkcia pre zistenie ci format datumu splna normy spravneho datumu
   {
    char *p;                         //deklaracia ukazatelu na znak pre vyuzitie funkcie strtol
    bool dat = 0;                    //deklaracia premennej typu bool pre zistenie ci su splnene normy spravneho datumu
    int year  = strtol(slovo,&p,10); //deklaracia roku
    int month = strtol(p,&p,10);     //deklaracia mesiaca
    month = -(month);
    int day   = strtol(p,&p,10);     //deklaracia dna
    day   = -(day);
    time_str.tm_year = year - 1990;
    time_str.tm_mon  = month - 1;
    time_str.tm_mday = day;
    time_str.tm_hour = 0;
    time_str.tm_min  = 0;
    time_str.tm_sec  = 1;
    time_str.tm_isdst= -1;
    if (mktime(&time_str) != -1)     //overenie spravneho formatu datumu
    {
      while (dat!=1)
        {
	        if ((month>12) || (month==0)) //overenie ci format datumu splna normy spravneho datumu
            break;
	        if ((day>31) || (day==0))
	          break;
          if (year<1900)
            break;
          strftime(daybuf, sizeof(daybuf), "%a", &time_str);
          dat=1;
        }
    }
    if (dat == 1)
      return 1; //je to datum
    else
      return 0; //je to slovo
   }
		    

