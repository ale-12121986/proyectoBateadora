| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# PproyectoBateadora
## informacion general
Este sistema se va a encargar de recopilar todos los datos de la geometria de la via y enviarlos por protocola MQTT a un servidor en la nube.
Consta de una pantalla tactil en la cual se va a a ver los valores de los transmisores y de los grupos de bajada.

La pantalla tactil se coomunica por puerto serial al ESP32, el cual va a servir de interfaz grafica poder interactuar, tiene un boton con el nombre de Conectar cuando se presione se va a suscribir al broker mqtt con el topic Bateadora/201 al estar conectado tienes que precionar el boton de sensores al precionar ese boton, se envia el mensaje "trabajo" y te va a regresar el ide trabajo y asi se tiene identificado el que trabajo se va a realizare, antes de empezar a medir tienes que elegir que recorrido va a realizar:
R1. El primer recorrido para ver como esta la via antes de realizar el trabajo de mejoramiento. 
R2. El segundo recorrido que se hace cuando se esta trabajando.
R3. El tercer recorrido es el que va a mostrar como quedo la via despues del trabajo de mejoramiento.



## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
proyectoBateadora/
│
├── build/
│
├── main/
│    ├── CMakeLists.txt
│    └── main.c
└── components
     ├── comunicacionWiFi/
     │     ├── include/
     │     |     └── comunicacionWiFi.h
     │     ├── ESP32_1.crt
     │     ├── rootCA.crt
     │     ├── ESP32_1.key
     │     ├── CMakeLists.txt
     │     ├── components.mk
     │     └── comunicacionWiFi.c
     ├── grupoBateo/
     │     ├── include/
     │     │    └── grupoBateo.h
     │     ├── CMakeLists.txt
     │     └── grupoBateo.c
     └── grupoMedicion/
           ├── include/
           |     └── grupoMedicion.h
           ├── CMakeLists.txt
           └── grupoMedicion.c

```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
