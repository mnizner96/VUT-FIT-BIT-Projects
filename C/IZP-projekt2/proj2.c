/*
 * Author: Martin Nizner
 * Description : Výpočet logaritmu pomocou taylorového polynómu alebo iterácií zreťazeného zlomku
*/

#include <stdio.h>
#include <string.h>  //pre pouzitie funkcie strcmp
#include <stdlib.h>  //pre pouzitie funkcii strtod a strtol
#include <math.h>    //pre pouzitie funkcie log pre overenie vypoctu
#include <limits.h>  //pre pouzitie funkcie DBL_MAX a UINT_MAX 
#include <float.h>   //pre pouzitie DBL_MAX
double taylor_log (double x, unsigned int n); 
unsigned int taylor_log_iterations (double eps, double min, double max); //prototyp funkcie pre vypocet poctu iteracii taylorovho polynomu
double cfrac_log (double x, unsigned int n);
unsigned int cfrac_log_iterations (double eps, double min, double max); //prototyp funkcie pre vypocet poctu iteracii zretazeneho zlomku
int main(int argc, char *argv[]) 
{
   if ((argc==4) && (strcmp(argv[1],"--log")==0))
   {
      double X;
      unsigned long int N;
      char *p, *q;
      X=strtod(argv[2], &p); 
      N=strtoul(argv[3], &q, 10);
      if ((p[0] != '\0') || (q[0] != '\0'))                     //ak bol zadany chybny vstup tak program informuje o chybe a skonci
      {
         printf ("bol zadany chybny vstup\n");
         return (0); 
      }
      else if ((X > DBL_MAX) || (N > UINT_MAX) || (N == 0))     //ak bolo zadane chybne cislo tak program o nom informuje a skonci
      {
         printf ("Bolo zadane chybne cislo presahujuce hodnotu max daneho typu alebo nespravny pocet iteracii\n");
         return (0);
      }
      printf ("log(%g) = %.12g\n",X,log(X));
      printf ("cf_log(%g) =  %.12g\n",X,cfrac_log(X,N));
      printf ("taylor_log(%g) = %.12g\n",X,taylor_log(X,N));
   }
   else if ((argc == 5) && (strcmp(argv[1],"--iter") == 0))
   {
      char *p, *q, *r;
      double MIN = strtod(argv[2],&p);
      double MAX = strtod(argv[3],&q);
      double EPS = strtod(argv[4],&r);
      if ((p[0] != '\0') || (q[0] != '\0') || (r[0] != '\0'))       //ak bol zadany chybny vstup tak program informuje o chybe a skonci
      {
         printf ("bol zadany nespravny vstup \n");
         return (0);
      }
      else if ((MIN > MAX) ||  (EPS <= 0.0) || (MIN > DBL_MAX) ||(MAX > DBL_MAX) || (EPS > DBL_MAX)) //ak bolo zadane chybne cislo tak program o nom informuje a skonci
      {                                                                                                                              
         printf ("Bola zadana zaporna hodnota EPS alebo MIN>MAX alebo hodnota presahujuca maximalnu hodnotu double\n");
         return (0);  
      }
      unsigned int cfrac_log_iter = cfrac_log_iterations(EPS,MIN,MAX);   //deklaracia poctu iteracii zretazeneho zlomku
      unsigned int taylor_log_iter = taylor_log_iterations(EPS,MIN,MAX); //deklaracia poctu iteracii taylorovho polynomu
      double cfrac_log_min = cfrac_log(MIN,cfrac_log_iter);              //deklaracia hodnoty logaritmu MIN cez cfrac log
      double cfrac_log_max = cfrac_log(MAX,cfrac_log_iter);              //deklaracia hodnoty logaritmu MAX cez cfrac log
      double taylor_log_min = taylor_log(MIN,taylor_log_iter);           //deklaracia hodnoty logaritmu MIN cez taylor log
      double taylor_log_max = taylor_log(MAX,taylor_log_iter);           //deklaracia hodnoty logaritmu MAX cez taylor log
      printf ("log(%g) = %.12g\n",MIN,log(MIN));
      printf ("log(%g) = %.12g\n",MAX,log(MAX));
      printf ("continued fraction iterations = %d\n",cfrac_log_iter);
      printf ("cf_log(%g) = %.12g\n",MIN,cfrac_log_min);
      printf ("cf_log(%g) = %.12g\n",MAX,cfrac_log_max); 
      printf ("taylor polynomial iterations = %d\n",taylor_log_iter);
      printf ("taylor_log(%g) = %.12g\n",MIN,taylor_log_min);
      printf ("taylor_log(%g) = %.12g\n",MAX,taylor_log_max);
   }
   else
   {
      printf("bol zadany chybny vstup\n");    //ak bol zadany chybny tvar vstupu tak program informuje o chybe a skonci
   } 
   return (0);
}	


