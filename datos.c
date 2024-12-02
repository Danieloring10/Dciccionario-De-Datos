#include "diccionario.h"

void altaDatos(FILE *arch, long cab_datos, long cab_atr) {
    CDat cab_dato;
    ATR atributo;
    long pos, pos_atr, pos_ant_dato, pos_ant_cab, tb = 0;
    long pos_dato;
    long pos_reset = -1;
    int primer_atributo = 1;
    void *dato;

    fseek(arch, cab_atr, SEEK_SET);
    fread(&pos_atr, sizeof(long), 1, arch);

    pos_ant_cab = cab_datos;

    fseek(arch, cab_datos, SEEK_SET);
    fread(&pos, sizeof(long), 1, arch);

    // Ultima cabecera de dato
    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        pos_ant_cab = ftell(arch);
        fread(&pos, sizeof(long), 1, arch);
    }

    fseek(arch, 0, SEEK_END);
    // Inicializar cabezera de dato nuevo
    cab_dato.dat = -1;
    cab_dato.sig = -1;

    pos = ftell(arch);

    fwrite(&cab_dato, sizeof(CDat), 1, arch);

    fseek(arch, pos_ant_cab, SEEK_SET);
    fwrite(&pos, sizeof(long), 1, arch);

    fseek(arch, pos + sizeof(long), SEEK_SET);
    pos_ant_dato = ftell(arch);

    // Leer atributo
    while (pos_atr != -1)
    {
        fseek(arch, pos_atr, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        pos_atr = atributo.sigAtr;

        tb = atributo.tam + sizeof(long);
        dato = malloc(tb);

        printf("Ingresa el valor de %s: ", atributo.nomAtr);
        switch (atributo.tipo)
        {
        case 1:

            scanf(" %c", (char *)dato);
            break;
        case 2:

            scanf("%d", (int *)dato);
            break;
        case 3:

            scanf("%f", (float *)dato);
            break;
        case 4:

            scanf(" %[^\n]", (char *)dato);
            break;
        }

        if (primer_atributo == 1)
        {
            if (buscarDatos(arch, cab_datos, (char *)dato) == 1)
            {
                printf("Ya existe un dato con el mismo nombre\n");
                fseek(arch, pos_ant_cab, SEEK_SET);
                fwrite(&pos_reset, sizeof(long), 1, arch);
                free(dato);
                system("pause");
                system("cls");
                return;
            }
            else
                primer_atributo = 0;
        }

        *((long *)((char *)dato + atributo.tam)) = -1;
        fseek(arch, 0, SEEK_END);
        pos_dato = ftell(arch);
        fwrite(dato, tb, 1, arch);

        // check
        fseek(arch, pos_ant_dato, SEEK_SET);
        fwrite(&pos_dato, sizeof(long), 1, arch);

        pos_ant_dato = pos_dato + atributo.tam;

        free(dato);
    }

    printf("Dato agregado correctamente\n");
    system("pause");
    system("cls");
}

void consultarDatos(FILE *arch, long cab_datos, long cab_atr) {
     CDat cab_dato;
    long pos, pos_dato;
    char nombre[20], nombre_dato[20];

    fseek(arch, cab_datos, SEEK_SET);
    fread(&pos_dato, sizeof(long), 1, arch);

    if (pos_dato == -1)
    {
        printf("No existen datos en la entidad\n");
        system("pause");
        system("cls");
        return;
    }

    printf("\nDame el nombre del dato a consultar: ");
    scanf(" %[^\n]", nombre);

    if (buscarDatos(arch, cab_datos, nombre) == 0)
    {
        printf("El dato con nombre %s no existe en la entidad\n", nombre);
        system("pause");
        system("cls");
        return;
    }

    cab_dato.sig = 0;
    while (cab_dato.sig != -1)
    {
        fseek(arch, pos_dato, SEEK_SET);
        fread(&cab_dato, sizeof(CDat), 1, arch);
        pos = cab_dato.dat;
        fseek(arch, pos, SEEK_SET);
        fread(nombre_dato, 20, 1, arch);

        if (strcmp(nombre, nombre_dato) == 0)
        {
            break;
        }
        pos_dato = cab_dato.sig;
    }

    imprimirTodosLosDatos(arch, cab_dato, cab_atr);
    system("pause");
    system("cls");
    printf("\n");
}

