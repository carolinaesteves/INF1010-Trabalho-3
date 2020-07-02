# INF1010 - Trabalho 3 - Traçar rota

O Trabalho 3 consiste em implementar os algoritmos para calcular o caminho mais curto em um mapa de 40(lin)x 82(col). O mapa é uma representação de um desfiladeiro e não há bloqueios. Cada casa (vértice) permite trafegar nas 4 adjacências (norte, sul, leste e  oeste;  aka  cima  baixo,  direita  e  esquerda).Também há uma notação especifica(em caracteres)que define o custo do caminho:
* M= 100 (caminho montanhoso –necessário escalar)
* R = 5 (rochas, trilha sedimentada, demora um pouco mais,porém é fácil)
* . (ponto) = 1 (livre)
\
\
Também há duas marcações especiais: I (letra i) define a partida e F define o destino.

### Requisito do Trabalho:
Implementar os 3 algoritmos apresentados como pseudo-código e comparar seus resultados. Os algoritmos A* e Dijkstra são bem parecidos e pode-se usar como base a implementação da busca em largura ou profundidade. O algoritmo Floyd-Warshall é separado em 2 etapas: Cálculo e função para demonstrar o caminho entre pontos A e B. Ambos são apresentados no material.
\
\
Apresentar os resultados e comparação de:
* Caminho mais curto encontrado (que será o mesmo)
* Número de vértices visitados durante o cálculo. Repare que no caso do Floyd-warshall o loop principal é diferente do loop que recupera o caminho entre A e B
* Tempo gasto durante a execução do algoritmo.
\
\
O objetivo final será comparar as diferenças entre esses resultados.
