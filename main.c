#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"
#include "minheap.h"

#define DELAY 100
#define SHOW_PROCESSING 1

#define ALG 0   // 0 A*, 1 DIJKSTRA, 2 FLOYD-WARSHALL

int distancia_manhattan (int x_atual, int x_final, int y_atual, int y_final)
{
    return abs(x_final-x_atual) + abs(y_final-y_atual);
}

int a_estrela(Heap* heap, MazeDef* maze, PlayerDef* player, int visited[maze_getGraphV(maze)], int distancia[maze_getGraphV(maze)], int vertices[maze_getGraphV(maze)])
{
    int state, v, w;
    
    if(heap_vazia(heap))
        return 0;

    state = 0;

    v = minheap_remove(heap);
    
    if(!visited[v]){
        visited[v] = 1;
    
        player->current_vertex = v;
        player->current_y = vertex_to_map_y(player->current_vertex, maze_getFileCols(maze));
        player->current_x = vertex_to_map_x(player->current_vertex, maze_getFileCols(maze));
        player->steps++;
        
        if(SHOW_PROCESSING) {
            display(player, maze);
            printf(" >> Executando A*\n");
            
            msleep(DELAY);
        }
    
        // chegou no final do labirinto
        if(maze_getFileTile(maze, player->current_y, player->current_x) == 'F') 
            return 1;
    
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
                else if (casa == '.')
                    custo_casa = 1;
                
                nova_distancia = distancia[v] + custo_casa;
                
                // distancia entre a posicao atual e o final
                heuristica = distancia_manhattan(vertex_to_map_x(w, maze_getFileCols(maze)), maze_getFinishX(maze), 
                vertex_to_map_y(w, maze_getFileCols(maze)), maze_getFinishY(maze));
                
                if(nova_distancia < distancia[w])
                {
                    distancia[w] = nova_distancia;
                    vertices[w] = v;
                }
                minheap_insere(heap, nova_distancia + heuristica, w);
            }
        }
    }
    state = a_estrela(heap, maze, player,visited, distancia, vertices);
    
    return state;
}




int main()
{
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
    
    /* A* */    
    if (ALG == 0)
    {
        Heap* heap = heap_cria (maze_getGraphV(maze)*10);
        int distancia[maze_getGraphV(maze)];
        int vertices[maze_getGraphV(maze)];
    
        for (w = 0; w < maze_getGraphV(maze); w++){
            distancia[w] = 10000;
            vertices[w] = -1;
        }
            
        distancia[player->current_vertex] = 0;
        minheap_insere(heap, 0, player->current_vertex);
        
        t = clock();
        if(a_estrela(heap, maze, player, visited, distancia, vertices))
        {
            t = clock() - t; 
            display(player, maze);
            printf("Final encontrado c/ A* em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
            printf("Custo do caminho mais curto: %d\n", distancia[map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze))]);
            
            v = map_to_vertex(maze_getStartY(maze), maze_getStartX(maze), maze_getFileCols(maze));
            w = map_to_vertex(maze_getFinishY(maze), maze_getFinishX(maze), maze_getFileCols(maze));

            maze_print_path(maze, w);
            while(v != w){
                w = vertices[w];
                maze_print_path(maze, w);    
            }
    
        }
        else
        {
            t = clock() - t; 
            display(player, maze);
            printf("Não encontrado c/ A* em %f segundos\n", ((double)t)/CLOCKS_PER_SEC);
        }
    
    }
    /* DIJKSTRA */   
    else if (ALG == 1)
    {
        /*inserir codigo*/
    }
    /* FLOYD-WARSHALL */
    else if (ALG == 2)
    {
        /*inserir codigo*/
    }

    return 0;
}

