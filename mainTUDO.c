#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "NoLib.h"
//#include "DistLib.h"

/*Letícia dos Santos

Aprendizado de Máquina

Implementação K-NearestNeighbors, K vizinhos mais próximos.
VERIFIQUE O CAMINHO DOS ARQUIVOS QUE SERÃO UTILIZADOS

Arquivo principal:
-> Dados de "diabetes.cvs";
-> Informações detalhadas de cada teste em "detalhes.txt";
-> Acurácia e F1 de cada fold de teste em "metricas.cvs";

Constantes: NUM_CLASSES, TAM_LINHA, K_FOLDS em NoLib.c
*/
#define NUM_CLASSES 9
#define TAM_LINHA 100
#define K_FOLDS 5



#define K_VIZINHOS 3//vizinhos que serão avalidados para determinar o valor do atributo alvo
#define BETA 1//utilizado no calculo de Kbeta_measure

float acuracia_Fbeta[K_FOLDS][2];

////////////////////////////////////////////////////////////////
float dados[][NUM_CLASSES]={{6.000000,148.000000,72.000000,35.000000,0.000000,33.599998,0.627000,50.000000,1.000000},
{1.000000,85.000000,66.000000,29.000000,0.000000,26.600000,0.351000,31.000000,0.000000},
{8.000000,183.000000,64.000000,0.000000,0.000000,23.299999,0.672000,32.000000,1.000000},
{1.000000,89.000000,66.000000,23.000000,94.000000,28.100000,0.167000,21.000000,0.000000},
{0.000000,137.000000,40.000000,35.000000,168.000000,43.099998,2.288000,33.000000,1.000000},
{5.000000,116.000000,74.000000,0.000000,0.000000,25.600000,0.201000,30.000000,0.000000},
{3.000000,78.000000,50.000000,32.000000,88.000000,31.000000,0.248000,26.000000,1.000000},
{10.000000,115.000000,0.000000,0.000000,0.000000,35.299999,0.134000,29.000000,0.000000},
{2.000000,197.000000,70.000000,45.000000,543.000000,30.500000,0.158000,53.000000,1.000000},
{8.000000,125.000000,96.000000,0.000000,0.000000,0.000000,0.232000,54.000000,1.000000},
{4.000000,110.000000,92.000000,0.000000,0.000000,37.599998,0.191000,30.000000,0.000000},
{10.000000,168.000000,74.000000,0.000000,0.000000,38.000000,0.537000,34.000000,1.000000},
{10.000000,139.000000,80.000000,0.000000,0.000000,27.100000,1.441000,57.000000,0.000000},
{1.000000,189.000000,60.000000,23.000000,846.000000,30.100000,0.398000,59.000000,1.000000},
{5.000000,166.000000,72.000000,19.000000,175.000000,25.799999,0.587000,51.000000,1.000000},
{7.000000,100.000000,0.000000,0.000000,0.000000,30.000000,0.484000,32.000000,1.000000},
{0.000000,118.000000,84.000000,47.000000,230.000000,45.799999,0.551000,31.000000,1.000000},
{7.000000,107.000000,74.000000,0.000000,0.000000,29.600000,0.254000,31.000000,1.000000},
{1.000000,103.000000,30.000000,38.000000,83.000000,43.299999,0.183000,33.000000,0.000000},
{1.000000,115.000000,70.000000,30.000000,96.000000,34.599998,0.529000,32.000000,1.000000},
{3.000000,126.000000,88.000000,41.000000,235.000000,39.299999,0.704000,27.000000,0.000000},
{8.000000,99.000000,84.000000,0.000000,0.000000,35.400002,0.388000,50.000000,0.000000},
{7.000000,196.000000,90.000000,0.000000,0.000000,39.799999,0.451000,41.000000,1.000000},
{9.000000,119.000000,80.000000,35.000000,0.000000,29.000000,0.263000,29.000000,1.000000},
{11.000000,143.000000,94.000000,33.000000,146.000000,36.599998,0.254000,51.000000,1.000000},
{10.000000,125.000000,70.000000,26.000000,115.000000,31.100000,0.205000,41.000000,1.000000},
{7.000000,147.000000,76.000000,0.000000,0.000000,39.400002,0.257000,43.000000,1.000000},
{1.000000,97.000000,66.000000,15.000000,140.000000,23.200001,0.487000,22.000000,0.000000},
{13.000000,145.000000,82.000000,19.000000,110.000000,22.200001,0.245000,57.000000,0.000000},
{5.000000,117.000000,92.000000,0.000000,0.000000,34.099998,0.337000,38.000000,0.000000},
{5.000000,109.000000,75.000000,26.000000,0.000000,36.000000,0.546000,60.000000,0.000000},
{3.000000,158.000000,76.000000,36.000000,245.000000,31.600000,0.851000,28.000000,1.000000},
{3.000000,88.000000,58.000000,11.000000,54.000000,24.799999,0.267000,22.000000,0.000000},
{6.000000,92.000000,92.000000,0.000000,0.000000,19.900000,0.188000,28.000000,0.000000},
{10.000000,122.000000,78.000000,31.000000,0.000000,27.600000,0.512000,45.000000,0.000000},
{4.000000,103.000000,60.000000,33.000000,192.000000,24.000000,0.966000,33.000000,0.000000},
{11.000000,138.000000,76.000000,0.000000,0.000000,33.200001,0.420000,35.000000,0.000000},
{9.000000,102.000000,76.000000,37.000000,0.000000,32.900002,0.665000,46.000000,1.000000},
{2.000000,90.000000,68.000000,42.000000,0.000000,38.200001,0.503000,27.000000,1.000000},
{4.000000,111.000000,72.000000,47.000000,207.000000,37.099998,1.390000,56.000000,1.000000},
{3.000000,180.000000,64.000000,25.000000,70.000000,34.000000,0.271000,26.000000,0.000000},
{7.000000,133.000000,84.000000,0.000000,0.000000,40.200001,0.696000,37.000000,0.000000},
{7.000000,106.000000,92.000000,18.000000,0.000000,22.700001,0.235000,48.000000,0.000000},
{9.000000,171.000000,110.000000,24.000000,240.000000,45.400002,0.721000,54.000000,1.000000},
};//44 linhas
#define LINHAS_DADOS 44
//float menores[NUM_CLASSES-1]={0.000000,78.000000,0.000000,0.000000,0.000000,0.000000,0.134000,21.000000};///usa para normalizar os valores
//float maiores[NUM_CLASSES-1]={13.000000,197.000000,110.000000,47.000000,846.000000,45.799999,2.288000,60.000000};


