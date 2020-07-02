#include<stdio.h>
#include<stdlib.h>
#include "minheap.h"

struct _item {
   int prioridade; // distância
   int casa; // índice da casa
};

struct _heap {
    int max; /* tamanho maximo do heapidx */
    int pos; /* proxima posicao disponivel no vetor */
    Item *itens; /* vetor de itens */
    int* pos_casa; /* para corrigir a heap */
};

Heap* heap_cria(int max){
    int i;
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->max = max;
    heap->pos = 0;
    heap->itens = (Item *)malloc(max * sizeof(Item));
    heap->pos_casa = (int *)malloc(max * sizeof(int));
    
    for (i = 0; i < max; i++)
        heap->pos_casa[i] = -1;
    
    return heap;
}

static void troca(int a, int b, Heap* heap) {
    int c;
    
    Item i = heap->itens[a];
    heap->itens[a] = heap->itens[b];
    heap->itens[b] = i;

    c = heap->pos_casa[heap->itens[a].casa];
    heap->pos_casa[heap->itens[a].casa] = heap->pos_casa[heap->itens[b].casa];
    heap->pos_casa[heap->itens[b].casa] = c;
}

void heap_print(Heap *heap){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0; i < heap->pos; i++)
        printf("-> %d(%d) ", heap->itens[i].prioridade, heap->itens[i].casa);

    printf("->__/\\__\n");
}


void heap_print_indent(Heap *heap){
    int i, j = 1, bin = 1;
    
    printf("_________Print Heap Indent_________\n");
    for (i=0; i < heap->pos;i++) 
    {
        printf("%d(%d) ", heap->itens[i].prioridade, heap->itens[i].casa);
        if (j == bin)
        {
            bin = bin * 2;
            j = 0;
            printf("\n");
        }
        
        j++;
    }
    printf("\n");
}


void heap_libera (Heap * heap) {
    int i;
  free(heap->itens);   
  free(heap->pos_casa);
  free(heap);
}


static void corrige_acima(Heap* heap, int pos) {
    int pai;
    while (pos > 0){
        pai = (pos-1) / 2;
        if (heap->itens[pai].prioridade > heap->itens[pos].prioridade)
            troca(pos, pai, heap);
        else
            break;
        pos = pai;
    }
}

void minheap_insere (Heap *heap, int prioridade, int casa) {
  if (heap->pos >= heap->max) {
      printf("Heap CHEIO!\n");
      exit(1);
  }
  heap->itens[heap->pos].prioridade = prioridade;
  heap->itens[heap->pos].casa = casa;
  heap->pos_casa[casa] = heap->pos; 
  corrige_acima(heap,heap->pos);
  heap->pos++;
}

// heapify iterativo
static void corrige_abaixo(Heap* heap){
    int pai = 0;
    int filho_esq, filho_dir, filho;
    
    while (2 * pai+1 < heap->pos){
        filho_esq = 2 * pai+1;
        filho_dir = 2 * pai+2;

        if (filho_dir >= heap->pos) filho_dir = filho_esq;
        
        if (heap->itens[filho_esq].prioridade < heap->itens[filho_dir].prioridade)
            filho = filho_esq;
        else
            filho = filho_dir;
        
        if (heap->itens[pai].prioridade > heap->itens[filho].prioridade)
            troca(pai, filho, heap);
        else
            break;
            
        pai=filho;
    }
}


int minheap_remove(Heap *heap) {
  int casa;
  if (heap->pos == 0) return -1;
  casa = heap->itens[0].casa;

  heap->pos_casa[casa] = -1;
  heap->itens[0].casa = heap->itens[heap->pos-1].casa;
  heap->itens[0].prioridade = heap->itens[heap->pos-1].prioridade;
  heap->pos_casa[heap->itens[0].casa] = 0;
  heap->pos--;

  corrige_abaixo(heap);
  return casa;
}

int heap_vazia(Heap *heap){
    return heap->pos == 0;
}



int minheap_topo(Heap* heap){
    if(heap_vazia(heap))
        return -1;
    
    return heap->itens[0].casa;
}

int minheap_prioridade(Heap* heap, int casa){
    if(heap_vazia(heap)) return -1;
        
    if(heap->max <= casa)
        return -1;
    
    int pos = heap->pos_casa[casa];
    
    if(pos == -1)
      return -1;  

    return heap->itens[pos].prioridade;
}

int minheap_casa(Heap* heap, int casa){
    if(heap_vazia(heap))
        return -1;
        
    if(heap->max <= casa)
        return -1;
    
    int pos = heap->pos_casa[casa];
    if(pos == -1)
      return -1;  
    
    return heap->itens[pos].casa;
}

void minheap_atualiza (Heap *heap, int prioridade, int casa) {
    int pos = heap->pos_casa[casa];
    
    if(pos == -1)
    {
        minheap_insere (heap, prioridade, casa);
    }
    else
    {
        heap->itens[pos].prioridade = prioridade;
        
        troca(pos, heap->pos-1, heap);
        corrige_acima(heap, heap->pos-1);
    }
}