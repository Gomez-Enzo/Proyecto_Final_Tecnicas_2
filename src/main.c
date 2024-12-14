#include "../inc/inc.h"
int main(void)
{

    pthread_t hilos[2];
    hilo_arg_t seqs[NUM_FUNCS];
    struct termios tty[4];

    int velInicial = VEL_INICIAL;
    void (*funcs[NUM_SEQS])(hilo_arg_t *) = {seq1, seq2, seq3, seq4, seq5, seq6, seq7, seq8};
    int fd, lfd, rfd, opt_num;
    char opt;
    int quit = 0;
    
    adcInit();
    
    lfd = initLocal(tty);
    rfd = initRemote(tty);
    
    fd = lfd;
    while (quit == 0)
    {
        printModeMenu(fd);
        setMode(&fd, tty, lfd, rfd);
        if (fd == -1)
        {
            cls(lfd);
            cls(rfd);
            closeFD(lfd, rfd, tty);
            exit(1);
        }

        dprintf(fd, "%d", fd);
        initWiringPi(fd);
        for (int i = 0; i < NUM_FUNCS; i++)
        {
            seqs[i].name = funcNames[i];
            seqs[i].id = i;
            seqs[i].func = funcs[seqs[i].id];
            seqs[i].vel = velInicial;
            seqs[i].fd = fd;
        }
        setNCanon(tty, fd);
        if (checkPwd(fd) == -1)
            quit = 1;
        resetCanon(tty, fd);

        while (quit == 0)
        {
            printSeqMenu(seqs, fd);
            read(fd, &opt, 1);
            tcflush(fd, TCIFLUSH);
            opt_num = opt - '0';
            switch ((int)opt_num)
            {
            case 0:
                cls(lfd);
                cls(rfd);
                closeFD(lfd, rfd, tty);
                exit(1);
                break;
            case 1 ... NUM_SEQS:
                setNCanon(tty, fd);
                execFunc(hilos, seqs, opt_num - 1);
                resetCanon(tty, fd);
                break;
            case NUM_SEQS + 1:
                hilo_arg_t velIArgs;
                velIArgs.fd = fd;
                setNCanon(tty, fd);
                if (pthread_create(&hilos[0], NULL, keyInput, &velIArgs) != 0)
                    break;
                if (pthread_create(&hilos[1], NULL, pageVelocidadInicial, &velIArgs) != 0)
                    break;
                pthread_join(hilos[0], NULL);
                pthread_join(hilos[1], NULL);
                for (int i = 0; i < NUM_SEQS; i++)
                {
                    seqs[i].vel = velIArgs.vel;
                }
                resetCanon(tty, fd);
                break;
            case 113 - 48:
                quit = 1;
            default:
                break;
            }
        }
        quit = 0;
    }
    cls(lfd);
    cls(rfd);
    closeFD(lfd, rfd, tty);
    return 0;
}
