#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NoLib.h"
#include "DistLib.h"

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

#define K_VIZINHOS 3//vizinhos que serão avalidados para determinar o valor do atributo alvo
#define BETA 1//utilizado no calculo de Kbeta_measure

float acuracia_Fbeta[K_FOLDS][2];

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
    FILE *fonte, *metricas, *detalhes;
    char linha[100];
    
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

    if( (fonte = fopen("/home/orgb/gem5/emb_exemplo/K-NearestNeighborsENXUTO/diabetes.csv", "r")) == NULL ){
        printf("ERRO AO ABRIR O ARQUIVO\n");
        return 1;
    }
    if( fgets(linha, sizeof(linha), fonte) == NULL ){
        printf("ERRO AO LER A LINHA");
        return 1;
    }

    printf("\nAnalisando: %s\n", linha);
    fprintf(detalhes, "\nAnalisando: %s\n", linha);

    //LÊ OS DADOS DO ARQUIVO diabetes.cvs
    insere_dados(&lista, fonte);

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

    fclose(fonte);
    fclose(detalhes);
    fclose(metricas);
    printf("FIMMMMMMMMMMMM");
    return 0;
}
