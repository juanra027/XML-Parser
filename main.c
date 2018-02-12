#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
struct listaURL {
    char URL[200];
    char titulo[500];
    char detalle[500];
    char eliminado;
    struct listaURL *sig;
}*inicio;

void imprimir(struct listaURL *inicio){//imprime la estructura
    printf(" \n Empieza a imprimir");
    struct listaURL *temp=inicio;
    int cont = 1;
    while (temp != NULL){
        if(temp->eliminado == 'n'){
            printf("\n %d", cont);
            printf("%s", "- ");
            printf("%s", temp->titulo);
            printf("\n %s", temp->detalle);
            printf("\n");
            cont++;

        }
        temp = temp->sig;
    }
}
void borrar(){//descarta una estructura
    int numero;
    char asd;
    printf("%s", "Digite el id del numero que desea borrar: ");
    scanf("%d",&numero);getchar();
    numero--;
    struct listaURL *temp=inicio;
    while(numero != 0){
        if (temp->eliminado == 'n'){
            temp = temp->sig;
            numero--;
        }
        else{
            temp=temp->sig;
        }
    }
    temp->eliminado='y';


    printf("%s", "\n Desea borrar otro elemento?: ");
    scanf("%c",&asd);getchar();

    if(asd == 'y'){
        system("clear");
        imprimir(inicio);
        borrar();
    }

}
void guardarArchivo(struct listaURL *inicio){//crea el archivo con las estructuras validas
    FILE* file = fopen("data.txt", "w");
    struct listaURL *temp=inicio;
    while (temp != NULL){
        if(temp->eliminado == 'n'){
            fprintf(file,"-");
            fprintf(file, temp->titulo);
            fprintf(file, "\n");
            fprintf(file, temp->detalle);
            fprintf(file, "\n");
        }

        temp = temp->sig;
    }
    fclose(file);
}
void imprimirArchivo(){//imprime el archivo data.txt
    char string[500];
    char c;
    FILE* file = fopen("data.txt", "r");
    while ((c =fgetc(file)) != EOF)
    {
        printf("%c",c);
    }
}
struct listaURL* extraerInfoFinal(struct listaURL* cabeza,char archivo[],char* inicio,char* fin,char tipo)// extrae el abstract y el title
{
    FILE* file = fopen( archivo, "r");
    char detalleFinal[1000] = "";
    int count = 0;
    char encontrado = 0;
    char *palabraInicio = inicio;
    char *palabraFInal = fin;
    char c;
    int contChar = strlen(palabraInicio);
    while((c = getc(file)) != EOF){//recorre todo el txt
        if( c == palabraInicio[count] ){// pregunta si completo la palabra del inicio para saber si el puntero esta ubicado correctamente
            count++;

            if(count == contChar){
                    encontrado = 1; break;
            }
        }
        else{
            count = 0;
        }
    }

    //if(!found_it){printf("Did not find [HOCKEY] \n"); return "Not found";}

    count = 0; encontrado = 0;
    contChar = strlen(palabraFInal);
    while((c = getc(file)) != EOF){//continua recorriendo el txt, pero ahora se extrae la informacion
        char* detalleActual= &c;
        strcat(detalleFinal,detalleActual);
        if( c == palabraFInal[count] ){
            count++;

            if(count == contChar){encontrado = 1; break;}
        }
        else{
            count = 0;
        }
    }

    //if(!found_it){printf("%s","Did not find [done with HOCKEY] \n");}
    char* detalleFinalaux = strtok(detalleFinal,"<");
    if (tipo == 't'){

        strcpy(cabeza->titulo,detalleFinalaux);

    }

    else
        strcpy(cabeza->detalle,detalleFinalaux);

    fclose(file);
    return cabeza;

}
struct listaURL* capas(char archivo[], struct listaURL *inicio, char tipo)
{
    int cont = 0;
    char string[500];
    FILE* file = fopen( archivo, "r");//abre el archivo

