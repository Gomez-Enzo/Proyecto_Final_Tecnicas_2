#include "../inc/inc.h"

const char leds[8] = {26, 21, 20, 16, 12, 25, 24, 23};
char *funcNames[NUM_SEQS] = {
    "Auto Fantástico",
    "Apilada",
    "Choque",
    "Carrera", "Salto", "ZigZag", "Vumetro", "Baliza"};

void setNCanon(struct termios *tty, int fd)
{
    if (fd == FD_STDIN)
    {
        tty[LOC_TTY].c_lflag = 0;              // anula entrada canónica y eco
        tcsetattr(fd, TCSANOW, &tty[LOC_TTY]); // actualiza con los valores nuevos de la config. TCSANOW = activar la modificación inmediatamente
    }
    else
    {
        tty[REM_TTY].c_lflag = 0;              // anula entrada canónica y eco
        tcsetattr(fd, TCSANOW, &tty[REM_TTY]); // actualiza con los valores nuevos de la config. TCSANOW = activar la modificación inmediatamente
    }
}

void resetCanon(struct termios *tty, int fd)
{
    if (fd == FD_STDIN)
    {
        tty[LOC_TTY].c_lflag |= (ICANON | ECHO | ECHOE); // anula entrada canónica y eco
        tcsetattr(fd, TCSANOW, &tty[LOC_TTY]);           // actualiza con los valores nuevos de la config. TCSANOW = activar la modificación inmediatamente
    }
    else
    {
        tty[REM_TTY].c_lflag |= (ICANON | ECHO | ECHOE); // anula entrada canónica y eco
        tcsetattr(fd, TCSANOW, &tty[REM_TTY]);           // actualiza con los valores nuevos de la config. TCSANOW = activar la modificación inmediatamente
    }
}

int checkPwd(int fd)
{
    char clave[MAX_LEN + 1];            // Array para almacenar la clave ingresada
    char contrasena[MAX_LEN + 1] = PWD; // Contraseña correcta
    char c;
    int i, resultado;

    cls(fd);
    tcflush(fd, TCIFLUSH);
    for (i = 0; i < MAX_INTENTOS; i++)
    {

        dprintf(fd, "Ingrese su contraseña: ");
        int pos = 0; // Índice para la clave

        while (1)
        {
            readChar(&c, fd); // Leer un carácter

            if (c == '\n')
            {                      // Detectar Enter
                clave[pos] = '\0'; // Terminar la cadena
                break;             // Salir del bucle si se presiona Enter
            }
            else if (c == 127 || c == '\b')
            { // Detectar Backspace
                if (pos > 0)
                {
                    pos--;                // Disminuir índice
                    dprintf(fd, "\b \b"); // Mover el cursor atrás y borrar el asterisco
                }
            }
            else
            {
                if (pos < MAX_LEN)
                {                     // Asegurar que no se exceda la longitud máxima
                    clave[pos++] = c; // Agregar el carácter a la clave
                    dprintf(fd, "*"); // Imprimir un asterisco en la pantalla
                }
            }
        }
        dprintf(fd, "\n");

        // Comparar la clave ingresada con la contraseña
        resultado = strcmp(clave, contrasena);
        if (resultado == 0)
        {
            dprintf(fd, "Bienvenido\n");
            break; // Salir del bucle de intentos
        }
        else
        {
            cls(fd);
            dprintf(fd, "Contraseña incorrecta\n");
        }

        if (i == MAX_INTENTOS - 1)
        {
            dprintf(fd, "Ha superado el límite de intentos\n");
            return -1;
        }
    }
    return 0;
}

// funcion para iniciar libreria y pines
void off(int fd)
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(leds[i], 0);
    }
    dprintf(fd, "\n");
}

void initWiringPi(int fd)
{
    wiringPiSetupGpio();
    for (int i = 0; i < 8; i++)
    {
        pinMode(leds[i], OUTPUT);
    }
    off(fd);
}

// funcion para encender leds con tabla
void encenderLeds(char bits, int fd)
{
    dprintf(fd, "\t");
    for (int i = 0; i < 8; i++)
    {
        if (bits & (1 << (7 - i)))
        {
            dprintf(fd, "[#]");
            digitalWrite(leds[i], 1);
        }
        else
        {
            dprintf(fd, "[ ]");
            digitalWrite(leds[i], 0);
        }
    }
    dprintf(fd, "\n");
}

void printSeqMenu(hilo_arg_t *seqs, int fd)
{
    cls(fd);
    dprintf(fd, "--------------------------------\n");
    dprintf(fd, "              Menu              \n");
    dprintf(fd, "--------------------------------\n");
    for (int i = 0; i < NUM_SEQS; i++)
    {
        dprintf(fd, "%d-------->%s\n", i + 1, seqs[i].name);
    }
    dprintf(fd, "--------------------------------\n");
    dprintf(fd, "%d-------->Configurar velocidad\n", NUM_SEQS + 1);
    dprintf(fd, "q-------->Cambiar modo\n");
    dprintf(fd, "0-------->Salir\n");
    dprintf(fd, "--------------------------------\n");
    dprintf(fd, "Seleccione una secuencia: ");
}

void nbd(int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;

    while (nanosleep(&ts, &ts) && errno == EINTR)
    {
        // Si es interrumpido por una señal, reintenta el retardo restante
        if (checkStop(&mut, &stop))
            break;
        else
            continue;
    }
}

void *hilo_func(void *arg)
{
    hilo_arg_t *args = (hilo_arg_t *)arg;
    args->func(args);

    return NULL;
}

void execFunc(pthread_t *hilos, hilo_arg_t *args, int id)
{
    if (pthread_create(&hilos[0], NULL, keyInput, &args[id]) != 0)
        return;
    if (pthread_create(&hilos[1], NULL, hilo_func, &args[id]) != 0)
        return;
    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
}

void printModeMenu(int fd)
{
    cls(fd);
    dprintf(fd, "--------------------------------\n");
    dprintf(fd, "       Modos de ejecucion       \n");
    dprintf(fd, "--------------------------------\n");
    dprintf(fd, "1-------->Local\n");
    dprintf(fd, "2-------->Remoto\n");
    dprintf(fd, "--------------------------------\n");
    dprintf(fd, "0-------->Salir\n");
    dprintf(fd, "Seleccione un modo: ");
}