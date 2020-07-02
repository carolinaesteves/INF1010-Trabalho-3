#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"

#define ALG 2   // 0 A*, 1 DIJKSTRA, 2 FLOYD-WARSHALL

int main()
{
    int w;
    
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
      /*inserir codigo*/ 
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

