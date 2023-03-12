#include "lab5.h"

// helper functions
typedef struct Heap {
  int *ptr;
  int max_el;
  int count_el;
} Heap;

void init_fancylist(Heap *fl, int starting_size);
void append(Heap *fl, int val);
Heap *initialize_heap(int root);
int get_parent_idx(Heap *heap, int idx);
void bubble_down(Heap *heap, int idx);
void bubble_up(Heap *heap, int idx);
void insert(Heap *heap, int val);
int pop(Heap *heap);
void plan_route_helper(Graph *gr, Vnode *start, Vnode *end);

void init_fancylist(Heap *fl, int starting_size) {
  fl->ptr = (int *)malloc(sizeof(int) * starting_size);
  fl->max_el = starting_size;
  fl->count_el = 0;
}

void append(Heap *fl, int val) {
  if (fl->count_el != fl->max_el) {
    fl->ptr[fl->count_el] = val;
    fl->count_el++;
  } else {
    fl->ptr = realloc(fl->ptr, (sizeof(int) * fl->max_el * 2));
    fl->ptr[fl->count_el] = val;
    fl->max_el = fl->max_el * 2;
    fl->count_el++;
  }
}

Heap *initialize_heap(int root) {
  Heap *heap = (Heap *)malloc(sizeof(Heap));
  init_fancylist(heap, 1);
  append(heap, root);
  return heap;
}

int get_parent_idx(Heap *heap, int idx) {
  if ((idx - 1) % 2 == 0) {
    return (idx - 1) / 2;
  } else {
    return (idx - 2) / 2;
  }
}

void bubble_down(Heap *heap, int idx) {
  /** left as an exercise */
  if (2 * idx + 1 > heap->count_el) {
    return;
  }
  int left_child = heap->ptr[2 * idx + 1];
  int right_child = heap->ptr[2 * idx + 2];
  int parent = heap->ptr[idx];

  if (left_child < right_child) {
    if (parent < left_child) {
      return;
    } else {
      heap->ptr[idx] = left_child;
      heap->ptr[2 * idx + 1] = parent;
      bubble_down(heap, 2 * idx + 1);
    }
  } else {
    if (parent < right_child) {
      return;
    } else {
      heap->ptr[idx] = right_child;
      heap->ptr[2 * idx + 2] = parent;
      bubble_down(heap, 2 * idx + 2);
    }
  }
  return;
}

void bubble_up(Heap *heap, int idx) {
  int parent_idx;
  int temp_swap;

  if (idx > 0) {
    parent_idx = get_parent_idx(heap, idx);
    if (heap->ptr[idx] < heap->ptr[parent_idx]) {
      temp_swap = heap->ptr[parent_idx];
      heap->ptr[parent_idx] = heap->ptr[idx];
      heap->ptr[idx] = temp_swap;
      bubble_up(heap, parent_idx);
    }
  }
}

void insert(Heap *heap, int val) {
  append(heap, val);
  bubble_up(heap, heap->count_el - 1);
}

int pop(Heap *heap) {
  /** left as an exercise*/
  int min_v = heap->ptr[0];
  heap->ptr[0] = heap->ptr[heap->count_el - 1];
  heap->ptr[heap->count_el - 1] = '\0';
  heap->count_el--;
  bubble_down(heap, 0);
  heap->ptr = realloc(heap->ptr, sizeof(int) * heap->count_el);
  return min_v;
}

char **plan_route(Graph *gr, char *start, char *dest) {
  int idx1 = 0;
  int idx2 = 0;

  for (int i = 0; i < gr->count; i++) {
    if (strcmp(start, gr->adj_list[i]->station) == 0) {
      idx1 = i;
    }
    if (strcmp(dest, gr->adj_list[i]->station) == 0) {
      idx2 = i;
    }
  }

  // 1. initialize all costs to inf
  for (int i = 0; i < gr->count; i++) {
    gr->adj_list[i]->cost = 100000;
    gr->adj_list[i]->visited = 0;
    gr->adj_list[i]->prev = NULL;
  }

  Vnode *starting = gr->adj_list[idx1];
  Vnode *ending = gr->adj_list[idx2];
  starting->visited = 1;
  starting->cost = 0;

  // 2. expand starting node
  // 4. repeat with recursive function
  plan_route_helper(gr, starting, ending);

  if (ending->prev == NULL) {
    return NULL;
  }

  char **path = (char **)calloc(gr->count, sizeof(char *));
  for (int i = 0; i < gr->count; i++) {
    path[i] = calloc(MAX_LEN, sizeof(char));
  }
  int i = 0;
  Vnode *curr = ending;
  while (strcmp(curr->station, starting->station) != 0) {
    strcpy(path[i++], curr->station);
    curr = curr->prev;
  }
  strcpy(path[i++], curr->station);

  return path;
}

