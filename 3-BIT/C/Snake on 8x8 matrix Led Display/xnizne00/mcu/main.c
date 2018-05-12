

//#include <fitkitlib.h>

// Autor: Martin Nizner
// Login: xnizne00

#define UP 2
#define DOWN 8
#define LEFT 4
#define RIGHT 6
#define HEAD 1
#define TAIL 9
#define FOOD 5
#include <stdio.h>
#include <stdlib.h>
#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include <limits.h>


char last_ch; //naposledy precteny znak
char char_cnt;
int high_score = 0;
void print_user_help(void){
}

unsigned int lfsr = 0xACE1u;
unsigned int bit,t=0;
 
unsigned int random(unsigned int startNumber,unsigned int endNumber) //generator nahodnych cisel podla bitoveho posunu
																	 //zdroj : https://en.wikipedia.org/wiki/Linear-feedback_shift_register
{
    if(startNumber == endNumber) return startNumber;
    int *p = malloc(sizeof(int));
    t = (int *)p;
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    lfsr = ((bit<<15) | (lfsr>>1) | t)%endNumber;
    while(lfsr<startNumber){
        lfsr = lfsr + endNumber - startNumber;
    }
    return lfsr;
}

/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/

unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/



void fpga_initialized()
{
  LCD_init();
  LCD_clear();
}

int keyboard_idle()
{
  char ch;
  ch = key_decode(read_word_keyboard_4x4());
  if (ch != last_ch) 
  {
    last_ch = ch;
  }
  return 0;
}




void print(char message[50], int value)
{
	char pom[50];
	LCD_clear();
	LCD_append_string(message);
	sprintf(pom, "%d", value);
	LCD_append_string(pom);
}

