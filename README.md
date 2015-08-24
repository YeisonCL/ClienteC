ClienteC
===========
## Descripción
Cliente que descarga un recurso de un sitio web hecho en el lenguaje C.

##Running
Compilar usando gcc y ejecutar con la opciones disponibles.

```
gcc -Wall -std=c99 httpclient.c -o httpclient
./httpclient
```

##Options

###-u
Define la URL a la cual se desea hacer un GET para descargar un recurso.

```
./httpclient -u http://www.google.com
```

##Notes
- El cliente no muestra en pantalla el resultado del GET realizado al URL dado, solo realiza la petición.
