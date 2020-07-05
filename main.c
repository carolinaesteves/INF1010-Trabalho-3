#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"
#include "minheap.h"

#define DELAY 100
#define SHOW_PROCESSING 1

#define ALG 2  // 0 A*, 1 DIJKSTRA, 2 FLOYD-WARSHALL

int distancia_manhattan (int x_atual, int x_final, int y_atual, int y_final)
{
    return abs(x_final-x_atual) + abs(y_final-y_atual);
}

void a_estrela(Heap* heap, MazeDef* maze, PlayerDef* player, int visited[maze_getGraphV(maze)], int distancia[maze_getGraphV(maze)], int pre_vertice[maze_getGraphV(maze)])
{
    int v, w;

    v = minheap_remove(heap);
    
    if(!visited[v])
    {
        visited[v] = 1;
    
        player->current_vertex = v;
        player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
        player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
        player->steps++;
        
        if(SHOW_PROCESSING)
        {
            display(player, maze);
            printf(" >> Executando A*\n");
            
            msleep(DELAY);
        }
    
        // chegou no final do labirinto
        if(maze_getFileTile(maze, player->current_y, player->current_x) == 'F') 
            return;
    
        for (w = 0; w < maze_getGraphV(maze); w++)
        {
            if (maze_getGraphEdge(maze, v, w) && !visited[w])
            {
                char casa;
                int custo_casa, nova_distancia, heuristica;
                
                // coordenadas da casa
                casa = maze_getFileTile(maze, vertex_to_map_y(w, maze_getFileCols(maze)), vertex_to_map_x(w, maze_getFileCols(maze)));
                
                // custo caminho
                if (casa == 'M')
                    custo_casa = 100;
                else if (casa == 'R')
                    custo_casa = 5;
                else
                    custo_casa = 1;
                
                nova_distancia = distancia[v] + custo_casa;
                
                // distancia entre a posicao atual e o final
                heuristica = distancia_manhattan(vertex_to_map_x(w, maze_getFileCols(maze)), maze_getFinishX(maze), 
                vertex_to_map_y(w, maze_getFileCols(maze)), maze_getFinishY(maze));
                
                if(nova_distancia < distancia[w])
                {
                    distancia[w] = nova_distancia;
                    pre_vertice[w] = v;
                }
                minheap_insere(heap, nova_distancia + heuristica, w);
            }
        }
    }
    a_estrela(heap, maze, player,visited, distancia, pre_vertice);
}

void dijkstra(Heap* heap, MazeDef* maze, PlayerDef* player,  int distancia[maze_getGraphV(maze)], int pre_vertice[maze_getGraphV(maze)])
{
    if(!heap_vazia(heap))
    {
        int v, w;
        
        v = minheap_remove(heap);
    
        player->current_vertex = v;
        player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
        player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
        player->steps++;
        
        if(SHOW_PROCESSING) {
            display(player, maze);
            printf(" >> Executando Dijkstra\n");
            
            msleep(DELAY);
        }
    
        for (w = 0; w < maze_getGraphV(maze); w++)
        {
            if (maze_getGraphEdge(maze, v, w))
            {
                char casa;
                int custo_casa, result;
                
                casa = maze_getFileTile(maze, vertex_to_map_y(w, maze_getFileCols(maze)), vertex_to_map_x(w, maze_getFileCols(maze)));
                // custo caminho
                if (casa == 'M')
                    custo_casa = 100;
                else if (casa == 'R')
                    custo_casa = 5;
                else
                    custo_casa = 1;
                
                result = distancia[v] + custo_casa; 
                
                if(result < distancia[w]){
                    distancia[w] = result;
                    pre_vertice[w] = v;
    
                    minheap_atualiza(heap, result, w);
                }
            }
        }
    
        dijkstra(heap, maze,player,  distancia, pre_vertice);
    }
}

void floyd_warshall(MazeDef* maze, PlayerDef* player, int **distancia, int**prox){
    //distancia vai ser a matriz de saída que finalmente terá a menor distância entre cada par de vértices
    int i,j,k;

    //inicializar a matriz de solução
    for(i =0; i< maze_getGraphV(maze); i++){
        for(j=0;j<maze_getGraphV(maze);j++){
            if (i != j && maze_getGraphEdge(maze, i, j)){
                char t = maze_getFileTile(maze, vertex_to_map_y(j, maze_getFileCols(maze)), vertex_to_map_x(j, maze_getFileCols(maze)));
                int custo_vw = t  == 'M' ? 100 : (t  == 'R' ? 5 : 1);
                distancia[i][j] = custo_vw;
                prox[i][j] = j;
            }
        }
    }

    for (k=0; k < maze_getGraphV(maze); k++){
        for(i=0; i < maze_getGraphV(maze); i++){
            for(j=0; j < maze_getGraphV(maze); j++){
                if((distancia[i][k] != 10000 && distancia[k][j] != 10000) 
                   && distancia[i][j] > distancia[i][k] + distancia[k][j]){
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
                    prox[i][j] = prox[i][k];
                    player->steps++; 
                }
            }
        }
    }
    
}

