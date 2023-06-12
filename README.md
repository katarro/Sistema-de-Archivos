# Mi Terminal en C++

Este es un proyecto de un terminal sencillo realizado en C++. El terminal puede ejecutar varios comandos básicos de Unix como `ls`, `pwd`, `mkdir`, `rm` y `cd`, y tiene soporte para los modificadores `-R`, `-i` y `-r` para algunos de los comandos.

## Comandos soportados

### `ls`

El comando `ls` lista todos los archivos y directorios en el directorio actual. Este comando también soporta los modificadores `-R` para listar los contenidos de manera recursiva, y `-i` para mostrar los inodos de los archivos.

### `pwd`

El comando `pwd` muestra el directorio de trabajo actual.

### `mkdir`

El comando `mkdir` crea un nuevo directorio en el directorio actual.

### `rm`

El comando `rm` elimina un archivo o un directorio. Este comando también soporta el modificador `-r` para eliminar directorios y sus contenidos de manera recursiva.

### `cd`

El comando `cd` cambia el directorio de trabajo actual a un directorio especificado.

## Uso

Para compilar el programa, debes tener instalado un compilador de C++ compatible con C++17 o superior.

Para compilar el programa, ejecuta:

```sh
g++ tarea3.cpp -o my_terminal
```

Para ejecutar el terminal, utiliza el siguiente comando:

```sh
./my_terminal
```

Una vez que el terminal esté en ejecución, puedes utilizar los comandos mencionados anteriormente.

