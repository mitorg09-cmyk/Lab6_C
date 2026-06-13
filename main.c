#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int destroyStack(struct stack* stk);
int readHead(struct stack*, double* val);
int parseFile(char* outStr, int* outTab, size_t* outLen, char* fileName);
int makeRPM(char* inStr, size_t len, char* outStr);

int main()
{
  int Tab[256] = {};
  char buff[2000] = {};
  size_t len = 0;

  printf("%d\n", parseFile(buff, Tab, &len, "test.txt"));
  printf(buff);
  printf("\n");
  printf("a = %d, b = %d\n", Tab[(int)'a'], Tab[(int)'b']);
  char RPMbuff[2000] = {};

  printf("%d\n", makeRPM(buff, len, RPMbuff));
  printf(RPMbuff);
  printf("\n");
  return 0;
}

int push(struct stack* stk, double val)
{
  if(!stk) return -1;

  struct point* newPoint = (struct point*)malloc(sizeof(struct point));
  if(!newPoint) return 2;

  newPoint->val = val;
  newPoint->next = stk->first; // if new stack then NULL
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

  stk->first = stk->first->next; // if one elem then NULL
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

int parseFile(char* outStr, int* outTab, size_t* outLen, char* fileName)
{
  if(!outStr || !outTab || !outLen || !fileName) return -1;

  FILE* fPtr = fopen(fileName, "rb");
  if(!fPtr) return 3;

  char el = 0;
  size_t i = 0;
  while(fscanf(fPtr, "%c", &el) != -1 && el != '\n' && i < 2000)
  {
    if(el != ' ')
    {
      outStr[i] = el;
      i++;
    }

  }
  if(i >= 2000)
  {
    fclose(fPtr);
    return 4;
  }
  *outLen = i;
  outStr[i] = '\0';

  int val = 0;
  while(fscanf(fPtr, "%c=%d", &el, &val) == 2)
  {
    if((el < (char)'A' || el > (char)'Z') && (el < (char)'a' || el > (char)'z'))
    {
      fclose(fPtr);
      return 4;
    }

    outTab[(int)el] = val;
  }

  fclose(fPtr);
  return 0;
}

int makeRPM(char* inStr, size_t len, char* outStr)
{
  if(!inStr || !outStr) return -1;

  char Tab[256] = {};
  for(int i = (int)'A'; i <= (int)'Z'; i++)
  {
    Tab[i] = -1;
  }
  for(int i = (int)'a'; i <= (int)'z'; i++)
  {
    Tab[i] = -1;
  }
  for(int i = (int)'0'; i <= (int)'9'; i++)
  {
    Tab[i] = -1;
  }

  char operands[8] = "-+*/()=";
  char prio[7] = {4,4,5,5,1,2,3};
  for(size_t i = 0; i < 7; i++)
  {
    Tab[(int)operands[i]] = prio[i];
  }


  struct stack stk;
  stk.first = NULL;
  stk.size = 0;

  // TEST !!!
  // struct stack brctsStack;
  // stk.first = NULL;
  // stk.size = 0;
  // TEST !!!

  char prevPrio = 4;

  size_t j = 0;
  double top = 0;
  for(size_t i = 0; i < len; i++)
  {
    if(Tab[(int)inStr[i]] == -1)
    {
      outStr[j] = inStr[i]; // if operand
      j++;
      prevPrio = Tab[(int)inStr[i]]; // prev is operand
    }
    else if(Tab[(int)inStr[i]] == 4 && prevPrio != -1 && prevPrio != 2) // Unar + and -
    {
      if(inStr[i] == '-')
      {
        outStr[j] = '0';
        j++;

        if(push(&stk, (double)'-'))
        {
          destroyStack(&stk);
          return 2;
        }
      }
      else
      {
        // skip '+'
      }
    }
    else if(Tab[(int)inStr[i]] == 1 || (!(stk.first) && Tab[(int)inStr[i]]))
    {
      if(Tab[(int)inStr[i]] == 2) // if begin with ')' - ERROR
      {
        destroyStack(&stk);
        return 2;
      }
      if(push(&stk, (double)inStr[i]))
      {
        destroyStack(&stk);
        return 2;
      }

      prevPrio = Tab[(int)inStr[i]]; // prev is '(' or operation in begin
    }
    else if(!readHead(&stk, &top) && (Tab[(int)inStr[i]] > Tab[(int)top]))
    {
      if(Tab[(int)inStr[i]] == 2) // bigger  only if ')' after '(' - ERROR
      {
        destroyStack(&stk);
        return 2;
      }
      if(push(&stk, (double)inStr[i]))
      {
        destroyStack(&stk);
        return 2;
      }

      prevPrio = Tab[(int)inStr[i]]; // operation
    }
    else if(Tab[(int)inStr[i]] > 0)
    {
      while(stk.size > 0 && !readHead(&stk, &top) && Tab[(int)inStr[i]] <= Tab[(int)top])
      {
        if(pop(&stk, &top))
        {
          destroyStack(&stk);
          return 2;
        }
        outStr[j] = (char)top;
        j++;
      }
      if(Tab[(int)inStr[i]] == 2)
      {
        if(Tab[(int)top] == 1)
        {
          if(pop(&stk, &top)) // pop '(' without writing
          {
            destroyStack(&stk);
            return 2;
          }
        }
        else // if ')' without '(' - ERROR
        {
          destroyStack(&stk);
          return 2;
        }
      }
      else
      {
        if(push(&stk, (double)inStr[i]))
        {
          destroyStack(&stk);
          return 2;
        }
      }

      prevPrio = Tab[(int)inStr[i]];
    }
    else if(inStr[i] == ' ')
    {
      // just skip
    }
    else // incorrect symbol - ERROR
    {
      destroyStack(&stk);
      return 2;
    }
  }
  while(stk.size > 0)
  {
    if(pop(&stk, &top))
    {
      destroyStack(&stk);
      return 2;
    }
    if(Tab[(int)top] != 1) // All operations writing, without '(' - just skip
    {
      outStr[j] = (char)top;
      j++;
    }
  }
  outStr[j] = '\0';

  return 0;
}
