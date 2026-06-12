#include <stdio.h>
#include <stdlib.h>

struct point
{
  double val;
  struct point* next;
};

struct stack
{
  struct point* first;
  size_t size;
};

int push(struct stack* stk, double val);
int pop(struct stack* stk, double* val);
int destroyStack(struct stack** stk);
int readHead(struct stack*, double* val);

int main()
{


  return 0;
}

int push(struct stack* stk, double val)
{
  if(!stk) return -1;

  struct point* newPoint = (struct point*)malloc(sizeof(struct point));
  if(!newPoint) return 2;

  newPoint->val = val;
  newPoint->next = stk->first; // if new stack than NULL
  stk->first = newPoint;

  stk->size++;

  return 0;
}

int pop(struct stack* stk, double* val)
{
  if(!stk) return -1;
  if(!(stk->size)) return -1;

  *val = stk->first->val;
  struct point* fHead = stk->first;

  stk->first = stk->first->next; // if one elem than NULL
  stk->size--;

  free(fHead);

  return 0;
}

int destroyStack(struct stack* stk)
{
  if(!stk) return 1;

  struct point* fHead = NULL;
  while(stk->first)
  {
    fHead = stk->first;
    stk->first = stk->first->next;
    stk->size--;

    free(fHead);
  }

  return 0;
}

int readHead(struct stack* stk, double* val)
{
  if(!stk) return -1;
  if(!(stk->first)) return -1;

  *val = stk->first->val;

  return 0;
}