    while (fgetc(file) != EOF)//pregunta si el archivo esta vacio
    {
        fgets(string, 1000, file);
        char* stringRecortado = strstr(string,"href"); //corta la cadena cuando encuentra el href
        if(stringRecortado != NULL)
        {
            stringRecortado = (strstr(stringRecortado,"www"));// corta la cadena cuando encuentra el www
            for (int i =0;i<strlen(stringRecortado);i++){ //for para eliminar el resto de la linea
                if(stringRecortado[i] == '"'){
                    for (int k = i;k<strlen(stringRecortado);k++){
                        stringRecortado[k] = NULL;
                    }
                    break;
                }
            }
            char link[] = "http://test1:test1@";
            strcat(link,stringRecortado); //le anade el usuario a el url para completarlo
            if(tipo == 'a'){
                struct listaURL *nn;
                nn = (struct listaURL *) malloc (sizeof(struct listaURL));
                nn->eliminado = 'n';
                nn->sig = NULL;
                strcpy(nn->URL,link);

                extraeInfo("data2.txt", link);
                nn = capas("data2.txt",nn,'b');
                nn=extraerInfoFinal(nn,"data3.txt","<title>","</title>",'t');
                nn=extraerInfoFinal(nn,"data3.txt","<abstract>","</abstract>",'a');
                if(inicio == NULL){
                    inicio = nn;
                    fin = nn;
                }
                else{
                    struct listaURL *temp = inicio;
                    while(temp->sig != NULL){
                        temp = temp->sig;
                    }
                    temp->sig= nn;
                    temp = nn;
                }
            }
            else if (tipo == 'b'){
                if(cont == 1){
                    extraeInfo("data3.txt",link);
                    break;
                }

                else
                    cont++;
            }
        }
    }
    fclose(file);
    return inicio;
}
void extraeInfo(char archivo[], char url[])
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL,url);
    FILE* file = fopen( archivo, "w");

    curl_easy_setopt( curl, CURLOPT_WRITEDATA, file) ;

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);

    fclose(file);
  }
}
int main(int argc, char *argv[])
{

    if(argc > 3)
    {
        printf("\n>> Error: EL maximo numero de argumentos es 3\n");
        exit(1);
    }
    char op [3];
    system("clear");
    char url[] = "http://test1:test1@www.idehn.tec.ac.cr/geoserver/rest/layers.xml";
    char archivo[] = "data1.txt";
    extraeInfo(archivo, url);
    inicio = capas(archivo, inicio,'a');
    printf("\n... Obteniendo información...\n");

    if (argc == 1)// Si solo ingresa el nombre
    {
        imprimir(inicio);
        borrar();
        guardarArchivo(inicio);
    }
    else if (argc == 2) //Si ingresa nombre y un -f p -p
    {
        if (strcmp(argv[1], "-f") == 0) // si devuelve 0 quiere decir que el parametro ingresado es igual
        {
            guardarArchivo(inicio);
            system("clear");
            printf("\n Los datos han sido almacenados en data.txt.\n");
        }
        else if (strcmp(argv[1], "-p") == 0)
        {
            imprimir(inicio);
            borrar();
            guardarArchivo(inicio);
            printf("\n Imprime el archivo data\n");
            imprimirArchivo();
            exit(1);
        }
        else
        {
            printf("\n Error: Debe ingresar como parametro '-p' o '-f'\n");
            exit(1);
        }
    }
    else if (argc == 3)
    {
        if ((strcmp(argv[1], "-f") == 0) & (strcmp(argv[2], "-p") == 0))
        {
            guardarArchivo(inicio);
            system("clear");
            printf("\n Los datos han sido almacenados en data.txt.\n");
            printf("\n Imprime el archivo data\n");
            imprimirArchivo();
        }
        else
        {
            printf("\n Error: Para utilizar el tecer parametro, debe ingresar '-f -p' \n");
            exit(1);
        }
    }
    else
    {
        printf("\n Error: Ademas del nombre, solo se espera maximo 2 parametros, '-f' o '-p' o '-f -p' \n");
        exit(1);
    }
    return 0;
}