int main( ){
	last_ch = 0;
	initialize_hardware();
	keyboard_init();
	char_cnt = 0;
	int cnt = 0;
	char pom_c = 0;
	int high_score = 0;
	int f_x = 0;   //suradnice potravy
	int f_y = 0;
	P6DIR = 0xff;
	P6OUT = 0x00;
	P4DIR = 0x0f;
	P4OUT = 0x00;
	P2DIR =	0xf0;
	P2OUT = 0x00;
	int had[8][8]; //hracia plocha
	int i = 0;
	int j = 0;
	for (i = 0 ; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			had[i][j] = 0;
		}
	}
	int h_x = random(0,7);  //inicializacia suradnic hlavy
	int h_y = random(0,7);
	int t_x = 0;   //inicializacia suradnic chvosta hada
	int t_y = 0;
	had[h_x][h_y] = LEFT;   //uvodna inicializacia hada
	if (h_y < 3) {
		had[h_x][h_y] = RIGHT;
		had[h_x][h_y + 1] = RIGHT;
		had[h_x][h_y + 2] = TAIL;
		t_x = h_x;
		t_y = h_y + 2;
	}
	else {
		had[h_x][h_y - 1] = LEFT;
		had[h_x][h_y - 2] = TAIL;
		t_x = h_x;
		t_y = h_y - 2;
	}

	

	f_x = random(0, 7);
	f_y = random(0, 7);
	
	while(1) {
		if (had[f_x][f_y] == 0)
			break;
		f_x = random(0, 7);
		f_y = random(0, 7);
	}
	had[f_x][f_y] = FOOD;
	print("Vase skore je ", high_score);
	int led_i = 0;
	int led_j = 0;
	int row = 0;
	int mul = 128;
	while(1){
		delay_ms(1);
		cnt++;
		if (pom_c != last_ch) {
			pom_c = last_ch;
			if (pom_c == '8') {
				h_x++;
				if (h_x > 7 || (had[h_x][h_y] != 0 && had[h_x][h_y] != 5)) {
					P6OUT = 0x00;
					P2OUT = 0xff;
					P4OUT = 0xff;
					print("Koniec, vysledne skore je ", high_score);
					break;
				}
			
				else {
					if (had[h_x][h_y] == 5) {
						had[h_x][h_y] = UP;
						high_score++;
						while(1) {
							if (had[f_x][f_y] == 0)
								break;
							f_x = random(0, 7);
							f_y = random(0, 7);
							}
						had[f_x][f_y] = FOOD;
					}
					else {
						had[h_x][h_y] = UP;
						had[t_x][t_y] = 0;
						/*print("zero na x ", t_x);
						delay_ms(2000);
						print("zero na y ", t_y);
						delay_ms(2000);
						print ("Ozaj zero ", had[t_x][t_y]);
						delay_ms(2000);
						*/
						if ((had[t_x+1][t_y] != 0) && (had[t_x+1][t_y] != 5) && (t_x != 7) && (had[t_x+1][t_y] == UP)) {
							had[t_x+1][t_y] = TAIL;
							t_x++;
						}
						else if ((had[t_x-1][t_y] != 0) && (had[t_x-1][t_y] != 5) && (t_x != 0) && (had[t_x - 1][t_y] == DOWN)) {
							had[t_x-1][t_y] = TAIL;
							t_x--;
						}
						else if ((had[t_x][t_y + 1] != 0) && (had[t_x][t_y + 1] != 5) && (t_y != 7) && (had[t_x][t_y + 1] == LEFT)) {
							had[t_x][t_y + 1] = TAIL;
							t_y++;
						}
						else if ((had[t_x][t_y - 1] != 0) && (had[t_x][t_y - 1] != 5) && (t_y != 0) && (had[t_x][t_y - 1] == RIGHT)) {
							had[t_x][t_y - 1] = TAIL;
							t_y--;
						}
					}
						
				}
			}
			else if (pom_c == '6') {
				h_y++;
				if (h_y > 7 || (had[h_x][h_y] != 0 && had[h_x][h_y] != 5)) {
					print("Koniec, vysledne skore je ", high_score);
					P6OUT = 0x00;
					P2OUT = 0xff;
					P4OUT = 0xff;
					break;
				}
			
				else {
					if (had[h_x][h_y] == 5) {
						had[h_x][h_y] = LEFT;
						high_score++;
						while(1) {
							if (had[f_x][f_y] == 0)
								break;
							f_x = random(0, 7);
							f_y = random(0, 7);
							}
						had[f_x][f_y] = FOOD;
					}
					else {
						had[h_x][h_y] = LEFT;
						had[t_x][t_y] = 0;
						/*print("zero na x ", t_x);
						delay_ms(2000);
						print("zero na y ", t_y);
						delay_ms(2000);
						print ("Ozaj zero ", had[t_x][t_y]);
						delay_ms(2000);
						*/
						if ((had[t_x+1][t_y] != 0) && (had[t_x+1][t_y] != 5) && (t_x != 7) && (had[t_x+1][t_y] == UP)) {
							had[t_x+1][t_y] = TAIL;
							t_x++;
						}
						else if ((had[t_x-1][t_y] != 0) && (had[t_x-1][t_y] != 5) && (t_x != 0) && (had[t_x - 1][t_y] == DOWN)) {
							had[t_x-1][t_y] = TAIL;
							t_x--;
						}
						else if ((had[t_x][t_y + 1] != 0) && (had[t_x][t_y + 1] != 5) && (t_y != 7) && (had[t_x][t_y + 1] == LEFT)) {
							had[t_x][t_y + 1] = TAIL;
							t_y++;
						}
						else if ((had[t_x][t_y - 1] != 0) && (had[t_x][t_y - 1] != 5) && (t_y != 0) && (had[t_x][t_y - 1] == RIGHT)) {
							had[t_x][t_y - 1] = TAIL;
							t_y--;
						}
					}
						
				}
			}
			else if (pom_c == '4') {
				h_y--;
				if (h_y < 0 || (had[h_x][h_y] != 0 && had[h_x][h_y] != 5)) {
					P6OUT = 0x00;
					P2OUT = 0xff;
					P4OUT = 0xff;
					print("Koniec, vysledne skore je ", high_score);
					break;
				}
			
				else {
					if (had[h_x][h_y] == 5) {
						had[h_x][h_y] = RIGHT;
						high_score++;
						while(1) {
							if (had[f_x][f_y] == 0)
								break;
							f_x = random(0, 7);
							f_y = random(0, 7);
							}
						had[f_x][f_y] = FOOD;
					}
					else {
						had[h_x][h_y] = RIGHT;
						had[t_x][t_y] = 0;
						/*print("zero na x ", t_x);
						delay_ms(2000);
						print("zero na y ", t_y);
						delay_ms(2000);
						print ("Ozaj zero ", had[t_x][t_y]);
						delay_ms(2000);
						*/
						if ((had[t_x+1][t_y] != 0) && (had[t_x+1][t_y] != 5) && (t_x != 7) && (had[t_x+1][t_y] == UP)) {
							had[t_x+1][t_y] = TAIL;
							t_x++;
						}
						else if ((had[t_x-1][t_y] != 0) && (had[t_x-1][t_y] != 5) && (t_x != 0) && (had[t_x - 1][t_y] == DOWN)) {
							had[t_x-1][t_y] = TAIL;
							t_x--;
						}
						else if ((had[t_x][t_y + 1] != 0) && (had[t_x][t_y + 1] != 5) && (t_y != 7) && (had[t_x][t_y + 1] == LEFT)) {
							had[t_x][t_y + 1] = TAIL;
							t_y++;
						}
						else if ((had[t_x][t_y - 1] != 0) && (had[t_x][t_y - 1] != 5) && (t_y != 0) && (had[t_x][t_y - 1] == RIGHT)) {
							had[t_x][t_y - 1] = TAIL;
							t_y--;
						}
					}
						
				}
			}
			else if (pom_c == '2') {
				h_x--;
				if (h_x < 0 || (had[h_x][h_y] != 0 && had[h_x][h_y] != 5)) {
					P6OUT = 0x00;
					P2OUT = 0xff;
					P4OUT = 0xff;
					print("Koniec, vysledne skore je ", high_score);
					break;
				}
			
				else {
					if (had[h_x][h_y] == 5) {
						had[h_x][h_y] = DOWN;
						high_score++;
						while(1) {
							if (had[f_x][f_y] == 0)
								break;
							f_x = random(0, 7);
							f_y = random(0, 7);
							}
						had[f_x][f_y] = FOOD;
					}
					else {
						had[h_x][h_y] = DOWN;
						had[t_x][t_y] = 0;
						/*print("zero na x ", t_x);
						delay_ms(2000);
						print("zero na y ", t_y);
						delay_ms(2000);
						print ("Ozaj zero ", had[t_x][t_y]);
						delay_ms(2000);
						*/
						if ((had[t_x+1][t_y] != 0) && (had[t_x+1][t_y] != 5) && (t_x != 7) && (had[t_x+1][t_y] == UP)) {
							had[t_x+1][t_y] = TAIL;
							t_x++;
						}
						else if ((had[t_x-1][t_y] != 0) && (had[t_x-1][t_y] != 5) && (t_x != 0) && (had[t_x - 1][t_y] == DOWN)) {
							had[t_x-1][t_y] = TAIL;
							t_x--;
						}
						else if ((had[t_x][t_y + 1] != 0) && (had[t_x][t_y + 1] != 5) && (t_y != 7) && (had[t_x][t_y + 1] == LEFT)) {
							had[t_x][t_y + 1] = TAIL;
							t_y++;
						}
						else if ((had[t_x][t_y - 1] != 0) && (had[t_x][t_y - 1] != 5) && (t_y != 0) && (had[t_x][t_y - 1] == RIGHT)) {
							had[t_x][t_y - 1] = TAIL;
							t_y--;
						}
					}
						
				}
			}
		}
		pom_c = last_ch;




		for (led_i = 0; led_i < 8; led_i++) {   //rozsvietenie maticoveho displeja podla hracej plochy
			delay_ms(1);
			for (led_j = 0; led_j < 8; led_j++){
				if (had[led_i][led_j] != 0) {
					/*print ("Had neni zero ", had[led_i][led_j]);
					delay_ms(2000);
					print ("No zero x", led_i);
					delay_ms(2000);
					print ("No zero y", led_j);
					delay_ms(2000);
					*/
					row = row + (mul);
				}
				mul = mul / 2;
			}
			mul = 128;
			P2OUT = row;
			P4OUT = row;
			row = 0;
			switch (led_i){
				case 0: 
					P6OUT = 0xfe; 
					break;
				case 1: 
					P6OUT = 0xfd; 
					break;
				case 2: 
					P6OUT = 0xfb; 
					break;
				case 3:
					P6OUT = 0xf7; 
					break;
				case 4: 
					P6OUT = 0xef; 
					break;
				case 5: 
					P6OUT = 0xdf; 
					break;
				case 6: 
					P6OUT = 0xbf; 
					break;
				case 7: 
					P6OUT = 0x7f; 
					break;
			}

		}
		print("Vase skore je ", high_score);

		 keyboard_idle();               
	}
}