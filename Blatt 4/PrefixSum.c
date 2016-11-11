#include<math.h>
#include<stdio.h>

#define N 8
#define TREE_HEIGHT 4



typedef union treeNode_u
{
  double sum;
  double fromLeft;
}treeNode;


double A[N], B[N];
treeNode sumTree[TREE_HEIGHT][N];

void testPrint()
{
  int i, j;
  for(i = 0; i < TREE_HEIGHT; i++)
  {
    for(j = 0; j < N; j++)
    {
      printf("%f|",sumTree[i][j].fromLeft);
    }
    printf("\n");
  }
  printf("\n");
}

void init3()
{
  int i, j;
  for(i = 0; i < TREE_HEIGHT-1; i++)
  {
    for(j = 0; j < N; j++)
    {
      sumTree[i][j].sum = 0;
    }
  }

  for(i = 0; i < N; i++)
  {
    A[i] = 0;
    B[i] = i;
    sumTree[TREE_HEIGHT-1][i].sum = i;
  }
}

void bottomUp()
{
  int i, layer;

  for(layer = TREE_HEIGHT-1; layer > 0; layer--)
  {
    for(i = 0; i < pow(2,layer); i += 2)
    {
      sumTree[layer-1][i/2].sum = sumTree[layer][i].sum + sumTree[layer][i+1].sum;
    }
  }
  testPrint();
}

void topDown()
{
  int layer, i;

  sumTree[0][0].fromLeft = 0;

  for(layer = 0; layer < TREE_HEIGHT-1; layer++)
  {
    for(i = 0; i < pow(2,layer); i++)
    {
      sumTree[layer+1][2*i+1].fromLeft = sumTree[layer+1][2*i].sum + sumTree[layer][i].fromLeft;
      sumTree[layer+1][2*i].fromLeft = sumTree[layer][i].fromLeft;
    }
  }
}

void A3()
{
  init3();
  bottomUp();
  topDown();
  testPrint();
}

int main()
{
  A3();
}
