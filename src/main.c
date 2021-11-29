/*
    Codigo que utiliza o Gurobi que calcula a solucao otima de um modelo dado
*/
#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"


void cria_ambiente(GRBenv *ambiente){
    GRBemptyenv(&ambiente);
    GRBsetstrparam(ambiente, "log", "modelo.log");
    GRBstartenv(ambiente);
}

void cria_modelo(GRBenv *ambiente, GRBmodel *modelo){
    // GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER, GRB_SEMICONT ou GRB_SEMIINT
    // GRBnewmodel(ambiente, modelo, "modelo", num_variaveis, objetivo, NULL, NULL, GRB_CONTINUOUS, variaveis);
    GRBnewmodel(ambiente, &modelo, "modelo", NULL,NULL,NULL,NULL,NULL,NULL);
    GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
}


//Modificar para leitura
void adiciona_variaveis(int num_variaveis, double *obj, int *tipo_variaveis){

    for(int i=0; i<num_variaveis; i++){
        obj[i] = 1; //scanf("%lf",&obj[i])
        tipo_variaveis[i] = GRB_CONTINUOUS;
    }

    GRBaddvars(model, num_variaveis, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
}

//Modificar para leitura
//GRB_LESS_EQUAL, GRB_GREATER_EQUAL, GRB_GREATER, GRB_LESS
void adiciona_restricoes(int num_restricoes, int num_variaveis, int *indices, double *coeficientes, int *sinais, int *valores){

    //int SINAIS[num_restricoes] = {GRB_LESS_EQUAL, GRB_LESS_EQUAL, GRB_GREATER_EQUAL, GRB_LESS};
    //ATENCAO... SE TIVER BUGADO, EH PQ A FUNCAO ADDCONSTR NAO COPIA VALOR, ELE USA O ENDERECO MSM, ENTAO ALOCA PRA CADA VEZ QUE LER

    for(int i=0; i<num_restricoes; i++)
        for(int j=0; j<num_variaveis; j++){
            indices[j] = j;
            coeficientes[j] = 1; //scanf("%lf",&coeficientes[i])
        }

    GRBaddconstr(model, num_variaveis, indices, coeficientes, sinais[i], valores[i], NULL);
    }

}

//Funcao que otimiza modelo
void otimiza_modelo(){
    GRBoptimize(model);
    GRBwrite(model, "modelo.lp"); //Nao serve pra nada
}

//Funcao que libera a memoria do programa
void libera_memoria(){
    GRBfreemodel(modelo);
    GRBfreeenv(ambiente);
    free(objetivo);
    free(tipo_variaveis);
    free(indices);
    free(coeficientes);
    free(sinais);
    free(valores);
}


int main(int   argc, char *argv[]){

    GRBenv   *ambiente = NULL;
    GRBmodel *modelo = NULL;

    int num_variaveis = 3;
    int num_restricoes = 4;

    double *objetivo = malloc(sizeof(double)*num_variaveis);

    int *tipo_variaveis = malloc(sizeof(int)*num_variaveis);

    int *indices = malloc(sizeof(int)*num_variaveis);

    double *coeficientes = malloc(sizeof(double)*num_variaveis);

    int *sinais = malloc(sizeof(int)*num_variaveis);

    double *valores = malloc(sizeof(double)*num_variaveis);

    cria_ambiente(ambiente):

    cria_modelo(modelo);

    adiciona_variaveis(num_variaveis, objetivo);

    adiciona_restricoes(num_restricoes, num_variaveis, indices, coeficientes, sinais, valores);

    otimiza_modelo();

    //Variaveis do processo de otimizacao
    int status_otimizacao;
    double valor_otimo;

    //Vetor de solucoes
    int *solucao = malloc(sizeof(int)*num_variaveis);


    GRBgetintattr(modelo, GRB_INT_ATTR_STATUS, &status_otimizacao);
    GRBgetdblattr(modelo, GRB_DBL_ATTR_OBJVAL, &valor_otimo);
    GRBgetdblattrarray(modelo, GRB_DBL_ATTR_X, 0, num_variaveis, solucao);


    if (optimstatus == GRB_OPTIMAL) {
        printf("%.2e\n", valor_otimo);

        for(int i=0; i<num_variaveis; i++){
            printf("%d ", solucao[i]);
        }
        printf(" !\n");

    }
    else if (optimstatus == GRB_INF_OR_UNBD) printf("Modelo é inviavel!\n");
    else printf("Otimizacao interrompida!\n");

    libera_memoria();

    return 0;
}
