typedef struct _heap Heap;
typedef struct _item Item;

Heap* heap_cria(int max);

void heap_print(Heap *heap);

void heap_print_indent(Heap *heap);

void heap_libera(Heap *heap);

void minheap_insere (Heap *heap, int prioridade, int casa);

int minheap_remove(Heap* heap);

int heap_vazia(Heap *heap);

int minheap_topo(Heap* heap);

int minheap_prioridade(Heap* heap, int casa);

int minheap_casa(Heap* heap, int casa);

void minheap_atualiza (Heap *heap, int prioridade, int casa);