# Sistema de recuperación - Aviónica AAIAQ

## 1. Información del proyecto

- **Proyecto:** Reto de reclutamiento AAIAQ 2026
- **División:** Aviónica
- **Ruta:** 3 - Firmware y sistemas embebidos
- **Enfoque:** Sistemas de control
- **Lenguaje:** C/C++
- **Plataforma:** Arduino
- **Simulador:** Wokwi

## 2. Objetivo

Desarrollar un sistema de control basado en una máquina de estados finitos para simular la lógica de recuperación de un vehículo aeroespacial, mediante la gestión de eventos de entrada, temporizadores no bloqueantes e indicadores visuales.

## 3. Descripción del proyecto

El firmware implementa una máquina de estados finitos de cinco estados: IDLE, ARMED, ASCENT, DEPLOYMENT y LANDED.

El sistema utiliza un botón como entrada de control, diferenciando entre pulsaciones cortas y largas. A partir de estos eventos, gestiona las transiciones entre estados y activa una salida de ignición simulada.

También incorpora un temporizador de respaldo de 8 segundos para ejecutar el despliegue cuando no se detecta manualmente el apogeo.

## 4. Funcionalidades

- Máquina de estados finitos de cinco estados.
- Detección de pulsaciones cortas y largas.
- Sistema de antirrebote por software.
- Temporizadores no bloqueantes mediante `millis()`.
- Indicadores LED para identificar los estados del sistema.
- Pulso de ignición de 1000 ms, ejecutado una sola vez.
- Temporizador de respaldo para el despliegue.
- Registro serial de transiciones y eventos ignorados con marca de tiempo.

## 5. Herramientas utilizadas

| Herramienta | Aplicación |
|---|---|
| Visual Studio Code | Edición del código |
| Arduino | Plataforma de programación |
| Wokwi | Simulación del sistema |
| Git | Control de versiones |
| GitHub | Alojamiento del repositorio |
| LaTeX | Elaboración del reporte técnico |

## 6. Ejecución y reproducción

1. Clonar el repositorio:

   `https://github.com/RitoMartin38/avionica-reto-2026.git`

2. Abrir el proyecto en Visual Studio Code.

3. Abrir el código fuente ubicado en `src/main.cpp`.

4. Abrir el circuito correspondiente en Wokwi:(https://wokwi.com/projects/481283162667649025)

5. Iniciar la simulación.

6. Interactuar con el botón para generar pulsaciones cortas y largas.

7. Observar los indicadores LED y verificar las transiciones de estado.

8. Consultar el monitor serial para visualizar los eventos registrados y sus marcas de tiempo.

## 7. Evidencias

El diagrama de estados y las evidencias complementarias se encuentran en la carpeta `media/`.

## 8. Autor

- **Nombre:** Rito Martín Jimenez Martinez
- **Carrera:** Ingeniería Aeroespacial
- **Semestre:** Primero

---

**Reto de Reclutamiento 2026 - División de Aviónica | AAIAQ**