/////////////////////////////BIBLIOTECAS//////////////////////////////////////
//NoLib


///usa para normalizar os valores
float menores[NUM_CLASSES-1] = {1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0};///usa para normalizar os valores
float maiores[NUM_CLASSES-1] = {0.0};

typedef struct no{
    float classe[NUM_CLASSES];
    struct no* prox;
}No;

No *criar_lista(void){
    return NULL;
}

void imprime_lista(No* lista){
    printf("-------------------\n");
    while (lista != NULL) {
        for(int i=0; i<NUM_CLASSES; i++)
            printf("\t%.3f", lista->classe[i]);
        printf("\n");
        lista = lista->prox;
    }
    printf("FIM DA LISTA\n");
}

void insere_comeco(No** lista, float dados[NUM_CLASSES]){
    No* no;

    no = (No*) malloc(sizeof(No));
    for(int i=0; i<NUM_CLASSES; i++)
        no->classe[i] = dados[i];
    no->prox = *lista;
    *lista = no;
}

void insere_dados(No **lista){//, FILE *fonte){
    //char linha[TAM_LINHA], num_ext[TAM_LINHA];

    int classe, i, cont;
    float num[NUM_CLASSES];

	//printf("NA FUNCAO Q LE O ARQ DE DADOS\n");

    for(cont=0;cont<LINHAS_DADOS;cont++){//analisa a tabela fonte

		for(classe=0; classe < NUM_CLASSES; classe++){
			num[classe]=dados[cont][classe];
		}

        for(i=0; i<NUM_CLASSES-1; i++){//encontra maiores e menores de cada classe
            if(num[i] < menores[i]) menores[i] = num[i];
            if(num[i] > maiores[i]) maiores[i] = num[i];
        }
        insere_comeco(lista, num);//coloca na lista
	//cont++;
    }
    return;
}