int floyd_warshall_entre(int u, int v, PlayerDef* player, MazeDef* maze, int **prox){

   if(prox[u][v] == -1)
      return 0;

    player->current_vertex = u;
    player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
    player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
    //player->steps++;
    
    display(player, maze);
    printf(" >> Executando Floyd-Warshall\n");
    
    msleep(DELAY);
    
    while(u != v){
        u = prox[u][v];
       
        player->current_vertex = u;
        player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
        player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
        //player->steps++;
        
        display(player, maze);
        printf(" >> Executando Floyd-Warshall\n");
        
        msleep(DELAY);
   }
   
   return u == v;
}
int main(){
    int v, w;
    clock_t t;
    
    MazeDef* maze = maze_criaLabirinto();
    maze = maze_carregaLabirinto(maze);
    maze = maze_carregaMatrizAdjacencias(maze);
    
    PlayerDef* player = maze_criaPlayer();

    player->current_x = maze_getStartX(maze);
    player->current_y = maze_getStartY(maze);

    player->current_vertex = map_to_vertex(player->current_y, player->current_x, maze_getFileCols(maze));
    
    int visited[maze_getGraphV(maze)];
    
    for (w = 0; w < maze_getGraphV(maze); w++) // inicializa visitados    
        visited[w] = 0;    
    int distancia[maze_getGraphV(maze)];
    
    /* A* */    
    if (ALG == 0)
    {
        Heap* heap = heap_cria (maze_getGraphV(maze)*10);
        int pre_vertice[maze_getGraphV(maze)];
    
        for (w = 0; w < maze_getGraphV(maze); w++){
            distancia[w] = 10000;
            pre_vertice[w] = -1;
        }
            
        distancia[player->current_vertex] = 0;
        minheap_insere(heap, 0, player->current_vertex);
        
        t = clock();
        a_estrela(heap, maze, player, visited, distancia, pre_vertice);
        t = clock() - t; 
        
        display(player, maze);
        printf("Final encontrado c/ A* em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
        printf("Custo do caminho mais curto: %d\n", distancia[map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze))]);
            
        v = map_to_vertex(maze_getStartY(maze), maze_getStartX(maze), maze_getFileCols(maze));
        w = map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze));

        maze_print_path(maze, w);
        while(v != w)
        {
            w = pre_vertice[w];
            maze_print_path(maze, w);    
        }
    }
    
    /* DIJKSTRA */   
    else if (ALG == 1)
    {
        Heap* heap = heap_cria (maze_getGraphV(maze));
        int pre_vertice[maze_getGraphV(maze)];

        for (w = 0; w < maze_getGraphV(maze); w++)
        {
            int x = vertex_to_map_x(w, maze_getFileCols(maze));
            int y = vertex_to_map_y(w, maze_getFileCols(maze));
            
            if(free_vertex(maze_getFileTile(maze, y, x)))
                minheap_insere(heap, 10000, w);
    
            distancia[w] = 10000;
            pre_vertice[w] = -1;
        }
        
        distancia[player->current_vertex] = 0;
        minheap_atualiza(heap, 0, player->current_vertex);
        
        t = clock();
        dijkstra(heap, maze, player, distancia, pre_vertice);
        t = clock() - t; 
        
        display(player, maze);
        printf("Final encontrado c/ Dijkstra em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
        printf("Custo do caminho mais curto: %d\n", distancia[map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze))]);
         
        v = map_to_vertex(maze_getStartY(maze), maze_getStartX(maze), maze_getFileCols(maze));
        w = map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze));
        
        maze_print_path(maze, w);
        while(v != w && w > -1){
            w = pre_vertice[w];
            maze_print_path(maze, w);   
        } 
    }
    /* FLOYD-WARSHALL */
    else if (ALG == 2)
    {
        int **dist_fw = (int**)malloc(maze_getGraphV(maze) * sizeof(int *));
        for(v = 0; v < maze_getGraphV(maze); v++) dist_fw[v] = (int *)malloc(maze_getGraphV(maze) * sizeof(int));
        
        int **prox_fw = (int**)malloc(maze_getGraphV(maze) * sizeof(int *));
        for(v = 0; v < maze_getGraphV(maze); v++) prox_fw[v] = (int *)malloc(maze_getGraphV(maze) * sizeof(int));

        for (v = 0; v < maze_getGraphV(maze); v++){
            for (w = 0; w < maze_getGraphV(maze); w++){
                dist_fw[v][w] = 10000;
                prox_fw[v][w] = -1;
            }
        } 
        t = clock(); 
        floyd_warshall(maze, player, dist_fw, prox_fw);
        t = clock() - t; 
        if(floyd_warshall_entre(map_to_vertex(maze_getStartY(maze), maze_getStartX(maze), maze_getFileCols(maze)), map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze)), player, maze, prox_fw)){display(player, maze);
            printf("Final encontrado c/ Floyd-Warshall em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
            printf("Custo total: %d\n", dist_fw[map_to_vertex(maze_getStartY(maze),  maze_getStartX(maze),  maze_getFileCols(maze))]
                                            [map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze))]);
        }
        else{
            display(player, maze);
            printf("Não encontrado c/ Floyd-Warshall em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
        }
            
            
        v = map_to_vertex(maze_getStartY(maze), maze_getStartX(maze), maze_getFileCols(maze));
        w = map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze));
        
        maze_print_path(maze, v);
        while(v != w){
            v = prox_fw[v][w];
            maze_print_path(maze, v);    
        }
    }
    
    printf ("\033[H");
    printf("\033[%dB", maze_getFileRows(maze)+6);
    
    return 0;
}

