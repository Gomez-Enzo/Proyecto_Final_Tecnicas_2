#include "../inc/inc.h"

pthread_mutex_t mut;
int stop = 0;

int checkStop(pthread_mutex_t *mut, int *stop_flag)
{

    int stop = 0;
    pthread_mutex_lock(mut);
    if (*stop_flag == 1)
    {
        stop = 1;
    }

    pthread_mutex_unlock(mut);
    return stop;
}

void *keyInput(void *args)
{
    hilo_arg_t *seq = (hilo_arg_t *)args;

    // setNCanon(seq->tty, seq->fd);

    char c;
    while (1)
    {
        if (checkStop(&mut, &stop))
            break;
        read(seq->fd, &c, 1); // Lee un carácter
        switch (c)
        {
        case 'q':
            if (checkStop(&mut, &stop))
                break;
            pthread_mutex_lock(&mut);
            stop = 1; // Actualiza la bandera para detener la secuencia
            pthread_mutex_unlock(&mut);
            break;
        case '\x1b':
            read(seq->fd, &c, 1);
            read(seq->fd, &c, 1);
            switch (c)
            {
            case SPEED_UP_CHAR:
                if (checkStop(&mut, &stop))
                    break;
                pthread_mutex_lock(&mut);
                seq->vel++; // Actualiza la bandera para detener la secuencia
                pthread_mutex_unlock(&mut);
                break;
            case SPEED_DOWN_CHAR:
                if (checkStop(&mut, &stop))
                    break;
                pthread_mutex_lock(&mut);
                seq->vel--; // Actualiza la bandera para detener la secuencia
                if(seq->vel <= 1)
                    seq->vel = 1;
                pthread_mutex_unlock(&mut);
                break;
            default:
                break;
            }
            break;
        default:
            if (checkStop(&mut, &stop))
                break;
            break;
        }
    }

    // resetCanon(seq->tty, seq->fd);
    return NULL;
}