void divideKfolds(No **lista, No *fold[K_FOLDS]){
    No* no;
    No* diabetico;
    No* no_diabetico;
    No* n_diabetico;
    No* no_n_diabetico;
    //poderia ter feito vetores de lista, mas escolhi fazer listas separadas
    //para ficar mais legível e evitar problemas com ponteiros
    
    diabetico = criar_lista();
    n_diabetico = criar_lista();
    no=*lista;

    while (no != NULL){//divide a lista de acordo com o atributo alvo
        if(no->classe[8] == 1.0){
            insere_comeco(&diabetico, no->classe);            
        }else{
            insere_comeco(&n_diabetico, no->classe);
        }
        no=no->prox;
    }

    no_n_diabetico=n_diabetico;//distribui não diabeticos em K_FOLDS
    while(no_n_diabetico != NULL){
        for(int i=0; (i < K_FOLDS) && (no_n_diabetico != NULL); i++, no_n_diabetico=no_n_diabetico->prox){
            insere_comeco(&fold[i], no_n_diabetico->classe);
        }
    }

    no_diabetico=diabetico;//distribui diabeticos em K_FOLDS
    while(no_diabetico != NULL){
        for(int i=0; (i < K_FOLDS) && (no_diabetico != NULL); i++, no_diabetico=no_diabetico->prox){
            insere_comeco(&fold[i], no_diabetico->classe);
        }
    }
    return;
}

//DistLib
typedef struct dist{
    float distancia;    //entre a instância e seu vizinho
    int saida;          //atributo alvo
    struct dist* ant;
    struct dist* prox;
}Dist;

Dist *dist_criar_lista(void){
    return NULL;
}

void dist_imprime_lista(Dist* lista){
    printf("----------INÍCIO----------\n");
    while (lista != NULL) {
        printf("\t%f\t%d", lista->distancia, lista->saida);
        printf("\n");
        lista = lista->prox;
    }
    printf("--------FIM DA LISTA------\n");
}

void dist_imprime_listaFIM(Dist* lista){
    //fiz isso para conferir se o ant tava encadeado certo
    while (lista!=NULL && lista->prox != NULL) {
        lista = lista->prox;
    }//chega até o final da lista

    printf("--------FIM DA LISTA------\n");
    while (lista != NULL) {
        printf("\t%f\t%d", lista->distancia, lista->saida);
        printf("\n");
        lista=lista->ant;
    }
    printf("----------INÍCIO----------\n");
}

void dist_insere_comeco(Dist** lista, float distancia, int saida){
    Dist* no;

	//printf("tentado colocar %f\n",distancia);
    no = (Dist*) calloc(1, sizeof(Dist));//malloc(sizeof(Dist));
    no->distancia=distancia;
    no->saida=saida;
    no->ant = NULL;
    no->prox = *lista;

    if(*lista != NULL)
        (*lista)->ant=no;
    *lista = no;
}

int dist_pop_lista(Dist** lista){
    int saida;
    Dist *escolhido;

    escolhido=*lista;
    //printf("dist de %f com saida %d\n", escolhido->distancia, escolhido->saida);
    saida=escolhido->saida;

    if(escolhido-> ant == NULL){//primeiro elemento
        escolhido->prox->ant = NULL;
        *lista = escolhido->prox;
    }else{
        escolhido->ant->prox = escolhido->prox;
    }

    if (escolhido->prox != NULL)//ultimo elemento
        escolhido->prox->ant = escolhido->ant;

    free(escolhido);

    return saida;
}


void dist_destroi_lista(Dist** lista){
    Dist *aux1;
    Dist *aux2;

    aux1 = *lista;
    while (aux1 != NULL){
        aux2 = (Dist *)aux1->prox;
        free(aux1);
        aux1 =aux2;
    }
    *lista = NULL;
    return;
}

////////////////////////////////////////////////////////////////////

/*normaliza os valores de acordo com menores e maiores obtidos na insere_dados*/
void normaliza(No **lista){
    No* no;
    for(no=*lista; no != NULL; no=no->prox){
        for(int i=0; i < NUM_CLASSES-1; i++){
            no->classe[i]= (no->classe[i] - menores[i]) / (maiores[i] - menores[i]);
        }
    }
}

