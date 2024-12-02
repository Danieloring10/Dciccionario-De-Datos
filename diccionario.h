#ifndef DICCIONARIO_H
#define DICCIONARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nomEnt[20];
    long cabAtr;
    long cabDatos;
    long sigEnt;
} ENT;

typedef struct {
    char nomAtr[20];
    int tipo; // 1. char, 2. int, 3. float, 4. cadena
    int tam;
    long sigAtr;
} ATR;

typedef struct {
    long sig;
    long dat;
} CDat;
// 
int aperturaDiccionario(FILE **arch, char modoAp[], char nomArch[]);
void abrirDiccionario(FILE *arch);
void creaArch(FILE **arch, char nombre[]);
void creaDiccinario(FILE *arch);
void menuPrincipal(FILE *arch);
void reporteDiccionario(FILE *arch);
void menuDatos(FILE *arch);

// Prototipos para entidades
int buscarEntidad(FILE *arch, char nombre[]);
ENT crearEntidad(char nombre[]);
void altaEntidad(FILE *arch);
void consultarEntidad(FILE *arch);
void bajaEntidad(FILE *arch);
void cambiarNombreEntidad(FILE *arch);
void reporteEntidades(FILE *arch);
void imprimirEntidad(ENT entidad);
void menuEntidad(FILE *arch);

// Prototipos para atributos
int buscarAtributo(FILE *arch, char nombAtr[], long cab_atr);
ATR crearAtributos(char nomAtr[], int tipo);
void altaAtributo(FILE *arch, long pos_ent);
void consultarAtributos(FILE *arch, long pos_ent);
void bajaAtributo(FILE *arch, long pos_ent);
void cambiarNombreAtributo(FILE *arch, long pos_ent);
void reporteAtributos(FILE *arch, long pos_ent);
void imprimirAtributo(ATR atributo);
void menuAtributos(FILE *arch);

// Prototipos para datos
void altaDatos(FILE *arch, long cab_datos, long cab_atr);
void consultarDatos(FILE *arch, long cab_datos, long cab_atr);
void bajaDatos(FILE *arch, long cab_datos);
void cambiarDatos(FILE *arch, long cab_datos, long cab_atr);
void reporteDatos(FILE *arch, long cab_datos, long cab_atr);
void imprimirTodosLosDatos(FILE *arch, CDat cab_dato, long cab_atr);
void imprimeUnDato(void *dato, int tipo);
int buscarDatos(FILE *arch, long cab_datos, char nombre[]);
#endif
