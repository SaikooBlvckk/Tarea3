#include "treemap.c"
#include "list.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


//Estructura 
typedef struct{
  char name[50];
  char releaseDate[30];
  int valuation;
  int cost;
}games;

//Declaracion de funciones
void menu();
void select_option();
const char *get_csv_field(char *, int);
int lower_than_int(void *, void *);
int lower_than_string(void *, void *);
void file_exists(TreeMap *, TreeMap *, TreeMap *);
int check_file_exists(char *);
void import_file(char *, TreeMap *, TreeMap *, TreeMap *);
games *createGame(char *, char *, int, int);
void insert_list(TreeMap *, games *, int);
void list_exists(TreeMap *, TreeMap *, games *);
void create_list(TreeMap *, games *, int);
void print_nameMap(TreeMap * , char *);
void print_byValuation(TreeMap *nameMap, TreeMap *map);
void add_game(TreeMap *, TreeMap *, TreeMap *);
void print_bycost(TreeMap *,TreeMap *);
void searchGame(TreeMap *, TreeMap *, TreeMap *);
void editGame(games *, TreeMap *, TreeMap *, TreeMap *);
void deleteFromList(TreeMap *, char *);
void print_byAge(TreeMap *);
void export_csv(TreeMap *);

void print_all_games(TreeMap *);

//Codigo principal
int main(){
  select_option();
  return 0;
}

//Menú opciones
void menu() {
  printf("===MENU===\n");
  printf("1. Importar juegos desde un archivo CSV\n");
  printf("2. Agregar juego\n");
  printf("3. Mostrar juegos por precio\n");
  printf("4. Filtrar juegos por valoracion\n");
  printf("5. Mostrar juegos del ano\n");
  printf("6. Buscar juego\n");
  printf("7. Exportar datos\n");
  printf("0. Salir");
}

//Declaracion de mapas y seleccion de opciones
void select_option() {
  TreeMap *nameMap = createTreeMap(lower_than_string);
  TreeMap *valuationMap = createTreeMap(lower_than_int);
  TreeMap *costMap = createTreeMap(lower_than_int);

  int choice = 1;
  system("cls");

  while (choice != 0) {
    menu();
    printf("\nSeleccione un opcion:\n");
    scanf("%d", &choice);
    system("cls");
    switch (choice) {
      case 1:
          file_exists(nameMap,valuationMap, costMap);
        break;
      case 2:
          add_game(nameMap,valuationMap, costMap);
        break;
      case 3:
          print_bycost(nameMap, costMap);
        break;
      case 4:
          print_byValuation(nameMap, valuationMap);
        break;
      case 5:
          print_byAge(valuationMap);
        break;
      case 6:
          searchGame(nameMap, valuationMap, costMap);
        break;
      case 7:
          export_csv(nameMap);
        break;
      case 0:
        printf("Hasta la proxima\n");
        break;
      default:
          printf("Opcion incorrecta, intente de nuevo\n");
        break;
    }
  }
}

//Manda a verificar que el archivo exista
void file_exists(TreeMap *nameMap,TreeMap *valuationMap, TreeMap *costMap) {
  char file[30];
  strcpy(file, "Videojuegos.csv"); /* ! testing purposes */
  printf("\nIngrese el nombre del Archivo .csv:\n");
  scanf("%s", &file);
  while (1) {
    if (check_file_exists(file)) {
      printf("\n---Importando Archivo Con los items---\n");
      break;
    } else {
      printf("Archivo no existe, intente de nuevo...\n");
      scanf("%s", &file);
    }
  }
  import_file(file, nameMap, valuationMap, costMap);
}

//Verifica que el archivo exista
int check_file_exists(char *file) {
  struct stat buffer;
  int exist = stat(file, &buffer);
  if (exist == 0)
    return 1;
  else
    return 0;
}

//Compara 2 claves de tipo int
int lower_than_int(void * key1, void * key2) {
  return key1<key2;
}


//Compara 2 claves de tipo char
int lower_than_string(void * key1, void * key2) {
    if(strcmp(key1, key2) < 0) return 1;
    return 0;
}

//funcion del profesor
const char *get_csv_field(char *tmp, int k) {
  int open_mark = 0;
  char *ret = (char *)malloc(100 * sizeof(char));
  int ini_i = 0, i = 0;
  int j = 0;
  while (tmp[i + 1] != '\0') {

    if (tmp[i] == '\"') {
      open_mark = 1 - open_mark;
      if (open_mark)
        ini_i = i + 1;
      i++;
      continue;
    }

    if (open_mark || tmp[i] != ',') {
      if (k == j)
        ret[i - ini_i] = tmp[i];
      i++;
      continue;
    }

    if (tmp[i] == ',') {
      if (k == j) {
        ret[i - ini_i] = 0;
        return ret;
      }
      j++;
      ini_i = i + 1;
    }

    i++;
  }

  if (k == j) {
    ret[i - ini_i] = 0;
    return ret;
  }

  return NULL;
}

