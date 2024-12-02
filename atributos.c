#include "diccionario.h"
int buscarAtributo(FILE *arch, char nombAtr[], long cab_atr)
{
    long pos;
    ATR atributo;
    fseek(arch, cab_atr, SEEK_SET);
    fread(&pos, sizeof(long), 1, arch);

    if (pos == -1)
    {
        printf("Lista de atributos vacia\n");
        system("pause");
        system("cls");
        return 0;
    }

    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        if (strcmp(atributo.nomAtr, nombAtr) == 0)
            return 1;
        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);
    }

    return 0;
}

ATR crearAtributos(char nomAtr[], int tipo)
{
    ATR nuevo_atr;
    strcpy(nuevo_atr.nomAtr, nomAtr);
    nuevo_atr.tipo = tipo;
    switch (tipo)
    {
    case 1:
        nuevo_atr.tam = sizeof(char);
        break;
    case 2:
        nuevo_atr.tam = sizeof(int);
        break;
    case 3:
        nuevo_atr.tam = sizeof(float);
        break;
    case 4:
        nuevo_atr.tam = 20;
        break;
    }
    nuevo_atr.sigAtr = -1;
    return nuevo_atr;
}

void altaAtributo(FILE *arch, long pos_ent) {
    ENT entidad;
    ATR atributo;
    long cab_atr, pos_atr, pos_ant, pos = 0;
    char nombAtr[20];
    int tipo = 0;

    printf("\nDame el nombre del atributo: ");
    scanf(" %[^\n]", nombAtr);

    fseek(arch, pos_ent, SEEK_SET);
    fread(&entidad, sizeof(ENT), 1, arch);

    fseek(arch, pos_ent + 20, SEEK_SET);
    pos_ant = ftell(arch);
    fread(&cab_atr, sizeof(long), 1, arch);

    if (buscarAtributo(arch, nombAtr, pos_ant) == 1)
    {
        printf("\nEl atributo ya existe en la base de datos\n");
        system("pause");
        system("cls");
        return;
    }

    printf("Tipo disponibles:\n");
    printf("1. Char\n");
    printf("2. Int\n");
    printf("3. Float\n");
    printf("4. String\n");
    printf("De que tipo sera el atributo?: ");
    scanf("%d", &tipo);

    pos = cab_atr;
    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        pos_ant = ftell(arch);
        fread(&pos, sizeof(long), 1, arch);
    }

    fseek(arch, 0, SEEK_END);
    pos_atr = ftell(arch);
    atributo = crearAtributos(nombAtr, tipo);
    fwrite(&atributo, sizeof(ATR), 1, arch);
    fseek(arch, pos_ant, SEEK_SET);
    fwrite(&pos_atr, sizeof(long), 1, arch);

    printf("\nAtributo agregado correctamente\n");
    system("pause");
    system("cls");
}

void consultarAtributos(FILE *arch, long pos_ent) {
    ENT entidad;
    ATR atributo;
    long cab_atr, pos_ant, pos = 0;
    char nombAtr[20];

    printf("\nDame el nombre del atributo: ");
    scanf(" %[^\n]", nombAtr);

    fseek(arch, pos_ent, SEEK_SET);
    fread(&entidad, sizeof(ENT), 1, arch);

    fseek(arch, pos_ent + 20, SEEK_SET);
    pos_ant = ftell(arch);
    fread(&cab_atr, sizeof(long), 1, arch);

    if (buscarAtributo(arch, nombAtr, pos_ant) == 0)
    {
        printf("\nEl atributo no existe en la base de datos\n");
        system("pause");
        system("cls");
        return;
    }

    pos = cab_atr;
    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        if (strcmp(atributo.nomAtr, nombAtr) == 0)
        {
            imprimirAtributo(atributo);
            system("pause");
            system("cls");
            return;
        }

        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);
    }
}
void bajaAtributo(FILE *arch, long pos_ent) {
    ENT entidad;
    ATR atributo;
    long cab_atr, pos_ant, pos = 0;
    char nombAtr[20];

    printf("\nDame el nombre del atributo: ");
    scanf(" %[^\n]", nombAtr);

    fseek(arch, pos_ent, SEEK_SET);
    fread(&entidad, sizeof(ENT), 1, arch);

    fseek(arch, pos_ent + 20, SEEK_SET);
    pos_ant = ftell(arch);
    fread(&cab_atr, sizeof(long), 1, arch);

    if (buscarAtributo(arch, nombAtr, pos_ant) == 0)
    {
        printf("\nEl atributo no existe en la base de datos\n");
        system("pause");
        system("cls");
        return;
    }

    pos = cab_atr;
    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);

        if (strcmp(atributo.nomAtr, nombAtr) == 0)
        {
            fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
            fread(&pos, sizeof(long), 1, arch);

            fseek(arch, pos_ant, SEEK_SET);
            fwrite(&pos, sizeof(long), 1, arch);
            printf("Eliminacion realizada con exito\n");
            system("pause");
            system("cls");
            return;
        }
        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        pos_ant = ftell(arch);
        fread(&pos, sizeof(long), 1, arch);
    }
}

