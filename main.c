/*
 * test.c
 * Zhaonian Zou
 * Harbin Institute of Technology
 * Jun 22, 2011
 */

#include <stdlib.h>
#include <stdio.h>
#include "extmem-c\extmem.c"
#define Address 31415926
#define Block_size 64
typedef struct {
    int A;
    int B;
} TypeR;
typedef struct {
    int C;
    int D;
} TypeS;
TypeR *getRandomR(Buffer *buf)
{
    int i=0;
    int n=0;
    TypeR *blk; /* A pointer to a block */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    for(n=0;n<16;n++)
    {
        if((blk = getNewBlockInBuffer(&buf))==NULL)
        {
            printf("failed to get Block!\n");
        }
        printf("%p\n",blk);
        for(i=0;i<7;i++)
        {
            (blk+i)->A=i+1;
            (blk+i)->B=i+1;
        }
    int *next = (blk+7);
    *next = Address+Block_size*(n+1);
    printf("%p\n",*next);
    if (writeBlockToDisk(blk, Address+Block_size*n, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    /*
    if ((blk = readBlockFromDisk(Address+Block_size*n, &buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }

        for (i = 0; i < 7; i++)
        {
            printf("%d,%d\n", (blk+i)->A,(blk+i)->A);
            freeBlockInBuffer(blk,&buf);
        }
        */

    }

}
int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    TypeR R;
    TypeS S;
    TypeR *blk; /* A pointer to a block */
    int i = 0;
    getRandomR(&buf);

    /* Initialize the buffer */

    /*
    if (!initBuffer(20, 8, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    } */

    /* Get a new block in the buffer */
    /* blk = getNewBlockInBuffer(&buf); */

    /* Fill data into the block */
    /*for (i = 0; i < 8; i++)
        *(blk + i) = 'a' + i; */

    /* Write the block to the hard disk */
    /*if (writeBlockToDisk(blk, 31415926, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    */
    /* Read the block from the hard disk */
    /*
    if ((blk = readBlockFromDisk(31415926, &buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }
    */
    /* Process the data in the block */
    /*
    for (i = 0; i < 8; i++)
        printf("%c", *(blk + i));

    printf("\n");
    printf("# of IO's is %d\n", buf.numIO); /* Check the number of IO's */

    return 0;
}

