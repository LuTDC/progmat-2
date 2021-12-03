import gurobipy as gurobi
from gurobipy import GRB
import numpy as np

#Criando modelo e definindo limite de tempo
modelo = gurobi.Model("modelo")
modelo.setParam("TimeLimit", 3*60)
modelo.setParam("Presolve", 0)
modelo.setParam("Cuts", 0)

#Lendo numero de agentes e numero de tarefas do arquivo
aux = input()
num_agentes, num_tarefas = aux.split()
#domínio das variaveis
num_agentes = int(num_agentes)
num_tarefas = int(num_tarefas)

#Preparando leitura dos inputs

c = {}
a = {}
cap = {}

#Lendo a matriz de satisfação
for i in range(num_agentes):
    aux = input()
    aux = aux.split()
    for j in range(num_tarefas):
        c[i,j] = int(aux[j])

#Lendo a matriz de recursos (a)
for i in range(num_agentes):
    aux = input()
    aux = aux.split()
    for j in range(num_tarefas):
        a[i,j] = int(aux[j])

#Lendo linha com capacidades de cada agente (cap)
aux = input()
aux = aux.split()
for i in range(num_agentes):
    cap[i] = int(aux[i])


#Adicionando a variavel binaria X
xvar = {}
for i in range(num_agentes):
   for j in range(num_tarefas):
     xvar[i,j] = modelo.addVar(vtype=gurobi.GRB.BINARY, name='x'+'{'+str(i)+','+str(j)+'}')

# Construindo a funcao objetivo
modelo.setObjective(gurobi.quicksum(gurobi.quicksum(xvar[i,j]*c[i,j] for j in range(num_tarefas) ) for i in range(num_agentes)), GRB.MAXIMIZE)

# Adicionando restricao: cada tarefa eh feita por somente um agente
for j in range(num_tarefas):
    modelo.addConstr(gurobi.quicksum(xvar[i,j] for i in range(num_agentes)) == 1)


# Adicionando restricao: cada agente realiza tarefas de acordo com seus recursos
for i in range(num_agentes):
    modelo.addConstr(gurobi.quicksum(xvar[i,j]*a[i,j] for j in range(num_tarefas)) <= cap[i])

#Escrevendo no arquivo
#modelo.write("modelo.lp")

#Otimizando o modelo
modelo.optimize()


for v in modelo.getVars():
    index = int(str(v).find("value"))
    if (int(float((str(v))[index+6:-2]))) == 1:
        print(v)
print('Valor da funcao objetivo: %g' % modelo.objVal)
