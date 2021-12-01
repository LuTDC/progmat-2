import gurobipy as gurobi
from gurobipy import GRB
import numpy as np

#Criando modelo e definindo limite de tempo
modelo = gurobi.Model("modelo")
modelo.setParam('TimeLimit', 3*60)

#Lendo numero de agentes e numero de tarefas do arquivo
aux = input()
num_agentes, num_tarefas = aux.split()
num_agentes = int(num_agentes)
num_tarefas = int(num_tarefas)

#Preparando leitura dos pesos

c = {}

#Transformando em matriz
for i in range(num_agentes):
    aux = input()
    aux = aux.split()
    for j in range(num_tarefas):
        c[i,j] = int(aux[j])


#Adicionando a variavel binaria X
xvar = {}
for i in range(num_agentes):
   for j in range(num_tarefas):
     xvar[i,j] = modelo.addVar(vtype=gurobi.GRB.BINARY, name='x'+'{'+str(i)+','+str(j)+'}')


# Construindo a funcao objetivo
modelo.setObjective(gurobi.quicksum(gurobi.quicksum(xvar[i,j]*c[i,j] for j in range(num_tarefas) ) for i in range(num_agentes)), GRB.MAXIMIZE)

# Add constraint: x + 2 y + 3 z <= 4
for i in range(num_agentes):
    modelo.addConstr(gurobi.quicksum(xvar[i,j] for j in range(num_tarefas)) == 1)

#Possibilidade de escrever o modelo
#modelo.write("modelo.lp")

#Otimizando o modelo
modelo.optimize()


for v in modelo.getVars():
    index = int(str(v).find("value"))
    if (int(float((str(v))[index+6:-2]))) == 1:
        print(v.varName)
print('Valor da funcao objetivo: %g' % modelo.objVal)
