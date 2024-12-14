#include "../inc/inc.h"

void adcInit(void)
{
    wiringPiSetup();

    if (ads1115Setup(BASE, ADC_ADDR) == -1)
    {
        printf("Error al inicializar el dispositivo I2C\n");
    }
}

int setVelocidadInicial(void)
{

    int cuentas;

    cuentas = analogRead(BASE);

    if (cuentas == -1)
    {
        printf("Error al leer el valor del ADC\n");
        return -1;
    }

    return cuentas;
}