/*um fold escolhido para teste e o restante são treino, saida na tela e nos arquivos passados*/
void k_vizinhos(No *fold[K_FOLDS], int escolhido_teste, FILE *detalhes, FILE *metricas){
    No *teste;
    No *treino;
    Dist *euclidiana;
    Dist *elemento_eu;
    float soma_quadrados;
    int i, cont, VP=0, FN=0, FP=0, VN=0;

    float menor=1.0;
    int soma_saidas=0, cont2;
    Dist *elemento_menor;

    printf("\n-----------------------------------------------------------\nCONJUNTO DE TESTE fold: %d\n", escolhido_teste);
    fprintf(detalhes, "\n-----------------------------------------------------------\nCONJUNTO DE TESTE fold: %d\n", escolhido_teste);

    printf("CONJUNTO DE TREINO: ");
    fprintf(detalhes, "CONJUNTO DE TREINO: ");
    for(i=(escolhido_teste + 1) % K_FOLDS; i != escolhido_teste; i=((i+1) % K_FOLDS)){
        printf("fold %d\t",i);
        fprintf(detalhes, "fold %d\t",i);
    }
    printf("\n");
    fprintf(detalhes, "\n");

    //cria lista q irá receber as distâncias e atributos alvo relacionados
    euclidiana=dist_criar_lista();
    //printf("lista de dist criada\n");

    //COMPARA O CONJUNTO DE TESTE COM TREINO
    for(teste=fold[escolhido_teste]; teste != NULL; teste=teste->prox){//faz para cada instancia de teste
    //printf("faz para cada instancia de teste\n");
        for(cont=(escolhido_teste + 1) % K_FOLDS; cont != escolhido_teste ; cont=((cont+1) % K_FOLDS)){
            //percore todos os folds, menos o de teste            
            //printf("fold %d no treino\n",cont);
            for(treino=fold[cont]; treino != NULL; treino=treino->prox){
                for(i=0, soma_quadrados=0.0; i<(NUM_CLASSES-1); i++){
                    soma_quadrados+=pow(treino->classe[i] - teste->classe[i], 2);
                }
		//printf("coloca na lista euclidiana\n");
                dist_insere_comeco(&euclidiana, pow(soma_quadrados, 0.5), treino->classe[8]);
            }
        }

	//dist_imprime_lista(euclidiana);	

        //encontra os K vizinhos mais proximos, ou seja, os menores valores da euclidiana
        //OTIMIZAÇÃO: soma_saidas<1+(K_VIZINHOS/2) -> já tem a maioria dos votos, então não precisa buscar o resto
        for(cont2=0, soma_saidas=0; cont2<K_VIZINHOS && soma_saidas<1+(K_VIZINHOS/2); cont2++){
            elemento_menor=NULL;
            for(elemento_eu=euclidiana, menor=1.0; elemento_eu != NULL; elemento_eu=elemento_eu->prox){
                if(elemento_eu->distancia < menor){
                    elemento_menor = elemento_eu;
                    menor=elemento_eu->distancia;
                }
            }
            soma_saidas+=dist_pop_lista(&elemento_menor);//tira o menor da lista e guarda o valor do atributo alvo dele
        }

        if(soma_saidas >= 1+(K_VIZINHOS/2)){//2 ou mais diabéticos
            fprintf(detalhes, "DIABETICO-> ");
            printf("DIABETICO-> ");
            if(teste->classe[8] == 1.0){
                fprintf(detalhes, "acertou\n");
                printf("acertou\n");
                VP++;
            }else{
                fprintf(detalhes,"errou\n");
                printf("errou\n");
                FN++;
            }
        }else{            
            fprintf(detalhes, "NORMAL-> ");
            printf("NORMAL-> ");
            if(teste->classe[8] == 0.0){
                fprintf(detalhes, "acertou\n");
                printf("acertou\n");
                VN++;
            }else{
                fprintf(detalhes,"errou\n");
                printf("errou\n");
                FP++;
            }
        }
        //elimina a lista de distâncias
        dist_destroi_lista(&euclidiana);
    
    }
    //TERMINA DE PROCESSAR OS DADOS

    //METRICAS
    printf("VP: %d\tFN: %d\tFP: %d\tVN: %d\n", VP, FN, FP, VN);
    float n_testes, f1, f2;
    float acc, prec, rev, Fbeta;

    n_testes=VP+VN+FP+FN;

    acc=(VP+VN) /( (float) n_testes);
    acuracia_Fbeta[escolhido_teste][0]=acc;//guarda acurária
    f1=VP+FN;
    rev= VP / ( f1);
    f2=( VP+FP);
    prec= VP / f2;

    Fbeta=(1+ BETA*BETA) * (prec*rev) / (BETA*BETA*prec + rev);
    acuracia_Fbeta[escolhido_teste][1]=Fbeta;//guarda Fbeta
            
    fprintf(detalhes, "\t+\t-\t\n+\t%d\t%d\n-\t%d\t%d\n", VP, FN, FP, VN);
    fprintf(detalhes, "Acuracia: %.10f\tRev: %.10f\tPrec: %.10f\nF1: %.10f\n", acc, rev, prec, Fbeta);

    printf("Acuracia: %.10f\tRev: %.10f\tPrec: %.10f\nF1: %.10f\n", acc, rev, prec, Fbeta);

    fprintf(metricas, "%d;%f;%f\n", escolhido_teste, acc, Fbeta);

    return;
}