double taylor_log(double x, unsigned int n )  
{
   if (x == 0.0)
   {
      return (-INFINITY);
   }     
   else if (x < 0.0)
   {
      return (NAN);
   }     
   else
   {
      double sum = 0.0; //deklaracia celkoveho suctu/rozdielu jednotlivych clenov podla vzorca
      double t;         //deklaracia jedneho clena t
      double citatel;   //deklaracia citatela clena t
      unsigned int i;  
      if ((x>0.0) && (x<1.0))
      {
         x = 1 - x;       //zmena hodnoty x kvoli log(1-x)
         citatel = 1.0;
         for (i = 0;i < n;i++)
   	 {
   	    citatel = citatel * x;    //i+1-ty citatel clena t
            t=citatel / ((double)i + 1);  //i+1-ty clen t
            sum =sum - t;                
         }
      }
      else if (x >= 1.0)
      {
         citatel = 1.0;
         for (i = 0;i < n;i++)
         { 
            citatel = citatel * ((x - 1) / x); 
            t = citatel/((double)i+1);
            sum=sum+t;
         }
      }
      return(sum);
   }
}
       

double cfrac_log(double x, unsigned int n)
{
   if (x == 0.0)
   {
      return (-INFINITY);
   }           
   else if (x < 0.0)
   {
      return (NAN);
   }           
   else
   {
      x = (x - 1)/(x + 1);  //zmena hodnoty x kvoli log(1-x)/(1+x)
      double cf = 0.0;  //deklaracia hodnoty zretazeneho zlomku
      double a;         //deklaracia clenov citatela zretazeneho zlomku
      double y;
      double b;         //deklaracia menovatela zretazeneho zlomku
      while (n>=1)
      {
         a = (n - 1) * (n - 1);
         b = (2 * n) - 1;
         y = x * x;
         if (n == 1)
         {
            a = 2.0;
            y = x;
         }
         cf = (a * y)/(b - cf);
         n  = n - 1;
      }
      return(cf);
   }
}
  
unsigned int taylor_log_iterations(double eps, double min, double max)
{
   unsigned int iterations = 1; //deklaracia poctu iteracii
   double y = 0.0;              //deklaracia odchylky  log min cez taylor log od skutocnej hodnoty log 
   double z = 0.0;              //deklaracia odchylky  log max cez taylor log od skutocnej hodnoty log
   while (1)
   {
      y = (taylor_log(min,iterations) - log(min));
      if (y < 0)
      {       
         y = -y;
      }       
      z = (taylor_log(max,iterations) - log(max));
      if (z < 0)
      {       
         z = -z;
      }       
      if ((y <= eps) && (z <= eps))
      {
         break;
      }
      iterations++;
   }
   return(iterations);
}
unsigned int cfrac_log_iterations (double eps, double min, double max)
{
   unsigned int iterations = 1;  //deklaracia poctu iteracii
   double y = 0.0;               //deklaracia odchylky log min cez cfrac log od skutocnej hodnoty log
   double z = 0.0;               //deklaracia odchylky log max cez cfrac log od skutocnej hodnoty log
   while (1)
   {
      y = (cfrac_log(min,iterations) - log(min));
      z = (cfrac_log(max,iterations) - log(max));
      if (y < 0.0)
      {          
         y = -y;
      }        
      if (z < 0.0)
      {          
         z = -z;
      }        
      if ((y <= eps) && (z <= eps))
      {
         break;
      }
      iterations++;
   }
   return (iterations);
}
                 
           
               

	
