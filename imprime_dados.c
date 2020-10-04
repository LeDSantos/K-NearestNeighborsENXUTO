#include <stdio.h>
#include <stdlib.h>

#define NUM_CLASSES 9
#define TAM_LINHA 100


///usa para normalizar os valores
//float menores[NUM_CLASSES-1] = {1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0};///usa para normalizar os valores
//float maiores[NUM_CLASSES-1] = {0.0};



int main(){
	FILE *fonte;
    char linha[TAM_LINHA], num_ext[TAM_LINHA];

    int classe, i, i_linha, i_num_ext, cont;
    float num[NUM_CLASSES];


    if( (fonte = fopen("/home/orgb/Desktop/K-NearestNeighborsENXUTO/diabetes.csv", "r")) == NULL ){
        printf("ERRO AO ABRIR O ARQUIVO\n");
        return 1;
    }
       

        if( fgets(linha, sizeof(linha), fonte) == NULL ){//lê linha
            printf("ERRO AO LER A LINHA");
            return -1;
        }
    printf("\nAnalisando: %s\n", linha);
	cont=0;
	printf("float dados[][NUM_CLASSES]={");
    while (!feof(fonte)){//analisa o arquivo fonte
        linha[0]='\0';
        if( fgets(linha, sizeof(linha), fonte) == NULL ){//lê linha
            //printf("ERRO AO LER A LINHA");
            break;
        }

	//printf("LEU LINHA\n");
		printf("{");
        i_num_ext=0;
        num_ext[0] = '\0';//zera o numero por extenso

        for(i_linha=0, classe = 0; classe < NUM_CLASSES; classe++){
			//printf("FOR DA LINHA\n");
            while(linha[i_linha] != ',' && linha[i_linha] != '\0'){
                num_ext[i_num_ext] = linha[i_linha];
				//printf("%c em i_linha: %d\n", linha[i_linha], i_linha);
                i_num_ext++;
                i_linha++;
            }
            i_linha++;
            num_ext[i_num_ext]='\0';
            num[classe] = atof(num_ext);//coleta cada numero
			//printf("Classe %d pronta\n",classe);
            
            i_num_ext=0;
            num_ext[0]='\0';
        }
		//printf("tTERMINOU FOR\n");
        for(i=0; i<NUM_CLASSES-1; i++){//encontra maiores e menores de cada classe
            if(num[i] < menores[i]) menores[i] = num[i];
            if(num[i] > maiores[i]) maiores[i] = num[i];
        }
        //insere_comeco(lista, num);//coloca na lista
		for(i=0; i<NUM_CLASSES-1; i++){			
			printf("%f,", num[i]);
		}
		printf("%f},\n",num[i]);
		cont++;
    }
		printf("};//%d linhas\n#define TAM_LINHAS %d\n",cont,cont);

/*
		printf("float menores[NUM_CLASSES-1]={");
		for(i=0; i<NUM_CLASSES-2; i++){			
			printf("%f,", menores[i]);
		}			
		printf("%f};///usa para normalizar os valores\n", menores[i]);

		printf("float maiores[NUM_CLASSES-1]={");
		for(i=0; i<NUM_CLASSES-2; i++){			
			printf("%f,", maiores[i]);
		}			
		printf("%f};\n", maiores[i]);
*/
	return 0;
}
