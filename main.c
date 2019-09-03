#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/time.h>

typedef struct{
  int dimension;
  int vehicles;
  int capacity;
}info_test;

typedef struct{
    int *rota;
    int custo;
}veiculo;

void createRota(  veiculo *car, info_test description, int numVertices, int matriz[][numVertices], int *demanda){
    int disp[numVertices];
    int contRota = 0;
    int i = 0, j, x, w = 1, count[description.vehicles];
    int cap[description.vehicles];

    for(i = 0; i < numVertices; i++) disp[i] = 1;

    for(i = 0; i < description.vehicles; i++){
        cap[i] = description.capacity;
        car[i].rota[0] = 0;
        count[i] = 1;
        car[i].custo = 0;
    }
    i = 0;
    while (i < description.vehicles){
        while(w < numVertices){
            //printf("demanda do vertice: %d vs capacidade do veiculo %d: %d\n", demanda[w], i, cap[i]);
            if(disp[w] && demanda[w] <= cap[i]){ 
                car[i].rota[count[i]] = w;
                disp[w] = 0;
                car[i].custo += matriz[car[i].rota[count[i]-1]][w];
                //printf("cap do veiculo %d antes %d\n",i, cap[i]);
                cap[i] = cap[i] - demanda[w];
                //printf("cap do veiculo %d depois %d\n",i, cap[i]);
            //  printf("carro[%d].rota[%d] = %d \n",i, count[i], car[i].rota[count[i]]);
            //   puts("");
                count[i]++;
                
            }
            w++;
        }
        w = 1;
        i++;
    }
  
    for(i = 0; i < description.vehicles; i++){
 
        car[i].rota[count[i]] = 0;
    } 

    for(i = 0; i < description.vehicles; i++){
        printf("carro [%d] - Vertices: ", i);
        for(j = 0; j <= count[i]; j++){
            printf("%d ", car[i].rota[j]);
        }
        printf("\nCusto Maximo: %d -> Custo Atual: %d\n", description.capacity, car[i].custo);
        if((car[i].custo-description.capacity)>0){
            printf("Precisa ser recalibrado\n\n");
        }
        else{
            puts("");
        }
    }

}




int main(void){
//    struct timeval stop, start;
//    gettimeofday(&start, NULL);
    info_test description;

    FILE *file_test;

    const char test1[] = "P-n19-k2.txt";
    const char test2[] = "P-n16-k8.txt";
    const char test3[] = "P-n20-k2.txt";
    const char test4[] = "P-n23-k8.txt"; 
    const char test5[] = "P-n45-k5.txt";
    const char test6[] = "P-n50-k10.txt"; 
    const char test7[] = "P-n51-k10.txt"; 
    const char test8[] = "P-n55-k7.txt";

    char nomeArq[14];
    strcpy(nomeArq, test8);
    char vert[2];
    vert[0] = nomeArq[3];
    vert[1] = nomeArq[4];
    int numVertices = atoi(vert);
    description.dimension = numVertices;
    file_test = fopen(nomeArq,"r");

    if(file_test== NULL)
        printf("Erro ao abrir o arquivo: %s.", nomeArq);

    char *line = NULL;
    size_t len = 0;
    size_t read;
    int i, j;
    char *aux = NULL;
    int flagDeman = 0;
    int demanda[numVertices+1], demanda1[numVertices];
    int contDemanda = 0;
    int flagAd = 0;
    int matriz[numVertices][numVertices];
    int contLinha = 0;
    int contColun = 0;
    
    while ((read = getline(&line, &len, file_test)) != -1) {
        //printf("linha: %s:\n", line);       
        if (line[0] == 'V'){
            aux = &line[10];
            aux[strlen(aux) - 1] = '\0';
            description.vehicles = atoi(aux);
        //    printf("VEHICLES: %d\n", description.vehicles);
            
        }
        else if (line[0] == 'C'){
            aux = &line[10];
            aux[strlen(aux) - 1] = '\0';
            description.capacity = atoi(aux);
        //    printf("CAPACITY: %d\n", description.capacity);

        }
        //INICIO DA FORMACAO DO ARRAY DE DEMANDA
        else if(flagDeman){
            if(line[5] == ' '){
                aux = &line[6];
                aux[7] = '\0';
                demanda[contDemanda] = atoi(aux);
            }
            else{
                aux = &line[5];
                aux[7] = '\0';
                demanda[contDemanda] = atoi(aux);
            }
            contDemanda++;
            if (contDemanda > numVertices) flagDeman = 0;
        }
        else if (line[0] == 'D')
            flagDeman = 1;
        //FIM DO ARRAY DE DEMANDA

        //INICIO DA FORMACAO DE MATRIZ DE ADJACENCIA
        else if(flagAd){
            for(contColun = 0; contColun <= numVertices-1; contColun++){
                if (contLinha == contColun) 
                    matriz[contLinha][contColun] = 0;
                else if(contColun == 0){
                    if(line[2] == ' '){
                        aux = &line[3];
                        aux[1] = '\0';
                        matriz[contLinha][contColun] = atoi(aux);
                    
                    }
                    else{
                        aux = &line[2];
                        aux[2] = '\0';
                        matriz[contLinha][contColun] = atoi(aux);
                    }
                }
                else{
                    if(line[7 + 5*(contColun-1)] == ' '){//Numero de 1 digito
                        aux = &line[8 + 5*(contColun-1)];
                        aux[1] = '\0';
                        matriz[contLinha][contColun] = atoi(aux);
                    
                    }
                    else{
                        aux = &line[7 + 5*(contColun-1)];//numero de dois digitos
                        aux[2] = '\0';
                        matriz[contLinha][contColun] = atoi(aux);
                    }
                }
                
            }
            contColun = 0;
            contLinha++;
            if (contLinha >= numVertices) flagAd = 0;
        }
        else if (line[0] == 'E')  flagAd = 1;
        //FIM DA MATRIZ E ADJACENCIA
        
    }


    

//SO PRA PRINTAR MESMO

    for(i = 0; i < numVertices; i++){
        demanda1[i] = demanda[i+1];
     //   printf("vertice: %d, demanda: %d\n", i, demanda1[i]);
    }
    puts("\n");
    /*
    for(i = 0; i < numVertices; i++){
        for(j = 0; j < numVertices; j++){
            printf("%d ", matriz[i][j]);
        }
        puts("\n");
    }
 */
    fclose(file_test);
 //   gettimeofday(&stop, NULL);
 //   printf("%lu\n", stop.tv_usec - start.tv_usec);
 //   puts("Todos os arquivos de leitura para testes foram fechados.");

    veiculo car[description.vehicles];
    for(i =0; i <= description.vehicles; i++){
        car[i].rota = (int*)malloc(numVertices* sizeof(int));
    }    

    createRota( car, description, numVertices, matriz, demanda1);


    return 0;
}
