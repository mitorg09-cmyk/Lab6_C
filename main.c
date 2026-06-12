#include <stdio.h>

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
int destroy(struct stack** stk);
int readHead(struct stack*, double* val);

int main()
{


  return 0;
}

int push(struct stack* stk, double val)
{
  if(!stk) return -1;


}

int pop(struct stack* stk, double* val)
{
  if(!stk) return -1;
  if(!(stk->size)) return -1;



}

int destroy(struct stack** stk)
{
  if(!stk) return -1;
  if(!*stk) return 1;


}

int readHead(struct stack*, double* val)
{

}
