| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# 🚧 Proyecto Bateadora - Sistema de Medición y Monitoreo de Vía

Sistema embebido para la adquisición, visualización y transmisión de datos de geometría de vía ferroviaria, utilizando ESP32 y comunicación MQTT hacia la nube.

---

## 📌 Descripción General

Este proyecto implementa un sistema de monitoreo para una máquina bateadora, encargado de:

- 📡 Recopilar datos de la geometría de la vía
- 📊 Visualizar en tiempo real los valores de sensores y grupos de bajada
- ☁️ Enviar información a un servidor en la nube mediante protocolo MQTT

El sistema está diseñado para operar en entornos industriales, permitiendo trazabilidad del estado de la vía antes, durante y después del proceso de mantenimiento.

---

## 🧠 Arquitectura del Sistema

El sistema está compuesto por:

- 🔹 **ESP32**: Unidad central de procesamiento y comunicación
- 🔹 **Pantalla táctil**: Interfaz gráfica de usuario (HMI)
- 🔹 **Sensores de medición**: Captura de datos de la vía
- 🔹 **Broker MQTT**: Comunicación con la nube

La pantalla táctil se comunica con el ESP32 mediante **puerto serial**, actuando como interfaz de operación.

---

## 🔌 Comunicación y Protocolo

- **Protocolo:** MQTT
- **Broker:** Configurable
- **Topic de suscripción:**

Bateadora/201


### Flujo de conexión:

1. El usuario presiona el botón **"Conectar"** en la pantalla
2. El sistema se suscribe al broker MQTT
3. Se habilita la interacción con sensores

---

## ⚙️ Flujo de Operación

### 1. Conexión
- Presionar botón **"Conectar"**
- Suscripción al topic MQTT

### 2. Selección de trabajo
- Presionar botón **"Sensores"**
- Se envía el mensaje:

"trabajo"

- El sistema responde con un ID de trabajo

### 3. Selección de recorrido

Antes de iniciar la medición, se debe seleccionar el tipo de recorrido:

- **R1:** Estado inicial de la vía (antes del mantenimiento)
- **R2:** Medición durante el proceso de trabajo
- **R3:** Estado final de la vía (post mantenimiento)

### 4. Medición y transmisión
- Lectura de sensores
- Visualización en HMI
- Envío de datos al servidor en la nube

---

## 📊 Características Principales

- ✔ Monitoreo en tiempo real
- ✔ Interfaz táctil intuitiva
- ✔ Comunicación IoT mediante MQTT
- ✔ Arquitectura modular basada en ESP-IDF
- ✔ Separación por componentes reutilizables

---

## 🗂️ Estructura del Proyecto


proyectoBateadora/
│
├── build/ # Archivos de compilación
│
├── main/ # Punto de entrada
│ ├── CMakeLists.txt
│ └── main.c
│
├── components/
│ ├── comunicacionWiFi/ # Manejo de WiFi y MQTT
│ │ ├── include/
│ │ │ └── comunicacionWiFi.h
│ │ ├── comunicacionWiFi.c
│ │ ├── certificados/
│ │ │ ├── ESP32_1.crt
│ │ │ ├── ESP32_1.key
│ │ │ └── rootCA.crt
│ │ └── CMakeLists.txt
│ │
│ ├── grupoBateo/ # Control del sistema de bateo
│ │ ├── include/
│ │ │ └── grupoBateo.h
│ │ ├── grupoBateo.c
│ │ └── CMakeLists.txt
│ │
│ └── grupoMedicion/ # Adquisición de datos
│ ├── include/
│ │ └── grupoMedicion.h
│ ├── grupoMedicion.c
│ └── CMakeLists.txt


---

## 🛠️ Tecnologías Utilizadas

- **Microcontrolador:** ESP32
- **Framework:** ESP-IDF
- **Lenguaje:** C
- **Comunicación:** UART (HMI) + MQTT (Cloud)
- **Seguridad:** Certificados TLS

---

## 🚀 Instalación y Ejecución

1. Clonar repositorio:
```bash
git clone https://github.com/ale-12121986/proyectoBateadora.git


## 📈 Aplicaciones
🚆 Mantenimiento ferroviario
🏭 Automatización industrial
🌐 Sistemas IoT distribuidos
📊 Monitoreo remoto de infraestructura
🧪 Estado del Proyecto

## 🟡 Prototipo funcional en desarrollo