//Se importa el archivo y se ingresan los respectivos datos
void import_file(char * file, TreeMap *nameMap, TreeMap *valuationMap, TreeMap *costMap) {
  FILE *fp = fopen(file, "r");

  char line[1024];

  fgets(line, 1023, fp);
  while (fgets(line, 1023, fp) != NULL) {
    games *game = createGame(get_csv_field(line, 0), get_csv_field(line, 1), atoi(get_csv_field(line,2)), atoi(get_csv_field(line, 3)));

    insertTreeMap(nameMap, game->name, game);
    list_exists(valuationMap, costMap, game);
  }
  printf("\nDatos Cargados Correctamente :D\n\n");
}

//Crea un jueg y rellena sus datos
games *createGame(char *name, char*releaseDate, int valuation, int cost){
  games *game = (games *)malloc(sizeof(games));
  strcpy(game->name, name);
  strcpy(game->releaseDate, releaseDate);
  game->valuation = valuation;
  game->cost = cost;
  return game;
}

//Ve si la lista de valoracion o costo existe, si no existe crea uno nuevo y si existe lo inserta en la lista
void list_exists(TreeMap *valuationMap, TreeMap *costMap, games *game){
  if(searchTreeMap(valuationMap, game->valuation) == NULL) create_list(valuationMap, game, 1);
  else insert_list(valuationMap, game, 1);
  
  if(searchTreeMap(costMap, game->cost) == NULL) create_list(costMap, game, 2);
  else insert_list(costMap, game, 2);
}

//Crea la lista 
void create_list(TreeMap *map, games *game, int option){
  List *list =createList();
  pushBack(list, game->name);
  if (option == 1)
    insertTreeMap(map, game->valuation, list);
  else
    insertTreeMap(map, game->cost, list);
}

//Inserta la lista en el mapa
void insert_list(TreeMap *map, games *game, int option){
  List *list = (List *)malloc(sizeof(List));
  if (option == 1)
    list = searchTreeMap(map, game->valuation)->value;
  else
    list = searchTreeMap(map, game->cost)->value;
  pushBack(list, game->name);
}


//Funcion propia que imprime todos los juegos
void print_all_games(TreeMap *nameMap){
  printf("Nombre, año de salida, valoracion, precio\n");
  Pair *pr = firstTreeMap(nameMap);
  int i = 1;
  do{
    games *data = pr->value;
    printf("%d ", i++);
    print_nameMap(nameMap, data->name);
    pr = nextTreeMap(nameMap);
  }while(pr != NULL);
}

//Imprime los nombres de todos los juegos
void print_nameMap(TreeMap *nameMap, char *key) {
  Pair *data = searchTreeMap(nameMap, key);
  games *aux = data->value;
  printf("%s %s %d %d\n", aux->name, aux->releaseDate, aux->cost,
          aux->valuation);
}

//Imprime los juegos por valoracion
void print_byValuation(TreeMap *nameMap, TreeMap *map){
  int valuation;
  printf("Indique la valoracion por la que quiere filtrar:\n");
  scanf("%d", &valuation);
  Pair *aux = upperBound(map, valuation);
  if (aux == NULL){
    printf("Valoracion no encontrada, reintente!\n");
    return ;
  }
  printf("Nombre, año de salida, valoracion, precio\n");
  do{
    List *list = aux->value;
    char *id = firstList(list);
    do{
      print_nameMap(nameMap, id);
      id = nextList(list);
    }while (id != NULL);
    aux = nextTreeMap(map);
  }while(aux != NULL);
}

//Pide los datos para crear un nuevo juego
void add_game(TreeMap *nameMap, TreeMap *valuationMap, TreeMap *costMap){
  char name[50];
  char date[30];
  int valuation, cost;
  printf("Ingrese el Nombre del juego\n");
  getchar();
  scanf("%100[^\n]s", name);
  getchar();
  printf("Ingrese la fecha de salida del juego [DD/MM/YYYY](Con los '/' incluidos)\n");
  getchar();
  scanf("%100[^\n]s", date);
  getchar();
  printf("Ingrese Valoracion del Juego\n");
  scanf("%d",&valuation);
  printf("Ingrese Precio del Juego\n");
  scanf("%d",&cost);
  
  games *game = createGame(name, date, valuation, cost);
  insertTreeMap(nameMap, game->name, game);
  list_exists(valuationMap, costMap, game);
}

