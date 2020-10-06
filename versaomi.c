#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_CLASSES 3
#define K_VIZINHOS 3//vizinhos que serão avalidados para determinar o valor do atributo alvo

//glucose e insulina
float dados[][NUM_CLASSES]={
{148,33.6,1 },
{85,26.6,0 },
{183,23.3,1 },
{89,28.1, 0 },
{137,43.1,1 },
};

#define LINHAS_DADOS 5
/////////////////////////////BIBLIOTECAS//////////////////////////////////////
typedef struct dist{
    float distancia;    //entre a instância e seu vizinho
    short int saida;          //atributo alvo
}Dist;
/////////////////////////////////////////////////

Dist euclidiana[LINHAS_DADOS-1];

///usa para normalizar os valores
float menores[NUM_CLASSES-1] = {1000, 1000};
float maiores[NUM_CLASSES-1] = {0};

int main(){
    short int i, j, escolhido_teste, cont, i_eucli, votacao;
    float soma_quadrados;
    Dist menor;

    //acha maiores
    for(j=0; j< LINHAS_DADOS; j++){
        for(i=0; i<NUM_CLASSES-1; i++){//encontra maiores e menores de cada classe
            if(dados[j][i] < menores[i]) menores[i] = dados[j][i];
            if(dados[j][i] > maiores[i]) maiores[i] = dados[j][i];
        }
    }

    //normaliza dados
    for(j=0; j< LINHAS_DADOS; j++){
        for(i=0; i < NUM_CLASSES-1; i++){
            dados[j][i]= (dados[j][i] - menores[i]) / (maiores[i] - menores[i]);
        }
    }

    /*
    //DEPOIS TIRO ISSO
    //imprime
    for(j=0; j< LINHAS_DADOS; j++){
        for(i=0; i < NUM_CLASSES; i++){
            printf("%f ", dados[j][i]);
        }
        printf("\n");
    }*/


    //o primeiro ser� o teste
    escolhido_teste=0;
    for(i=escolhido_teste+1,i_eucli=0;i< LINHAS_DADOS; i++){
	    for(cont=0, soma_quadrados=0.0; cont<(NUM_CLASSES-1); cont++){
			soma_quadrados+=(dados[escolhido_teste][cont] - dados[i][cont])*(dados[escolhido_teste][cont] - dados[i][cont]);
		}
        //printf("soma dos quadrados de %d com %d eh %f\n", escolhido_teste, i, soma_quadrados);
	    euclidiana[i_eucli].distancia=sqrt(soma_quadrados);
        euclidiana[i_eucli].saida=dados[i][NUM_CLASSES-1];
        i_eucli++;
    }    
    
    /*
    for(i=0;i<((K_FOLDS-1)*LINHAS_DADOS/K_FOLDS);i++){
    	printf("%f -> %d\n", euclidiana[i].distancia, euclidiana[i].saida);
    }*/
    
    //ordenar euclidiana
    for(j=0;j<K_VIZINHOS;j++){
    	menor=euclidiana[j];
	    for(i=j+1;i<(LINHAS_DADOS-1);i++){
	    	if(euclidiana[i].distancia<menor.distancia){
	    		euclidiana[j]=euclidiana[i];
	    		euclidiana[i]=menor;
	    		menor=euclidiana[j];
	    	}
	    }
    }
    /*
    printf("eucli com %d menores ordenados\n", K_VIZINHOS);
    for(i=0;i<((K_FOLDS-1)*LINHAS_DADOS/K_FOLDS);i++){
    	printf("%f -> %d\n", euclidiana[i].distancia, euclidiana[i].saida);
    }
    */

    for(i=0, votacao=0;i<K_VIZINHOS;i++){
    	votacao+=euclidiana[i].saida;
    }
//    printf("RESULTADO votacao %d\n", votacao);

    switch(votacao){
    	case 0:
    	case 1://printf("NAO diabetico\n");
    			if(dados[escolhido_teste][NUM_CLASSES-1]==0.0){
    				//printf("ACERTOU\n");
                    return 1;
    			}else{
    				//printf("ERROU\n");
                    return 2;
    			}
    			//break;
    	case 2:
    	case 3://printf("diabetico\n");
    			if(dados[escolhido_teste][NUM_CLASSES-1]==1.0){
    				//printf("ACERTOU\n");
                    return 3;
    			}else{
    				//printf("ERROU\n");
                    return 4;
    			}
    			//break;    
    
    }
    	
    return 0;
}