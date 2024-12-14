#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPiI2C.h>
#include <ads1115.h>
#include <fcntl.h>
#include <wiringSerial.h>
#include <unistd.h>


#ifndef CRTSCTS
#define CRTSCTS 020000000000
#endif

#define FD_STDIN 0

#define BASE 120
#define ADC_ADDR 0x48

#define VEL_INICIAL 100
#define VEL_MAX_INICIAL 1000
#define VEL_MAX 3000
#define VEL_MIN 0

#define BAUDRATE B115200 /* Definicion de cte de velocidad de comnicacion serial*/

#define LOC_TTY 0
#define REM_TTY 1
#define O_L_TTY 2
#define O_R_TTY 3


#define NUM_SEQS 8
#define CNUM_SEQS sprintf(CNUM_SEQS, "%d", NUM_SEQS);
#define NUM_FUNCS NUM_SEQS+1
#define MAX_LEN 6
#define PWD "12345"
#define MAX_INTENTOS 2
#define SPEED_UP_CHAR 'B'
#define SPEED_DOWN_CHAR 'A'

extern int velMs(int cuentas);


typedef struct
{
    int id;
    char* name;
    void (*func)();
    int vel;
    struct termios* tty;
    int fd;

} hilo_arg_t;

extern char* funcNames[NUM_SEQS];

void setNCanon(struct termios*, int);
void resetCanon(struct termios*, int);
void adcInit(void);



void cls(int);
void initWiringPi(int);
void off(int);
void* keyInput(void*);
void seq1(hilo_arg_t*);
void seq2(hilo_arg_t*);
void seq3(hilo_arg_t*);
void seq4(hilo_arg_t*);
void seq5(hilo_arg_t*);
void seq6(hilo_arg_t*);
void seq7(hilo_arg_t*);
void seq8(hilo_arg_t*);
void* velUpdate(void*);
void* pageVelocidadInicial(void*);
int setVelocidadInicial(void);

int checkPwd(int);
void setMode(int*, struct termios*, int, int);
void nbd(int ms);
void encenderLeds(char bits, int);
int checkStop(pthread_mutex_t*, int*);
void printModeMenu(int);
void printSeqMenu(hilo_arg_t*, int);
void execFunc(pthread_t*, hilo_arg_t*, int);
void* hilo_func(void*);

void closeFD(int, int , struct termios*);
//variables y banderas
extern int stop;
extern pthread_mutex_t mut;



int initLocal(struct termios*);
int initRemote(struct termios*);