void bajaDatos(FILE *arch, long cab_datos) {
    CDat cab_dato;
    long pos, pos_dato, pos_cab_ant;
    char nombre[20], nombre_dato[20];

    pos_cab_ant = cab_datos;

    fseek(arch, cab_datos, SEEK_SET);
    fread(&pos_dato, sizeof(long), 1, arch);

    if (pos_dato == -1)
    {
        printf("No existen datos en la entidad\n");
        system("pause");
        system("cls");
        return;
    }

    printf("\nDame el nombre del dato a eliminar: ");
    scanf(" %[^\n]", nombre);

    if (buscarDatos(arch, cab_datos, nombre) == 0)
    {
        printf("El dato con nombre %s no existe en la entidad\n", nombre);
        system("pause");
        system("cls");
        return;
    }

    cab_dato.sig = 0;
    while (cab_dato.sig != -1)
    {
        fseek(arch, pos_dato, SEEK_SET);

        fread(&cab_dato, sizeof(CDat), 1, arch);
        pos = cab_dato.dat;
        fseek(arch, pos, SEEK_SET);
        fread(nombre_dato, 20, 1, arch);

        if (strcmp(nombre, nombre_dato) == 0)
            break;

        pos_cab_ant = pos_dato;
        pos_dato = cab_dato.sig;
    }

    pos_dato = cab_dato.sig;
    fseek(arch, pos_cab_ant, SEEK_SET);
    fwrite(&pos_dato, sizeof(long), 1, arch);

    printf("Eliminacion realizada correctamente\n");

    printf("\n");
    system("pause");
    system("cls");
}

void cambiarDatos(FILE *arch, long cab_datos, long cab_atr) {
     CDat cab_dato;
    ATR atributo;
    void *dato;
    long pos, pos_dato;
    char nombre[20], nombre_dato[20], nombre_atr[20];

    fseek(arch, cab_datos, SEEK_SET);
    fread(&pos_dato, sizeof(long), 1, arch);

    if (pos_dato == -1)
    {
        printf("No existen datos en la entidad\n");
        system("pause");
        system("cls");
        return;
    }

    printf("\nDame el nombre del dato a actualizar: ");
    scanf(" %[^\n]", nombre);

    if (buscarDatos(arch, cab_datos, nombre) == 0)
    {
        printf("El dato con nombre %s no existe en la entidad\n", nombre);
        system("pause");
        system("cls");
        return;
    }

    cab_dato.sig = 0;
    while (cab_dato.sig != -1)
    {
        fseek(arch, pos_dato, SEEK_SET);
        fread(&cab_dato, sizeof(CDat), 1, arch);
        pos = cab_dato.dat;
        fseek(arch, pos, SEEK_SET);
        fread(nombre_dato, 20, 1, arch);

        if (strcmp(nombre, nombre_dato) == 0)
        {
            break;
        }
        pos_dato = cab_dato.sig;
    }
    // cab_dato

    printf("Dame el nombre del atributo que quieres actualizar: ");
    scanf(" %[^\n]", nombre_atr);

    if (buscarAtributo(arch, nombre_atr, cab_atr) == 0)
    {
        printf("Ese atributo no existe\n");
        system("pause");
        system("cls");
        return;
    }

    pos_dato = cab_dato.dat;

    pos = cab_atr;
    fseek(arch, cab_atr, SEEK_SET);
    fread(&pos, sizeof(long), 1, arch);

    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        if (strcmp(atributo.nomAtr, nombre_atr) == 0)
            break;
        imprimirAtributo(atributo);
        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);

        fseek(arch, pos_dato + atributo.tam, SEEK_SET);
        fread(&pos_dato, sizeof(long), 1, arch);
    }

    dato = malloc(atributo.tam);
    fseek(arch, pos_dato, SEEK_SET);
    printf("Ingresa el valor de %s: ", atributo.nomAtr);
    switch (atributo.tipo)
    {
    case 1:

        scanf(" %c", (char *)dato);
        break;
    case 2:

        scanf("%d", (int *)dato);
        break;
    case 3:

        scanf("%f", (float *)dato);
        break;
    case 4:

        scanf(" %[^\n]", (char *)dato);
        break;
    }

    fwrite(dato, atributo.tam, 1, arch);
    free(dato);

    printf("Dato actualizado correctamente\n");
    imprimirTodosLosDatos(arch, cab_dato, cab_atr);
    printf("\n\n");
    system("pause");
    system("cls");
}