void cambiarNombreAtributo(FILE *arch, long pos_ent) {
    ENT entidad;
    ATR atributo;
    long cab_atr, pos_ant, pos = 0;
    char nombAtr[20], nombreNuevo[20];

    printf("\nDame el nombre del atributo: ");
    scanf(" %[^\n]", nombAtr);

    fseek(arch, pos_ent, SEEK_SET);
    fread(&entidad, sizeof(ENT), 1, arch);

    fseek(arch, pos_ent + 20, SEEK_SET);
    pos_ant = ftell(arch);
    fread(&cab_atr, sizeof(long), 1, arch);

    if (buscarAtributo(arch, nombAtr, pos_ant) == 0)
    {
        printf("\nEl atributo no existe en el diccionario\n");
        system("pause");
        system("cls");
        return;
    }

    printf("\nDame el nuevo nombre del atributo: ");
    scanf(" %[^\n]", nombreNuevo);

    pos = cab_atr;
    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        if (strcmp(atributo.nomAtr, nombAtr) == 0)
        {
            fseek(arch, pos, SEEK_SET);
            fwrite(&nombreNuevo, 20, 1, arch);
            printf("\nNombre actualizado con exito\n");
            system("pause");
            system("cls");
            return;
        }

        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);
    }
}

void reporteAtributos(FILE *arch, long pos_ent) {
    ENT entidad;
    ATR atributo;
    long cab_atr, pos = 0;

    fseek(arch, pos_ent, SEEK_SET);
    fread(&entidad, sizeof(ENT), 1, arch);

    fseek(arch, pos_ent + 20, SEEK_SET);
    fread(&cab_atr, sizeof(long), 1, arch);

    if (cab_atr == -1)
    {
        printf("la lista de atributos esta vacia\n");
        system("pause");
        system("cls");
        return;
    }

    pos = cab_atr;
    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&atributo, sizeof(ATR), 1, arch);
        imprimirAtributo(atributo);
        fseek(arch, pos + sizeof(ATR) - sizeof(long), SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);
    }
    printf("\n");
    system("pause");
    system("cls");
}

void imprimirAtributo(ATR atributo)
{
    printf("\nNombre del atributo: %s \nTipo del atributo: %d \nTamaño del atributo: %d \nSiguienteATR: %ld\n", atributo.nomAtr, atributo.tipo, atributo.tam, atributo.sigAtr);
}
void menuAtributos(FILE *arch)
{
    int op = 0, lim = 0, repeticion = 0;
    char nomEnt[20];
    long pos = 0;
    ENT entidad;
    printf("Con cual entidad trabajaras?: ");
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

    if (entidad.cabDatos != -1)
        lim = 1;

    while (op != 6)
    {
        printf("\nMENU ATRIBUTOS\n");
        printf("\n1) Consultar Atributo");
        printf("\n2) Alta de Atributo");
        printf("\n3) Baja de Atributo");
        printf("\n4) Actualiza Atributo");
        printf("\n5) Reporte de Atributo");
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
                consultarAtributos(arch, pos);
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
                if (lim != 1)
                    altaAtributo(arch, pos);
                else
                {
                    printf("No es posible dar de alta atributos\n");
                    system("pause");
                    system("cls");
                }
                printf("Quires dar alta de nuevo? \n1.Si 0.No : ");
                do
                {
                    scanf("%d", &repeticion);
                    if (repeticion > 1 || repeticion < 0)
                    {
                        printf("\nOPCION NO VALIDA. INTENTA DE NUEVO\n");
                        printf("Quires dar alta de nuevo? \n1.Si 0.No :");
                    }
                } while (repeticion > 1 || repeticion < 0);
                system("cls");
            } while (repeticion);
            break;
        case 3:
            do
            {
                if (lim != 1)
                    bajaAtributo(arch, pos);
                else
                {
                    printf("No es posible dar de baja atributos\n");
                    system("pause");
                    system("cls");
                }
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
                if (lim != 1)
                    cambiarNombreAtributo(arch, pos);
                else
                {
                    printf("No es posible actualizar atributos\n");
                    system("pause");
                    system("cls");
                }
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
            reporteAtributos(arch, pos);
            break;
        case 6:
            printf("Cerrando menu atributos\n");
            system("pause");
            system("cls");
            return;
            break;
        }
    }
}