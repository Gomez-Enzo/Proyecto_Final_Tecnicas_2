#include "../inc/inc.h"

int initLocal(struct termios *tty)
{
    tcgetattr(FD_STDIN, &tty[O_L_TTY]);
    tty[LOC_TTY] = tty[O_L_TTY];
    return FD_STDIN;
}

int initRemote(struct termios *tty)
{
    int fd;                                       /* Descriptor de archivo del puerto serie*/
    fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY); /* Apertura del puerto serie */
    if (fd == -1)
    {
        printf("ERROR : no se pudo abrir el dispositivo.\n");
        return -1;
    }
    tcgetattr(fd, &tty[O_R_TTY]);
    tty[REM_TTY] = tty[O_R_TTY];
    tty[REM_TTY].c_cflag = BAUDRATE | CREAD;
    tty[REM_TTY].c_cflag &= ~PARENB;
    tty[REM_TTY].c_cflag &= ~CSTOPB;
    tty[REM_TTY].c_cflag &= ~CSIZE;
    tty[REM_TTY].c_cflag |= CS8;
    tty[REM_TTY].c_lflag |= ICANON | ECHO | ECHOE;
    tty[REM_TTY].c_cc[VTIME] = 0;          /* temporizador entre caracter */
    tty[REM_TTY].c_cc[VMIN] = 1;           /* bloquea lectura hasta llegada de un caracter */
    tcflush(fd, TCIFLUSH);                 /* Descarta datos recibidos en el buffer del puerto serie pero no leido aun*/
    tcsetattr(fd, TCSANOW, &tty[REM_TTY]); /* Seteo de los parametros de configuracion nuevos*/
    return fd;
}

void readChar(char *c, int fd)
{
    if (fd == FD_STDIN)
        *c = getchar();
    else
        read(fd, c, 1);
}

void setMode(int *fd, struct termios *tty, int lfd, int rfd)
{
    resetCanon(tty, *fd);
    int opt_n;
    char opt;
    
    read(*fd, &opt, 1);
    opt_n = (int)(opt - '0');
    if (opt_n == 1)
    {
        *fd = lfd;
        dprintf(rfd, "\033[2J\033[H");
        dprintf(rfd, "MODO LOCAL\n");
        dprintf(lfd, "\033[2J\033[H");
        tty[REM_TTY].c_lflag &= ~(ECHO | ECHOE);
        tcsetattr(rfd, TCSANOW, &tty[REM_TTY]);
    }
    else if (opt_n == 2)
    {
        *fd = rfd;
        dprintf(lfd, "\033[2J\033[H");
        dprintf(lfd, "MODO REMOTO\n");
        dprintf(rfd, "\033[2J\033[H");
        tty[LOC_TTY].c_lflag &= ~(ECHO | ECHOE);
        tcsetattr(lfd, TCSANOW, &tty[LOC_TTY]);
    }
    else
    {
        *fd = -1;
    }
}

void cls(int fd)
{
    dprintf(fd, "\033[2J\033[H"); /* Limpiar pantalla y mover cursor al inicio */
}

void closeFD(int lfd, int rfd, struct termios *tty)
{
    resetCanon(tty, lfd);
    resetCanon(tty, rfd);
    tcflush(lfd, TCIOFLUSH);                /*Vacia buffers de entrada y salida*/
    tcflush(rfd, TCIOFLUSH);
    tcsetattr(lfd, TCSANOW, &tty[O_L_TTY]); /* Configura el puerto serie con los parametros originales*/
    tcsetattr(rfd, TCSANOW, &tty[O_R_TTY]); 
    close(lfd);                             /*Cierre del puerto serie*/
    close(rfd);                             
}

