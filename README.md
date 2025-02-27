# Proyecto Final Técnicas Digitales 2

Este proyecto consiste en el control de LEDs, ADC y pulsadores mediante una Raspberry Pi. El software está desarrollado en C, permitiendo el manejo de hilos y el control del puerto serie.

## Características

- **Control de LEDs:** Encendido y apagado de LEDs conectados a la Raspberry Pi.
- **Lectura de ADC:** Obtención de valores analógicos a través de un convertidor ADC.
- **Gestión de Pulsadores:** Detección de estados de pulsadores para interacción con el sistema.
- **Manejo de Hilos:** Implementación de multitarea para operaciones concurrentes.
- **Comunicación Serie:** Interacción con otros dispositivos mediante el puerto serie.

## Estructura del Proyecto

- `src/`: Contiene los archivos fuente en C.
- `inc/`: Incluye los archivos de cabecera.
- `Makefile`: Archivo para la compilación del proyecto.

## Requisitos

- Raspberry Pi con sistema operativo compatible.
- Compilador GCC instalado.
- Conexiones de hardware: LEDs, pulsadores y módulo ADC conectados a la Raspberry Pi.

## Instalación y Ejecución

1. **Clonar el repositorio:**
   ```bash
   git clone https://github.com/Gomez-Enzo/Proyecto_Final_Tecnicas_2.git
   cd Proyecto_Final_Tecnicas_2
2. **Compilar el proyecto:**
   make
3. **Ejecutar el programa:**
   ./nombre_del_ejecutable
