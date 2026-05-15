# SSOOIIGLE

Buscador multihilo de palabras en ficheros de texto — Práctica 3 de Sistemas Operativos II.

SSOOIIGLE simula un motor de búsqueda concurrente donde múltiples clientes (hilos) buscan palabras simultáneamente sobre una colección de libros en texto plano, compitiendo por un número limitado de réplicas del sistema y pagando por los resultados si son usuarios Premium.

---

## Características

- **Búsqueda paralela por fragmentos**: cada libro se divide en tantos fragmentos como núcleos hardware haya; cada fragmento es procesado por un hilo `Buscador` independiente.
- **Tres tipos de usuario**: Gratuito (límite fijo de resultados), Premium (créditos por resultado con recarga automática) y Premium Ilimitado (sin restricciones).
- **Control de acceso con equidad**: un `Monitor` gestiona el acceso a las réplicas del sistema aplicando una política **8:2** (máximo 8 Premium y 2 Gratuitos por cada ciclo de 10 turnos) para evitar la inanición de los usuarios Gratuitos.
- **Sistema de pagos como hilo dedicado**: `SistemaPago` opera como productor-consumidor singleton; los clientes Premium que agotan su saldo se bloquean hasta que la recarga es confirmada.
- **Informe final detallado**: al terminar la simulación se muestra por cada cliente el tiempo total, número de resultados y cada ocurrencia con su contexto (palabra anterior y posterior, línea y rango porcentual del hilo que la encontró).

---

## Estructura del proyecto

```
SSOOIIGLE/
├── src/                        # Implementaciones (.cpp)
│   ├── SSOOIIGLE.cpp           # main: arranque y orquestación de la simulación
│   ├── Cliente.cpp             # Hilo de usuario; lanza buscadores y filtra resultados
│   ├── Buscador.cpp            # Búsqueda paralela por fragmento de fichero
│   ├── ResultadoBusqueda.cpp   # Value object con los datos de una ocurrencia
│   ├── Monitor.cpp             # Control de acceso con política de equidad 8:2
│   └── SistemaPago.cpp         # Hilo singleton de recarga de créditos Premium
├── inc/                        # Cabeceras (.hpp)
│   ├── Cliente.hpp
│   ├── Buscador.hpp
│   ├── ResultadoBusqueda.hpp
│   ├── Monitor.hpp
│   └── SistemaPago.hpp
├── data/                       # Libros en texto plano y diccionario de palabras
│   ├── diccionario.txt         # Palabras que se asignan aleatoriamente a los clientes
│   └── *.txt                   # Libros sobre los que se realiza la búsqueda
├── exec/                       # Ejecutable generado (creado por make)
├── obj/                        # Ficheros objeto intermedios (creados por make)
├── Makefile
└── README.md
```

---

## Requisitos

| Requisito | Versión mínima |
|-----------|---------------|
| g++ | 10 o superior |
| Estándar C++ | C++20 |
| Sistema operativo | Linux (POSIX threads) |
| `std::filesystem` | Incluido en C++17/20 |

---

## Compilación y ejecución

### Compilar
```bash
make
```

### Ejecutar
```bash
make solution
ó
./exec/SSOOIIGLE <num_clientes> <num_replicas>
```

| Parámetro | Descripción |
|-----------|-------------|
| `num_clientes` | Número total de usuarios que realizarán búsquedas (entero > 0) |
| `num_replicas` | Número máximo de clientes que pueden buscar simultáneamente (entero > 0) |

**Ejemplo** — 10 clientes compitiendo por 2 réplicas:
```bash
make solution     # equivale a ./exec/SSOOIIGLE 10 2
```

### Ejecutar con salida a fichero de depuración
```bash
make debug        # equivale a ./exec/SSOOIIGLE 10 2 > debug.txt
```

### Limpiar artefactos de compilación
```bash
make clean
```

---

## Arquitectura y diseño concurrente

### Diagrama de componentes

```
main()
 ├── SistemaPago (1 hilo dedicado, singleton)
 ├── Monitor     (compartido, controla nReplicas)
 └── Cliente × N (1 hilo por cliente)
       └── Buscador × (núcleos × libros)  (1 hilo por fragmento de libro)
             └── ResultadoBusqueda[]
```

### Flujo de una búsqueda

```
Cliente                Monitor              SistemaPago
   │                      │                      │
   │── esperarTurno() ──► │  (bloqueo si         │
   │                      │   no hay réplica      │
   │◄── acceso concedido ─│   o no es su turno)  │
   │                      │                      │
   │──── realizarBusqueda() ────────────────────►│
   │        (lanza N hilos Buscador)             │
   │        (si saldo==0 → solicitarPago()) ────►│
   │                                             │ (500ms simulados)
   │◄─────────────── saldo recargado (+10) ──────│
   │                                             │
   │── liberarTurno() ──► │                      │
   │                      │                      │
```

### Política de equidad del Monitor (ciclo 8:2)

Para evitar que los usuarios Premium monopolicen las réplicas:

- Cada ciclo de **10 turnos** permite pasar como máximo **8 Premium** y **2 Gratuitos**.
- Un Premium avanza si: hay réplica libre **y** (no hay Gratuitos esperando **o** el ciclo no ha consumido aún 8 Premium).
- Un Gratuito avanza si: hay réplica libre **y** (no hay Premium esperando **o** el ciclo ya ha consumido sus 8 Premium).
- Al completarse los 10 turnos, los contadores se reinician.

### Partición del fichero por offsets

En lugar de dividir por líneas (costoso: requiere contar todo el fichero), `Buscador::calcularOffsets` divide por **bytes** y ajusta cada punto de corte al final de la línea en curso, garantizando que ningún hilo parta una palabra a la mitad.

---

## Tipos de usuario

| Tipo | Probabilidad de aparición | Límite de resultados | Coste |
|------|--------------------------|----------------------|-------|
| `Gratuito` | 40 % | 10 resultados | Gratuito |
| `Premium` | 40 % | Ilimitado\* | 1 crédito por resultado |
| `PremiumIlimitado` | 20 % | Ilimitado | Gratuito |

\* Si el saldo llega a 0, el hilo se bloquea hasta que `SistemaPago` recargue 10 créditos.

---

## Formato del informe final

```
==========================================
       INFORME FINAL DE BÚSQUEDAS
==========================================

[Cliente 3] (Premium) Palabra: 'silencio'
   Tiempo de búsqueda: 1.243s | Resultados: 7
[Hilo 2 inicio:25 – final: 50] :: línea 134 :: … profundo [silencio] absoluto …
[Hilo 4 inicio:75 – final: 100] :: línea 287 :: … roto el [silencio] de la …
```

Cada línea de resultado indica:
- **Hilo N** — identificador del hilo Buscador que encontró la ocurrencia.
- **inicio / final** — rango porcentual del fichero que procesaba ese hilo.
- **línea** — número de línea dentro del fragmento.
- **contexto** — palabra anterior y posterior a la ocurrencia.

---

## Datos de prueba

El directorio `data/` incluye ocho libros en español (texto plano) y un `diccionario.txt` con palabras frecuentes que se asignan aleatoriamente a cada cliente al arrancar la simulación.

---

## Autor

**Jose Matías Ruiz Valero y Héctor García Rubio**  
Sistemas Operativos II — 2026