int main(){
    No* lista;
    No* folds[K_FOLDS];
    FILE *metricas, *detalhes;
	//FILE *fonte;
    //char linha[100];
    
    //CRIA LISTAS
    lista = criar_lista();
    for( int i=0; i<K_FOLDS; i++){
        folds[i] = criar_lista();
    }

    //ABRE ARQUIVOS
    if( (metricas = fopen("/home/orgb/gem5/emb_exemplo/K-NearestNeighborsENXUTO/metricas.csv", "w")) == NULL ){
        printf("ERRO AO ABRIR O ARQUIVO DE SAIDA\n");
        return 1;
    }
    fputs("Fold de teste/Interação;Acurácia;F1-measure\n", metricas);

    if( (detalhes = fopen("/home/orgb/gem5/emb_exemplo/K-NearestNeighborsENXUTO/detalhes.txt", "w")) == NULL ){
        printf("ERRO AO ABRIR O ARQUIVO DE SAIDA\n");
        return 1;
    }
    fputs("Detalhes do processo de treinamento\n", detalhes);

    /*if( (fonte = fopen("/home/orgb/gem5/emb_exemplo/K-NearestNeighborsENXUTO/diabetes.csv", "r")) == NULL ){
        printf("ERRO AO ABRIR O ARQUIVO\n");
        return 1;
    }
    if( fgets(linha, sizeof(linha), fonte) == NULL ){
        printf("ERRO AO LER A LINHA");
        return 1;
    }

    printf("\nAnalisando: %s\n", linha);
    fprintf(detalhes, "\nAnalisando: %s\n", linha);
*/
    //LÊ OS DADOS DO ARQUIVO diabetes.cvs
    insere_dados(&lista);

	printf("\nDADOS CARREGADOS\n");

    //NUMEROS QUE SERÃO USADOS NA NORMALIZAÇÃO
    fprintf(detalhes, "MENORES PARA NORMALIZAR\n");
    for(int i=0; i<NUM_CLASSES-1; i++){
        fprintf(detalhes, "%f\t", menores[i]);
    }
    fprintf(detalhes, "\nMAIORES PARA NORMALIZAR\n");
    for(int i=0; i<NUM_CLASSES-1; i++){
        fprintf(detalhes, "%f\t", maiores[i]);
    }

    normaliza(&lista);

    divideKfolds(&lista, folds);

    for(int i=0; i<1;i++){//realiza k_vizinhos no fold 0, colocar K_FOLDS no lugar do 1 para todos os folds
        k_vizinhos(folds, i, detalhes, metricas);
    }

    //fclose(fonte);
    fclose(detalhes);
    fclose(metricas);
    printf("FIMMMMMMMMMMMM");
    return 0;
}
