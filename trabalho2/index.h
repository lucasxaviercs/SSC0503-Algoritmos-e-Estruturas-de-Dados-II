#pragma once

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>

    #define TAM_INDEX_HEADER 1
    #define TAM_INDEX_REGISTRO 8
  
    typedef struct {
        char status; // indica a consistência do arquivo de dados
    } IndexHeader;

    typedef struct {
        int codEstacao; // código único que identifica a estação
        int RRN; // RRN do registro correspondente ao código da estação
    } IndexRegistro;
