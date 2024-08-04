// #include <stdio.h>
// #include "comunicacionSerial.h"

// char* sendMessage(float valorPot, int sensor, const char* valor) {
//     static char cadena[BUFFER_SIZE];  // Usa un buffer estático para la cadena
//     char buffer[BUFFER_SIZE];
//     snprintf(buffer, sizeof(buffer), "%4.1f", valorPot);

//     // Asegúrate de limpiar el buffer antes de usarlo
//     memset(cadena, 0, sizeof(cadena));

//     if (strcmp(valor, "gb") == 0) {
//         switch (sensor) {
//         case 1:
//             strcpy(cadena, "page2.NmGrIzEx.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "\"\xFF\xFF\xFF");
//             break;
//         case 2:
//             strcpy(cadena, "page2.NmGrIzIn.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "\"\xFF\xFF\xFF");
//             break;
//         case 3:
//             strcpy(cadena, "page2.NmGrDeIn.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "\"\xFF\xFF\xFF");
//             break;
//         case 4:
//             strcpy(cadena, "page2.NmGrDeEx.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "\"\xFF\xFF\xFF");
//             break;
//         default:
//             return NULL;  // Retorna NULL si el sensor no es válido
//         }
//         return cadena;
//     } else if (strcmp(valor, "gm") == 0) {
//         switch (sensor) {
//         case 1:
//             strcpy(cadena, "page1.NmAlineacion.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "mm\"\xFF\xFF\xFF");
//             break;
//         case 2:
//             strcpy(cadena, "page1.NmPeralte.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "mm\"\xFF\xFF\xFF");
//             break;
//         case 3:
//             strcpy(cadena, "page1.NmNivelIz.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "mm\"\xFF\xFF\xFF");
//             break;
//         case 4:
//             strcpy(cadena, "page1.NmNivelDe.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "mm\"\xFF\xFF\xFF");
//             break;
//         case 10:
//             strcpy(cadena, "page1.NmDistancia.txt=\"");
//             strcat(cadena, buffer);
//             strcat(cadena, "m\"\xFF\xFF\xFF");
//             break;
//         default:
//             return NULL;  // Retorna NULL si el sensor no es válido
//         }
//         return cadena;
//     }

//     return NULL;  // Retorna NULL si `valor` no es "gb" o "gm"
// }