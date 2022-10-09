#include "treemap.c"
#include "list.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct{
    int dia;
    int mes;
    int año;
}date;

typedef struct{
    char name[30];
    date releaseDate;
    int valoracion;
    int precio;
}game;

void menu();
void select_option();

int main(){
    select_option();
    return 0;
}

void menu() {
  printf("===MENU===\n");
  printf("1. Importar juegos desde un archivo CSV\n");
  printf("2. Agregar juego\n");
  printf("3. Mostrar juegos por precio\n");
  printf("4. Filtrar juegos por valoracion\n");
  printf("5. Mostrar juegos del año\n");
  printf("6. Buscar juego\n");
  printf("7. Exportar datos\n");
  printf("0. Salir");
}
void select_option() {
  int choice = -1;
  system("clear");

  while (choice != 0) {
    menu();
    printf("\nSeleccione un opcion:\n");
    scanf("%d%*c", &choice);
    switch (choice) {
    case 1:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 2:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 3:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 4:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 5:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 6:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 7:
        system("clear");
        printf("op %d\n", choice);
      break;
    case 0:
        system("clear");
      printf("Hasta la proxima\n");
      break;
    default:
        system("clear");
      printf("Opcion incorrecta, intente de nuevo\n");
      break;
    }
  }
}