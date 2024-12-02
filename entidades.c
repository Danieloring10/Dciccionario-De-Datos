#include "diccionario.h"
int buscarEntidad(FILE *arch, char nombre[])
{
    long pos_i = 0;
    ENT entidad;

    fseek(arch, 0, SEEK_SET);
    fread(&pos_i, sizeof(long), 1, arch);

    if (pos_i == -1)
    {
        // No hay entidades
        printf("La lista de entidades esta vacia\n");
        system("pause");
        system("cls");
        return 0;
    }

    while (pos_i != -1)
    {
        fseek(arch, pos_i, SEEK_SET);
        fread(&entidad, sizeof(ENT), 1, arch);
        if (strcmp(nombre, entidad.nomEnt) == 0)
        {
            return 1;
        }
        pos_i = entidad.sigEnt;
    }

    return 0;
}
ENT crearEntidad(char nombre[])
{
    ENT nueva_ent;
    strcpy(nueva_ent.nomEnt, nombre);
    nueva_ent.cabAtr = -1;
    nueva_ent.cabDatos = -1;
    nueva_ent.sigEnt = -1;
    return nueva_ent;
}
void altaEntidad(FILE *arch) {
    ENT nueva_ent;
    char nombre_ent[20];
    long pos_f = 0, pos_ent = 0, pos_ant;
    printf("Ingresa el nombre de la entidad: ");
    scanf(" %[^\n]", nombre_ent);

    if (buscarEntidad(arch, nombre_ent) == 1)
    {
        printf("La entidad ya existe\n");
        system("pause");
        system("cls");
        return;
    }
    // Lee cabezera
    fseek(arch, 0, SEEK_SET);
    fread(&pos_f, sizeof(long), 1, arch);

    pos_ant = -1;
    while (pos_f != -1)
    {
        pos_ant = pos_f;
        fseek(arch, pos_f + sizeof(ENT) - sizeof(long), SEEK_SET);
        fread(&pos_f, sizeof(long), 1, arch);
    }

    nueva_ent = crearEntidad(nombre_ent);

    fseek(arch, 0, SEEK_END);
    pos_ent = ftell(arch);
    fwrite(&nueva_ent, sizeof(ENT), 1, arch);

    if (pos_ant == -1)
    {
        fseek(arch, 0, SEEK_SET);
        fwrite(&pos_ent, sizeof(long), 1, arch);
    }
    else
    {
        fseek(arch, pos_ant + sizeof(ENT) - sizeof(long), SEEK_SET);
        fwrite(&pos_ent, sizeof(long), 1, arch);
    }

    printf("Entidad '%s' agregada correctamente.\n\n", nombre_ent);
    system("pause");
    system("cls");
}

void consultarEntidad(FILE *arch) {
    long pos = 0;
    ENT entidad;
    char nomEnt[20];
    printf("Ingresa el nombre de la entidad a consultar: ");
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
            {
                break;
            }
            pos = entidad.sigEnt;
        }

        imprimirEntidad(entidad);
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
}

void bajaEntidad(FILE *arch) {
    ENT entidad;
    char nomEnt[20];
    long pos = 0;
    long pos_ant = 0;
    printf("Ingresa el nombre de la entidad: ");
    scanf(" %[^\n]", nomEnt);

    if (buscarEntidad(arch, nomEnt) == 1)
    {
        fseek(arch, 0, SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);
        fseek(arch, pos, SEEK_SET);
        fread(&entidad, sizeof(ENT), 1, arch);

        while (strcmp(nomEnt, entidad.nomEnt) != 0)
        {

            pos = ftell(arch);
            fseek(arch, pos - sizeof(long), SEEK_SET);
            pos_ant = ftell(arch);
            pos = entidad.sigEnt;
            fseek(arch, pos, SEEK_SET);
            fread(&entidad, sizeof(ENT), 1, arch);
        }

        pos = entidad.sigEnt;
        fseek(arch, pos_ant, SEEK_SET);
        fwrite(&pos, sizeof(long), 1, arch);
        printf("Eliminacion realizada correctamente\n\n");
        system("pause");
        system("cls");
    }
    else
    {
        printf("La entidad a eliminar no existe en la base de datos\n\n");
        system("pause");
        system("cls");
        return;
    }
}

void cambiarNombreEntidad(FILE *arch) {
    ENT entidad;
    char nomEnt[20], nomNuevo[20];

    long pos = 0;
    printf("Ingresa el nombre de la entidad a actualizar: ");
    scanf(" %[^\n]", nomEnt);

    printf("\nIngresa el nombre nuevo: ");
    scanf(" %[^\n]", nomNuevo);

    if (buscarEntidad(arch, nomEnt) == 1)
    {
        fseek(arch, 0, SEEK_SET);
        fread(&pos, sizeof(long), 1, arch);
        fseek(arch, pos, SEEK_SET);
        fread(&entidad, sizeof(ENT), 1, arch);

        while (strcmp(nomEnt, entidad.nomEnt) != 0)
        {
            pos = entidad.sigEnt;
            fseek(arch, pos, SEEK_SET);
            fread(&entidad, sizeof(ENT), 1, arch);
        }

        fseek(arch, pos, SEEK_SET);
        strcpy(entidad.nomEnt, nomNuevo);
        fwrite(&entidad, sizeof(ENT), 1, arch);
        printf("Actualizacion realizada correctamente\n\n");
        system("pause");
        system("cls");
    }
    else
    {
        printf("La entidad a actualizar no existe en la base de datos\n\n");
        system("pause");
        system("cls");
        return;
    }
}

void reporteEntidades(FILE *arch) {
    long pos = 0;
    ENT entidad;

    printf("REPORTE ENTIDADES\n");

    fseek(arch, 0, SEEK_SET);
    fread(&pos, sizeof(long), 1, arch);
    if (pos == -1)
    {
        printf(" - NO HAY ENTIDADES\n");
        system("pause");
        system("cls");
        return;
    }

    fseek(arch, pos, SEEK_SET);
    fread(&entidad, sizeof(ENT), 1, arch);
    imprimirEntidad(entidad);
    pos = entidad.sigEnt;

    while (pos != -1)
    {
        fseek(arch, pos, SEEK_SET);
        fread(&entidad, sizeof(ENT), 1, arch);
        pos = entidad.sigEnt;
        imprimirEntidad(entidad);
    }
    system("pause");
    system("cls");
}

void imprimirEntidad(ENT entidad)
{
    printf("\nNombre Entidad: %s \nCabeceraATR: %ld, \nCabeceraDatos: %ld, \nSiguiente Entidad: %ld\n", entidad.nomEnt, entidad.cabAtr, entidad.cabDatos, entidad.sigEnt);
}

void menuEntidad(FILE *arch)
{
    int op = 0;
    int repeticion = 0;
    while (op != 6)
    {
        printf("\nMENU ENTIDADES\n");
        printf("\n1) Consultar entidad");
        printf("\n2) Alta de entidad");
        printf("\n3) Baja de entidad");
        printf("\n4) Actualiza entidad");
        printf("\n5) Reporte entidad");
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
                consultarEntidad(arch);
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
                altaEntidad(arch);
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
                bajaEntidad(arch);
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
                cambiarNombreEntidad(arch);
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
            reporteEntidades(arch);
            break;
        case 6:
            return;
            break;
        }
    }
}