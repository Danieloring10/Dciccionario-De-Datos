#include "entidades.c"
#include "atributos.c"
#include "datos.c"

void creaArch(FILE **arch, char nombre[])
{
    long cab = -1;
    int abrir = 0;
    if (fopen(nombre, "rb") == NULL)
    {
        *arch = fopen(nombre, "wb+");
        fwrite(&cab, sizeof(long), 1, *arch);
        printf("Archivo creado correctamente\n\n");
        system("pause");
        system("cls");
    }
    else
    {
        if (*arch != NULL)
            fclose(*arch);

        printf("El archivo ya existe, desea abrirlo? \n1.Si 2.No :");
        scanf("%d", &abrir);
        while (abrir < 1 || abrir > 2)
        {
            printf("opcion invalida\n");
            printf("Ingresa una opcion: ");
            scanf("%d", &abrir);
        }
        system("cls");
        if (abrir == 1)
        {

            if (aperturaDiccionario(arch, "rb+", nombre) == 0)
            {
                printf("No se logro abrir el archivo\n");
                system("pause");
                system("cls");
            }
            else
                menuPrincipal(*arch);
        }
        else
            return;
    }

    if (*arch != NULL)
        fclose(*arch);
}

void creaDiccinario(FILE *arch)
{
    char nombre[20];
    printf("\nIngrese el nombre del diccionario: ");
    scanf(" %[^\n]", nombre);
    strcat(nombre, ".bin");
    creaArch(&arch, nombre);
}

int aperturaDiccionario(FILE **arch, char modoAp[], char nomArch[])
{

    *arch = fopen(nomArch, modoAp);
    if (*arch == NULL)
        return 0;
    else
    {
        printf("Archivo abierto correctamente\n");
        system("pause");
        system("cls");
        return 1;
    }
}

void abrirDiccionario(FILE *arch)
{
    int crear = 0;
    char nomArch[20];
    printf("Ingresa el nombre del archivo a modificar: ");
    scanf(" %[^\n]", nomArch);
    strcat(nomArch, ".bin");
    if (aperturaDiccionario(&arch, "rb+", nomArch) == 0)
    {
        printf("El archivo no existente, desea crear el archivo? \n1.Si 2.No: ");
        scanf("%d", &crear);
        while (crear < 1 || crear > 2)
        {
            printf("Opcion no valida\n");
            printf("Ingresa una opcion: ");
            scanf("%d", &crear);
        }
        if (crear == 1)
        {
            creaArch(&arch, nomArch);
            aperturaDiccionario(&arch, "rb+", nomArch);
        }

        if (crear == 2)
        {
            system("pause");
            system("cls");
            return;
        }
    }
    menuPrincipal(arch);
}

void menuPrincipal(FILE *arch)
{
    int op = 0;
    while (op != 5)
    {
        // printf("\n");
        printf("\nMENU DICCIONARIO\n");
        printf("\n1) Menu Entidades");
        printf("\n2) Menu Atributos");
        printf("\n3) Menu Datos");
        printf("\n4) Reporte Completo");
        printf("\n5) Volver\n");

        printf("\nIngresa una opcion: ");
        scanf("%d", &op);

        while (op < 1 || op > 5)
        {
            printf("opcion invalida\n");
            printf("Ingresa una opcion: ");
            scanf("%d", &op);
        }
        system("cls");
        switch (op)
        {
        case 1:
            menuEntidad(arch);
            break;
        case 2:
            menuAtributos(arch);
            break;
        case 3:
            menuDatos(arch);
            break;
        case 4:
            reporteDiccionario(arch);
            break;
        case 5:
            printf("Archivo cerrado correctamente\n");
            return;
            break;
        }
    }
    if (arch != NULL)
        fclose(arch);
}

void reporteDiccionario(FILE *arch)
{
    ENT entidad;
    ATR atributo;
    CDat cab_dato;
    long cab_datos, cab_atr, pos_ent;
    long pos_atr, pos_cab;

    printf("\nREPORTE COMPLETO\n");

    fseek(arch, 0, SEEK_SET);
    fread(&pos_ent, sizeof(long), 1, arch);
    if (pos_ent == -1)
    {
        printf(" - No existen entidades\n");
        system("pause");
        system("cls");
        return;
    }

    while (pos_ent != -1)
    {
        fseek(arch, pos_ent, SEEK_SET);
        fread(&entidad, sizeof(ENT), 1, arch);
        imprimirEntidad(entidad);
        printf("\n");

        fseek(arch, pos_ent + 20, SEEK_SET);
        fread(&cab_atr, sizeof(long), 1, arch);

        if (cab_atr == -1)
        {
            printf("la lista de atributos esta vacia\n");
            pos_atr = -1;
        }
        else
            pos_atr = cab_atr;

        while (pos_atr != -1)
        {
            fseek(arch, pos_atr, SEEK_SET);
            fread(&atributo, sizeof(ATR), 1, arch);
            imprimirAtributo(atributo);

            fseek(arch, pos_atr + sizeof(ATR) - sizeof(long), SEEK_SET);
            fread(&pos_atr, sizeof(long), 1, arch);
        }

        printf("\n");

        fseek(arch, pos_ent + 20, SEEK_SET);
        cab_atr = ftell(arch);

        // Reporte de datos
        fseek(arch, pos_ent + sizeof(ENT) - (2 * sizeof(long)), SEEK_SET);
        cab_datos = ftell(arch);

        fseek(arch, cab_datos, SEEK_SET);
        fread(&pos_cab, sizeof(long), 1, arch);

        if (pos_cab == -1)
        {
            printf("No hay datos en la entidad\n");
        }

        while (pos_cab != -1)
        {
            fseek(arch, pos_cab, SEEK_SET);
            fread(&cab_dato, sizeof(CDat), 1, arch);
            pos_cab = cab_dato.sig;

            imprimirTodosLosDatos(arch, cab_dato, cab_atr);
            printf("\n");
        }
        printf("\n");
        // Fin datos

        pos_ent = entidad.sigEnt;
        printf("\n");
    }
    printf("\n");
    system("pause");
    system("cls");
}

void menu()
{
    FILE *arch = NULL;
    int op = 0;
    while (op != 3)
    {

        printf("\nDICCIONARIO DE DATOS\n");
        printf("\n1) Crear Diccionario");
        printf("\n2) Abrir Diccionario");
        printf("\n3) Salir\n");

        printf("\nIngresa una opcion: ");
        scanf("%d", &op);

        while (op < 1 || op > 3)
        {
            printf("opcion invalida\n");
            printf("Ingresa una opcion: ");
            scanf("%d", &op);
        }

        system("cls");
        switch (op)
        {
        case 1:
            creaDiccinario(arch);
            system("cls");
            break;
        case 2:
            abrirDiccionario(arch);
            system("cls"); 
            break;
        case 3:
            printf("Programa Finalizado con exito\n");
            system("pause");
            system("cls");
            break;
        }
    }

    if (arch != NULL)
    {
        fclose(arch);
    }
}

int main()
{
    menu();
    return 0;
}