void plan_route_helper(Graph *gr, Vnode *start, Vnode *end) {
  int not_inf = 0;

  if (start == NULL) {
    return;
  }
  if (strcmp(start->station, end->station) == 0) {
    return;
  }

  Enode *curr = start->edges;
  Heap *min_heap;
  int curr_smallest;
  Vnode *neighbors[gr->count];
  int j = 0;
  start->visited = 1;

  while (curr != NULL) {
    for (int i = 0; i < gr->count; i++) {
      // adding to min heap
      if (strcmp(curr->vertex, gr->adj_list[i]->station) == 0 &&
          gr->adj_list[i]->visited == 0) {
        if (gr->adj_list[i]->cost > start->cost + curr->weight) {
          gr->adj_list[i]->cost = start->cost + curr->weight;
          gr->adj_list[i]->prev = start;
        }

        for (int i = 0; i < gr->count; i++) {
          if (gr->adj_list[i]->visited == 0 &&
              gr->adj_list[i]->cost != 100000) {
            not_inf = 1;
          }
        }
        if (not_inf == 0) {
          return;
        }

        neighbors[j++] = gr->adj_list[i];
        if (curr == start->edges) {
          min_heap = initialize_heap(gr->adj_list[i]->cost);
        } else {
          insert(min_heap, gr->adj_list[i]->cost);
        }
      }
    }
    curr = curr->next;
  }

  for (int l = 0; l < j; l++) {
    curr_smallest = pop(min_heap);
    for (int k = 0; k < j; k++) {
      if (neighbors[k]->cost == curr_smallest) {
        plan_route_helper(gr, neighbors[k], end);
      }
    }
  }
  if (curr != start->edges) {
    free(min_heap);
  }
}

void add(Graph *gr, char *station) {
  for (int i = 0; i < gr->count; i++) {
    if (strcmp(station, gr->adj_list[i]->station) == 0) {
      return;
    }
  }

  Vnode *new_node = (Vnode *)malloc(sizeof(Vnode));
  strcpy(new_node->station, station);
  new_node->cost = 100000;
  new_node->edges = NULL;
  new_node->prev = NULL;

  gr->adj_list = realloc(gr->adj_list, (gr->count + 1) * sizeof(Vnode *));

  // add the node to the nodes array
  gr->adj_list[gr->count] = new_node;

  // increment the count of number of nodes
  gr->count++;
}

// change a directed edge within a graph
void update(Graph *gr, char *start, char *dest, int weight) {
  int idx1 = -1;
  int idx2 = -1;

  for (int i = 0; i < gr->count; i++) {
    if (strcmp(start, gr->adj_list[i]->station) == 0) {
      idx1 = i;
    }
    if (strcmp(dest, gr->adj_list[i]->station) == 0) {
      idx2 = i;
    }
  }

  if (idx1 == -1) {
    add(gr, start);
  }
  if (idx2 == -1) {
    add(gr, dest);
  }

  Enode *el1 = gr->adj_list[idx1]->edges;

  if (weight == 0) {
    Enode *curr = el1;
    Enode *prev = el1;
    while (curr->next != NULL) {
      if (strcmp(curr->vertex, dest) == 0) {
        if (prev == curr) {
          gr->adj_list[idx1]->edges = el1->next;
          curr->next = NULL;
          free(curr);
        } else {
          prev->next = curr->next;
          curr->next = NULL;
          free(curr);
        }
      }
      prev = curr;
      curr = curr->next;
    }
    if (strcmp(curr->vertex, dest) == 0) {
      if (prev == curr) {
        gr->adj_list[idx1]->edges = el1->next;
        curr->next = NULL;
        free(curr);
      } else {
        prev->next = curr->next;
        curr->next = NULL;
        free(curr);
      }
    }

  } else {
    if (el1 == NULL) {
      gr->adj_list[idx1]->edges = (Enode *)malloc(sizeof(Enode));
      el1 = gr->adj_list[idx1]->edges;
      strcpy(el1->vertex, dest);
      el1->weight = weight;
      el1->next = NULL;
    } else {
      Enode *curr = el1;
      while (curr->next != NULL) {
        if (strcmp(curr->vertex, dest) == 0) {
          curr->weight = weight;
          return;
        }
        curr = curr->next;
      }
      if (strcmp(curr->vertex, dest) == 0) {
        curr->weight = weight;
        return;
      }
      curr->next = (Enode *)malloc(sizeof(Enode));
      strcpy(curr->next->vertex, dest);
      curr->next->weight = weight;
      curr->next->next = NULL;
    }
  }
}

void disrupt(Graph *gr, char *station) {
  int idx = -1;
  for (int i = 0; i < gr->count; i++) {
    if (strcmp(station, gr->adj_list[i]->station) == 0) {
      idx = i;
    }
  }

  Vnode *to_remove = gr->adj_list[idx];
  Enode *curr = to_remove->edges;
  Enode *prev = to_remove->edges;
  while (curr != NULL) {
    if (strcmp(prev->vertex, curr->vertex) == 0) {
      curr = curr->next;
    } else {
      prev->next = NULL;
      free(prev);
      prev = curr;
      curr = curr->next;
    }
  }
  free(prev);
  free(to_remove);

  for (int i = idx + 1; i < gr->count; i++) {
    gr->adj_list[i - 1] = gr->adj_list[i];
    gr->adj_list[i] = NULL;
  }

  gr->count--;
  if (gr->count == 0) {
    gr->adj_list = NULL;
    return;
  }

  gr->adj_list = realloc(gr->adj_list, sizeof(Vnode *) * gr->count);

  for (int i = 0; i < gr->count; i++) {
    prev = gr->adj_list[i]->edges;
    curr = gr->adj_list[i]->edges;
    if (curr == NULL) {
      continue;
    }
    while (curr->next != NULL) {
      if (strcmp(curr->vertex, station) == 0) {
        if (strcmp(curr->vertex, prev->vertex) == 0) {
          gr->adj_list[i]->edges = curr->next;
          free(curr);
        } else {
          prev->next = curr->next;
          curr->next = NULL;
          free(curr);
          curr = prev->next;
        }
      } else {
        prev = curr;
        curr = curr->next;
      }
    }
    if (curr != NULL) {
      if (strcmp(curr->vertex, station) == 0) {
        gr->adj_list[i]->edges = NULL;
        free(curr);
      }
    }
  }
}