void reporteDatos(FILE *arch, long cab_datos, long cab_atr) {
    CDat cab_dato;
    long pos_cab;

    fseek(arch, cab_datos, SEEK_SET);
    fread(&pos_cab, sizeof(long), 1, arch);

    if (pos_cab == -1)
    {
        printf("No hay datos en la entidad\n");
        system("pause");
        system("cls");
        return;
    }

    while (pos_cab != -1)
    {
        fseek(arch, pos_cab, SEEK_SET);
        fread(&cab_dato, sizeof(CDat), 1, arch);
        pos_cab = cab_dato.sig;

        imprimirTodosLosDatos(arch, cab_dato, cab_atr);
        printf("\n");
    }
    system("pause");
    system("cls");
}
void imprimirTodosLosDatos(FILE *arch, CDat cab_dato, long cab_atr)
{
    ATR atributo;
    long pos_actual = cab_dato.dat;
    long pos_atr;
    void *dato;

    fseek(arch, cab_atr, SEEK_SET);
    fread(&pos_atr, sizeof(long), 1, arch);

    while (pos_actual != -1)
    {
        fseek(arch, pos_atr, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        dato = malloc(atributo.tam);

        fseek(arch, pos_actual, SEEK_SET);
        switch (atributo.tipo)
        {
        case 1:
            fread(dato, sizeof(char), 1, arch);
            break;
        case 2:
            fread(dato, sizeof(int), 1, arch);
            break;
        case 3:
            fread(dato, sizeof(float), 1, arch);
            break;
        case 4:
            fread(dato, 20, 1, arch);
            break;
        }
        printf("%s: ", atributo.nomAtr);
        fread(&pos_actual, sizeof(long), 1, arch);
        imprimeUnDato(dato, atributo.tipo);
        pos_atr = atributo.sigAtr;
    }
}
int buscarDatos(FILE *arch, long cab_datos, char nombre[])
{
    CDat cab;
    long pos, pos_dato;
    char nom_dato[20];

    if (cab_datos == -1)
    {
        printf("La lista de datos esta vacia\n");
        system("pause");
        system("cls");
        return 0;
    }

    fseek(arch, cab_datos, SEEK_SET);
    fread(&pos, sizeof(long), 1, arch);

    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&cab, sizeof(CDat), 1, arch);
        pos_dato = cab.dat;
        if (pos_dato == -1)
        {
            return 0;
        }

        fseek(arch, pos_dato, SEEK_SET);
        fread(nom_dato, 20, 1, arch);

        if (strcmp(nom_dato, nombre) == 0)
        {
            return 1;
        }
        pos = cab.sig;
    }

    return 0;
}
void imprimeUnDato(void *dato, int tipo)
{
    switch (tipo)
    {
    case 1:
        printf(" %c ", *(char *)dato);
        break;
    case 2:
        printf(" %d ", *(int *)dato);
        break;
    case 3:
        printf(" %f ", *(float *)dato);
        break;
    case 4:
        printf(" %s ", (char *)dato);
        break;
    }
    printf("\n");
}
void menuDatos(FILE *arch)
{
    int op = 0, repeticion = 0;
    char nomEnt[20];
    long pos = 0, cab_datos, cab_atr;
    ENT entidad;
    printf("Dame el nombre de la entidad: ");
    scanf(" %[^\n]", nomEnt);

    if (buscarEntidad(arch, nomEnt) == 1)
    {
        fseek(arch, 0, SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);

        while (pos != -1)
        {
            fseek(arch, pos, SEEK_SET);
            fread(&entidad, sizeof(ENT), 1, arch);
            if (strcmp(nomEnt, entidad.nomEnt) == 0)
                break;
            pos = entidad.sigEnt;
        }
        imprimirEntidad(entidad);
        printf("\nSe trabajara con la entidad: %s\n", entidad.nomEnt);
        system("pause");
        system("cls");
    }
    else
    {
        printf("La entidad buscada no existe en la base de datos\n\n");
        system("pause");
        system("cls");
        return;
    }

    if (entidad.cabAtr == -1)
    {
        printf("No es posible agregar datos, si no hay atributos\n");
        system("pause");
        system("cls");
        return;
    }

    fseek(arch, pos + sizeof(ENT) - (3 * sizeof(long)), SEEK_SET);
    cab_atr = ftell(arch);
    fseek(arch, sizeof(long), SEEK_CUR);
    cab_datos = ftell(arch);

    while (op != 6)
    {
        printf("\nMENU DATOS\n");
        printf("\n1) Consultar dato");
        printf("\n2) Alta de dato");
        printf("\n3) Baja de dato");
        printf("\n4) Actualiza dato");
        printf("\n5) Reporte de datos");
        printf("\n6) Salir\n");

        printf("\nEscoge una opcion: ");
        scanf("%d", &op);

        while (op < 1 || op > 6)
        {
            printf("opcion invalida\n");
            printf("Ingresa una opcion: ");
            scanf("%d", &op);
        }
        system("cls");
        switch (op)
        {
        case 1:
            do
            {
                consultarDatos(arch, cab_datos, cab_atr);
                printf("Quires consultar de nuevo? \n1.Si 0.No : ");
                do
                {
                    scanf("%d", &repeticion);
                    if (repeticion > 1 || repeticion < 0)
                    {
                        printf("\nOPCION NO VALIDA. INTENTA DE NUEVO\n");
                        printf("Quires consultar de nuevo? \n1.Si 0.No :");
                    }
                } while (repeticion > 1 || repeticion < 0);
                system("cls");
            } while (repeticion);
            break;
        case 2:
            do
            {
                altaDatos(arch, cab_datos, cab_atr);
                printf("Quires dar de alta de nuevo? \n1.Si 0.No : ");
                do
                {
                    scanf("%d", &repeticion);
                    if (repeticion > 1 || repeticion < 0)
                    {
                        printf("\nOPCION NO VALIDA. INTENTA DE NUEVO\n");
                        printf("Quires dar de alta de nuevo? \n1.Si 0.No :");
                    }
                } while (repeticion > 1 || repeticion < 0);
                system("cls");
            } while (repeticion);
            break;
        case 3:
            do
            {
                bajaDatos(arch, cab_datos);
                printf("Quires dar de baja de nuevo? \n1.Si 0.No : ");
                do
                {
                    scanf("%d", &repeticion);
                    if (repeticion > 1 || repeticion < 0)
                    {
                        printf("\nOPCION NO VALIDA. INTENTA DE NUEVO\n");
                        printf("Quires dar de baja de nuevo? \n1.Si 0.No :");
                    }
                } while (repeticion > 1 || repeticion < 0);
                system("cls");
            } while (repeticion);
            break;
        case 4:
            do
            {
                cambiarDatos(arch, cab_datos, cab_atr);
                printf("Quires actualizar de nuevo? \n1.Si 0.No : ");
                do
                {
                    scanf("%d", &repeticion);
                    if (repeticion > 1 || repeticion < 0)
                    {
                        printf("\nOPCION NO VALIDA. INTENTA DE NUEVO\n");
                        printf("Quires actualizar de nuevo? \n1.Si 0.No :");
                    }
                } while (repeticion > 1 || repeticion < 0);
                system("cls");
            } while (repeticion);
            break;
        case 5:
            reporteDatos(arch, cab_datos, cab_atr);
            break;
        case 6:
            printf("Cerrando menu datos\n");
            system("pause");
            system("cls");
            return;
            break;
        }
    }
}