void seq1(hilo_arg_t *args)
{
    unsigned char bits = 0b10000000;
    while (1)
    {

        if (checkStop(&mut, &stop))
            break;

        // Secuencia de luces
        for (int i = 0; i < 7; i++)
        {
            if (checkStop(&mut, &stop))
                break;

            cls(args->fd);
            pthread_mutex_lock(&mut);
            dprintf(args->fd, "...Se ejecuta ");
            dprintf(args->fd, "%s...\n\n", args->name); // Limpia la pantalla
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(bits >> i, args->fd); // Enciende los LEDs
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel); // Retardo de 100 ms
            pthread_mutex_unlock(&mut);
        }

        for (int i = 7; i >= 1; i--)
        {
            if (checkStop(&mut, &stop))
                break;

            cls(args->fd);
            pthread_mutex_lock(&mut);
            dprintf(args->fd, "...Se ejecuta ");
            dprintf(args->fd, "%s...\n\n", args->name); // Limpia la pantalla
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(bits >> i, args->fd); // Enciende los LEDs
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel); // Retardo de 100 ms
            pthread_mutex_unlock(&mut);
        }
    }
    stop = 0;
    off(args->fd); // Apaga los LEDs
}
void seq2(hilo_arg_t *args)
{

    while (1)
    {
        if (checkStop(&mut, &stop))
            break;
        unsigned char bits = 0b10000000;
        unsigned char bits2 = 0b00000000;
        for (int j = 8; j >= 0; j--)
        {
            for (int i = 0; i < j; i++)
            {
                cls(args->fd);
                if (checkStop(&mut, &stop))
                    break;
                dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
                dprintf(args->fd, "Presione q para volver al menu\n\n");
                encenderLeds(bits2 | bits >> i, args->fd);
                dprintf(args->fd, "Tiempo: %d\n", args->vel);
                nbd(args->vel);
                if (i == j - 1)
                {
                    bits2 = bits2 | bits >> i;
                }
            }
        }
    }
    stop = 0;
    off(args->fd);
}
void seq3(hilo_arg_t *args)
{
    char tabla[6] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42};

    while (1)
    {
        if (checkStop(&mut, &stop))
            break;
        for (int i = 0; i < 6; i++)
        {
            cls(args->fd);
            if (checkStop(&mut, &stop))
                break;
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(tabla[i], args->fd);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel);
        }
    }
    stop = 0;
    off(args->fd);
}
void seq4(hilo_arg_t *args)
{

    char tabla[16] = {0b10000000, 0b10000000, 0b01000000, 0b01000000, 0b00100000, 0b00100000, 0b00010000, 0b00010000, 0b10001000, 0b01001000, 0b00100100, 0b00010100, 0b00001010, 0b00000110, 0b00000011, 0b00000001};

    while (1)
    {

        if (checkStop(&mut, &stop))
            break;
        for (int i = 0; i < 16; i++)
        {
            if (checkStop(&mut, &stop))
                break;
            cls(args->fd);
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(tabla[i], args->fd);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel);
        }
    }
    stop = 0;
    off(args->fd);
}
void seq5(hilo_arg_t *args)
{
    char table[9] = {0b00000000, 0b10000000, 0b00100000, 0b00001000, 0b00000010, 0b00000001, 0b00000100, 0b00010000, 0b01000000};

    while (1)
    {
        if (checkStop(&mut, &stop))
            break;
        for (int i = 0; i < 9; i++)
        {
            cls(args->fd);
            if (checkStop(&mut, &stop))
                break;
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(table[i], args->fd);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel);
            cls(args->fd);
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(table[0], args->fd);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(10);
        }
    }
    stop = 0;
    off(args->fd);
}
void seq6(hilo_arg_t *args)
{
    unsigned char bits = 0b10000000;

    while (1)
    {
        int par = 0;
        int impar = 7;

        if (checkStop(&mut, &stop))
            break;
        for (int i = 0; i < 8; i++)
        {
            cls(args->fd);
            if (checkStop(&mut, &stop))
                break;
            if ((i % 2) == 0)
            {
                dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
                dprintf(args->fd, "Presione q para volver al menu\n\n");

                encenderLeds(bits >> par, args->fd);
                dprintf(args->fd, "Tiempo: %d\n", args->vel);
                par++;
            }
            else
            {
                dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
                dprintf(args->fd, "Presione q para volver al menu\n\n");

                encenderLeds(bits >> impar, args->fd);
                dprintf(args->fd, "Tiempo: %d\n", args->vel);

                impar--;
            }
            nbd(args->vel);
        }
    }
    stop = 0;
    off(args->fd);
}
void seq7(hilo_arg_t *args)
{

    while (1)
    {
        if (checkStop(&mut, &stop))
            break;

        for (int i = 0; i < 8; i++)
        {
            if (checkStop(&mut, &stop))
                break;
            cls(args->fd);
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(0b10000000, args->fd);
            pthread_mutex_lock(&mut);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            pthread_mutex_unlock(&mut);
            cls(args->fd);
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(0b11111111 << i, args->fd);
            pthread_mutex_lock(&mut);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel);
            pthread_mutex_unlock(&mut);
        }
    }
    stop = 0;
    off(args->fd);
}

void seq8(hilo_arg_t *args)
{
    char table[2] = {0b11110000, 0b00001111};

    while (1)
    {

        if (checkStop(&mut, &stop))
            break;
        for (int i = 0; i < 2; i++)
        {
            if (checkStop(&mut, &stop))
                break;
            cls(args->fd);
            dprintf(args->fd, "...Se ejecuta %s...\n\n", args->name);
            dprintf(args->fd, "Presione q para volver al menu\n\n");
            encenderLeds(table[i], args->fd);
            dprintf(args->fd, "Tiempo: %d\n", args->vel);
            nbd(args->vel);
        }
    }
    stop = 0;
    off(args->fd);
}

void *pageVelocidadInicial(void *arg)
{
    hilo_arg_t *args = (hilo_arg_t *)arg;
    while (1)
    {
        if (checkStop(&mut, &stop))
            break;
        int cuentas = setVelocidadInicial() != -1 ? setVelocidadInicial() : 100;
        // float velocidadPorcentaje = (cuentas * (VEL_MAX_INICIAL + 1)) / 26367;
        float velocidadPorcentaje = velMs(cuentas);
        cls(args->fd);
        dprintf(args->fd, "...Se esta configurando la velocidad inicial...\n\n");
        dprintf(args->fd, "\tVelocidad Inicial: ----------->%.2f\n\n", velocidadPorcentaje);
        dprintf(args->fd, "Presione q para volver al menu\n\n");

        args->vel = (int)velocidadPorcentaje ? (int)velocidadPorcentaje : VEL_INICIAL;
    }
    stop = 0;
    return NULL;
}