//Imprime segun la valoracion
void print_bycost(TreeMap *nameMap,TreeMap *costMap){
  int option;
  printf("[1] imprimir de menor a mayor\n");
  printf("[2] imprimir de mayor a menor\n");
  scanf("%d", &option);
  Pair *aux;
  if(option == 1){
    aux = firstTreeMap(costMap);
    while(aux != NULL){
      List *list = aux->value;
      games *game = firstList(list);
      while (game != NULL){
        print_nameMap(nameMap, game->name);
        game =nextList(list);
      }
      aux =nextTreeMap(costMap);
    }
  }else if(option == 2){
    aux = lastTreeMap(costMap);
    while(aux != NULL){
      List *list = aux->value;
      games *game = firstList(list);
      while (game != NULL){
        print_nameMap(nameMap, game->name);
        game =nextList(list);
      }
      aux =prevTreeMap(costMap);
    }
  }else printf("Opcion incorrecta (1 o 2)\n");
}

//Busca el juego que el usuario ingresa para luego editar los datos o eliminar el juego
void searchGame(TreeMap *nameMap, TreeMap *valuationMap, TreeMap *costMap){
  char name[50];
  int option = 1;
  printf("Ingrese El juego que desea buscar\n");
  getchar();
  scanf("%100[^\n]s", name);
  getchar();
  Pair *aux = searchTreeMap(nameMap, name);
  if(aux == NULL) {
    printf("Juego no existe, reintente!\n");
    return ;
  }
  games *game = aux->value;
  print_nameMap(nameMap, name);
  printf("Ingrese lo que desea realizar\n");
  printf("[1] Editar datos del juego\n");
  printf("[2] Eliminar Juego\n");
  scanf("%d", &option);

  if(option == 1){
    editGame(game, nameMap, valuationMap, costMap);
  }else if (option == 2){
    removeNode(nameMap, nameMap->current);
  }else return ;

  deleteFromList(valuationMap, name);
  deleteFromList(costMap, name);
}

//Funcion para editar un juego
void editGame(games *game, TreeMap *nameMap ,TreeMap *valuationMap, TreeMap *costMap){
  removeNode(nameMap, nameMap->current);
  
  char name[50];
  char releaseDate[30];
  int valuation, cost;

  printf("Ingrese el nuevo nombre:\n");
  scanf("%100[^\n]s", name);

  getchar();

  printf("Ingrese la nueva fecha de lanzamiento:\n");
  scanf("%100[^\n]s", releaseDate);

  printf("Ingrese la nueva valuacion:\n");
  scanf("%d", &valuation);

  printf("Ingrese el nuevo precio\n");
  scanf("%d", &cost);

  strcpy(game->name, name);
  strcpy(game->releaseDate, releaseDate);
  game->valuation = valuation;
  game->cost = cost;

  insertTreeMap(nameMap, game->name, game);
  list_exists(valuationMap, costMap, game);
  
}

//Funcion para borrar un juego
void deleteFromList(TreeMap *map, char *name){
  Pair *aux = firstTreeMap(map);
  List *list = aux->value;
  do{
    List *list = aux->value;
    char *id = firstList(list);
    do{
      if(strcmp(id, name) == 0){
        popCurrent(list);
      }
      id = nextList(list);
    }while (id != NULL);
    aux = nextTreeMap(map);
  }while(aux != NULL);
}

//Imprime por año
void print_byAge(TreeMap *JuegosVal){
  char age[30];
  printf("Ingrese el anio a buscar\n");
  getchar();
  scanf("%100[^\n]s", age);

  Pair * aux = lastTreeMap(JuegosVal);
  int gamePrinted = 0, valuationSearched;
  while(aux!=NULL){
    List * juegos = aux->value;
    games * game = lastList(juegos);
    while(game!=NULL){  
      if(game->releaseDate[6] == age[0] && game->releaseDate[7] == age[1] && game->releaseDate[8] == age[2] && game->releaseDate[9] == age[3]){
        if (gamePrinted == 0){
          printf("%s\n",game->name);
          gamePrinted++;
          valuationSearched = game->valuation;
        }else{
          if (valuationSearched == game->valuation){
            printf("%s\n",game->name);
            gamePrinted++;
          }
        }  
      }
      game = prevList(juegos);
    }
    aux= prevTreeMap(JuegosVal);
  }
  if(gamePrinted > 1) printf("Todos con una valoracion de %d\n", valuationSearched);
  else if (gamePrinted == 1)printf("Con una valoracion de %d\n", valuationSearched);
  else printf("Ano no encontrado\n");
}


//Exporta el archivo
void export_csv(TreeMap *nameMap){
  char fileName[30];
  printf("Ingrese Nombre para el archivo (Recuerde la extencion .csv)\n");
  scanf("%s",&fileName);
  FILE *outFile = fopen(fileName, "w");
  if(outFile == NULL) printf("Error al crear el archivo, termine el programa\n");
  else{
    fprintf(outFile,"Nombre,año de salida,valoracion,precio\n");

    Pair *pr = firstTreeMap(nameMap);
    do{
      games *data = pr->value;
      fprintf(outFile,"%s, %s, %d, %d\n",data->name, data->releaseDate, data->valuation, data->cost);
      pr = nextTreeMap(nameMap);
    }while(pr != NULL);
    printf("Archivo creado con exito!!\n");
  }
}
