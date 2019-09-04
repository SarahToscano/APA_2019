#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

typedef struct{
  int vehicles;
  int capacity;
}info_test;

typedef struct{
    int *rota;
    int custo;
    int cap;
}veiculo;

typedef struct{
    int custo;
    int posiVertice;
    int carro1;
    int carro2;
    int posiAresta1;
    int posiAresta2;
    int vertice;
}vnd;


void shift(veiculo *car, info_test description, int numVertices, int matriz[][numVertices], int *demanda,int *count){
    int i, j, j2, k, l, l2, m;
    int ver1, ver2, atualdif = 0;
    vnd dif;
    dif.custo = 0;
    int flagMelhora = 0;
    for(i = 0; i < description.vehicles; i++){
      //  printf("carro1: %d\n", i);
        for(j = 1; j < count[i]; j++){
        //    printf("\tvertice de posicao %d: %d\n",j, car[i].rota[j]);
            for(k = 0; k < description.vehicles; k++){
             //   printf("\t\tcarro2: %d, demanda do vertice: %d vs capacidade do carro2: %d\n", k, demanda[car[i].rota[j]], car[k].cap);
                if(i != k && demanda[car[i].rota[j]] <= car[k].cap){
                    for(l = 0 , l2 = 1; l2 < count[k] ; l++, l2++){
                     //   printf("carro1: %d, vertice de posicao %d: %d, carro2: %d, demanda do vertice: %d vs capacidade do carro2: %d, aresta %d - %d\n", i, j, car[i].rota[j], k, demanda[car[i].rota[j]], car[k].cap, car[k].rota[l], car[k].rota[l2]);

                        j2 = car[i].rota[j];// vertice em questao

                        //ALTERAR A ROTA DE ORIGEM
                        ver1 = car[i].custo;
                        ver1 -= matriz[car[i].rota[j-1]][j2];
                        ver1 -= matriz[j2][car[i].rota[j+1]];
                        ver1 += matriz[car[i].rota[j-1]][car[i].rota[j+1]];
                        

                        //ALTERAR A ROTA DE DESTINO
                        ver2 = car[k].custo;
                        ver2 -= matriz[car[k].rota[l]][car[k].rota[l2]];
                        ver2 += matriz[car[k].rota[l]][j2];
                        ver2 += matriz[j2][car[k].rota[l2]];

                        atualdif = (car[i].custo + car[k].custo) - (ver2 + ver1);
                      //  printf("dif: %d vs atualdif: %d\n", dif.custo, atualdif);
                        if(atualdif > dif.custo){
                            printf("melhora!!\n");
                            flagMelhora = 1;
                            dif.custo = atualdif;
                            dif.carro1 = i;
                            dif.carro2 = k;
                            dif.posiAresta1 = l;
                            dif.posiAresta2 = l2;
                            dif.posiVertice = j;
                            dif.vertice = j2;
                        }
                    }
                }
            }
        }
    }
 //   printf("teste\n");
    if(flagMelhora){
    //    printf("teste, dif.posiVertice: %d, count[dif.carro1]: %d\n",dif.posiVertice, count[dif.carro1] );
        car[dif.carro1].cap = description.capacity;
        car[dif.carro2].cap = description.capacity;
        car[dif.carro1].custo = 0;
        car[dif.carro2].custo = 0;
        for(m = dif.posiVertice; m <= count[dif.carro1]-1; m++){
        //    printf("vertice indo embora: %d\n", car[dif.carro1].rota[m] );
            car[dif.carro1].rota[m] = car[dif.carro1].rota[m+1];
        } 
        count[dif.carro1]--;

        for(m = 0; m < count[dif.carro1]; m++){
            car[dif.carro1].cap -= demanda[car[dif.carro1].rota[m+1]];
            car[dif.carro1].custo += matriz[car[dif.carro1].rota[m]][car[dif.carro1].rota[m+1]];
        }

        for(m = count[dif.carro2]+1; m > dif.posiAresta2; m--){
            car[dif.carro2].rota[m] = car[dif.carro2].rota[m-1];
        }
        car[dif.carro2].rota[m] = dif.vertice; 
        count[dif.carro2]++;

        for(m = 0; m < count[dif.carro2]; m++){
            car[dif.carro2].cap -= demanda[car[dif.carro2].rota[m+1]];
            car[dif.carro2].custo += matriz[car[dif.carro2].rota[m]][car[dif.carro2].rota[m+1]];
        }

        shift(car,description, numVertices, matriz, demanda, count);

    }
}


void createRota(  veiculo *car, info_test description, int numVertices, int matriz[][numVertices], int *demanda, int *count){
    int disp[numVertices];
    int contRota = 0;
    int i = 0, j, x, w = 1;

    for(i = 0; i < numVertices; i++) disp[i] = 1;

    for(i = 0; i < description.vehicles; i++){
        car[i].cap = description.capacity;
        car[i].rota[0] = 0;
        count[i] = 1;
        car[i].custo = 0;
    }
    i = 0;
    while (i < description.vehicles){
     //   printf("Carro[%d]: ", i);
        while(w < numVertices){
            //printf("demanda do vertice: %d vs capacidade do veiculo %d: %d\n", demanda[w], i, cap[i]);

            if(disp[w] && demanda[w] <= car[i].cap){ 
                car[i].rota[count[i]] = w;
                disp[w] = 0;
                x = car[i].rota[count[i]-1];
                //printf(" aresta %d-%d: %d\n",x,w,matriz[x][w]);
                car[i].custo += matriz[x][w];
                //printf("cap do veiculo %d antes %d\n",i, car[i].cap);
                car[i].cap = car[i].cap - demanda[w];
                //printf("cap do veiculo %d depois %d\n",i, car[i].cap);
            //  printf("carro[%d].rota[%d] = %d \n",i, count[i], car[i].rota[count[i]]);
            //   puts("");
            /*
                if(matriz[x][w]>maior[i][0]){
                    maior[i][0]=matriz[x][w];
                    maior[i][1]=x;
                    maior[i][2]=w;
                    printf("Maior peso: %d, arestas: %d-%d\n", maior[i][0], maior[i][1], maior[i][2]);

                }
                */
                count[i]++;
                
                    

                
            }
            w++;
        }
        puts("");
        w = 1;
        i++;
    }
  
    for(i = 0; i < description.vehicles; i++){
 
        car[i].rota[count[i]] = 0;
        car[i].custo += matriz[car[i].rota[count[i]-1]][0];
    } 
    
    //SO PRA PRINTAR MESMO
    for(i = 0; i < description.vehicles; i++){
        printf("carro [%d] - Vertices: ", i);
        for(j = 0; j <= count[i]; j++){
            printf("%d, ",car[i].rota[j]);
         //   printf("Trocando os de maiores custos...\n");
        }
        printf("tam: %d, custo: %d, capacidade restante: %d\n",count[i], car[i].custo, car[i].cap);
    }
    shift(car,description, numVertices, matriz, demanda, count);

    for(i = 0; i < description.vehicles; i++){
        printf("carro [%d] - Vertices: ", i);
        for(j = 0; j <= count[i]; j++){
            printf("%d, ",car[i].rota[j]);
         //   printf("Trocando os de maiores custos...\n");
        }
        printf("tam: %d, custo: %d, capacidade restante: %d\n",count[i], car[i].custo, car[i].cap);
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
    strcpy(nomeArq, test2);
    char vert[2];
    vert[0] = nomeArq[3];
    vert[1] = nomeArq[4];
    int numVertices = atoi(vert);
    file_test = fopen(nomeArq,"r");

    if(file_test== NULL)
        printf("Erro ao abrir o arquivo: %s.", nomeArq);

    char *line = NULL;
    size_t len = 0;
    int i, j;
    char *aux = NULL;
    int flagDeman = 0;
    int demanda[numVertices+1], demanda1[numVertices];
    int contDemanda = 0;
    int flagAd = 0;
    int matriz[numVertices][numVertices];
    int contLinha = 0;
    int contColun = 0;
    
    while ((getline(&line, &len, file_test)) != -1) {
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

    int count[description.vehicles];
    createRota( car, description, numVertices, matriz, demanda1, count);

    for(i = 0; i < description.vehicles; i++) free(car[i].rota);
   return 0;
}
