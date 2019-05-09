#include <stdlib.h>
#include <stdio.h>
#include "extmem-c\extmem.c"
#include <time.h>
#define Raddress 0
#define Saddress 100
#define Block_size 1
#include <math.h>
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
    /*
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    } */
    srand((unsigned)time( NULL ) );
    for(n=0;n<16;n++)
    {
        if((blk = getNewBlockInBuffer(buf))==NULL)
        {
            printf("failed to get Block!\n");
        }
        for(i=0;i<7;i++)
        {
            (blk+i)->A=rand()%40+1;
            (blk+i)->B=rand()%1000+1;
        }
        int *next = (blk+7);
        *next = Raddress+Block_size*(n+1);
        if (writeBlockToDisk(blk, Raddress+Block_size*n, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        /*
        if ((blk = readBlockFromDisk(*next-Block_size, &buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("\n");
        for (i = 0; i < 7; i++)
        {
            printf("%d,%d\n", (blk+i)->A,(blk+i)->B);
            freeBlockInBuffer(blk,&buf);
        }
        */
    }
}

TypeS *getRandomS(Buffer *buf)
{

    int i=0;
    int n=0;
    TypeS *blk; /* A pointer to a block */
    TypeS *head;
    /*
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    } */
    srand((unsigned)time( NULL ) );
    for(n=0;n<32;n++)
    {
        if((blk = getNewBlockInBuffer(buf))==NULL)
        {
            printf("failed to get Block!\n");
        }
        for(i=0;i<7;i++)
        {
            (blk+i)->C=rand()%41+20;
            (blk+i)->D=rand()%1000+1;
        }
        int *next = (blk+7);
        *next = Raddress+Block_size*(n+1);

        if (writeBlockToDisk(blk, Saddress+Block_size*n, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        /*
        if ((blk = readBlockFromDisk(*next-Block_size, &buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("\n");
        for (i = 0; i < 7; i++)
        {
            printf("%d,%d,%d\n", n,(blk+i)->C,(blk+i)->D);
            freeBlockInBuffer(blk,&buf);
        }
        */
    }
}
void selectByOrder(Buffer *buf)
{
    int Rresult = 200;
    int Rindex = 0;
    int Sresult = 250;
    int Sindex = 0;
    TypeR *tempR;
    TypeR *blkR;
    TypeS *blkS;
    TypeS *tempS;
    int index = 0;
    int n=0;
    /*
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    } */
    if((tempR = getNewBlockInBuffer(buf))==NULL)
    {
        printf("failed to get Block!\n");
    }
    for(n=0;n<16;n++)
    {
        if ((blkR = readBlockFromDisk(Raddress+n*Block_size, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        int i=0;
        for (i = 0; i < 7; i++)
        {
            if((blkR+i)->A==40)
            {
                printf("Find tuple R(A,B): %d,%d\n",(blkR+i)->A,(blkR+i)->B);
                (tempR+index)->A = (blkR+i)->A;
                (tempR+index)->B = (blkR+i)->B;
                index ++;
                if(index>=7)
                {
                    if (writeBlockToDisk(tempR, Rresult+Block_size*Rindex, &buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    int *next = (blkR+7);
                    *next = Rresult+Block_size*(Rindex+1);
                    if((tempR = getNewBlockInBuffer(&buf))==NULL)
                    {
                        printf("failed to get Block!\n");
                    }
                    Rindex ++;
                    index = 0;
                }
            }
        }

        freeBlockInBuffer(blkR,buf);
    }
    int i=0;
    for(i=index;i<7;i++)
    {
        (tempR+index)->A = 0;
        (tempR+index)->B = 0;
    }
    if (writeBlockToDisk(tempR, Rresult+Block_size*Rindex, buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    if((tempS = getNewBlockInBuffer(buf))==NULL)
    {
        printf("failed to get Block!\n");
    }
    index = 0;
    for(n=0;n<32;n++)
    {
        if ((blkS = readBlockFromDisk(Saddress+n*Block_size, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        for (i = 0; i < 7; i++)
        {
            if((blkS+i)->C==60)
            {
                printf("Find tuple S(C,D): %d,%d\n",(blkS+i)->C,(blkS+i)->D);
                (tempS+index)->C = (blkS+i)->C;
                (tempS+index)->D = (blkS+i)->D;
                index ++;
                if(index>=7)
                {
                    if (writeBlockToDisk(tempS, Sresult+Block_size*Sindex, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    int *next = (blkS+7);
                    *next = Sresult+Block_size*(Sindex+1);
                    if((tempS = getNewBlockInBuffer(buf))==NULL)
                    {
                        printf("failed to get Block!\n");
                    }
                    Sindex ++;
                    index = 0;
                }
            }
        }
        freeBlockInBuffer(blkS,buf);
    }

    for(i=index;i<7;i++)
    {
        (tempS+index)->C = 0;
        (tempS+index)->D = 0;
    }
    if (writeBlockToDisk(tempS, Sresult+Block_size*Sindex, buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
}
void sortS(TypeS *blk)
{
    int i=0;
    int j=0;
    TypeS temp;
    for(i=0;i<7;i++)
    {
        for(j=i;j<7;j++)
        {
            if((blk+i)->C>(blk+j)->C)
            {
                temp.C = (blk+i)->C;
                temp.D = (blk+i)->D;
                (blk+i)->C = (blk+j)->C;
                (blk+i)->D = (blk+j)->D;
                (blk+j)->C = temp.C;
                (blk+j)->D = temp.D;
            }
        }
    }
    return;
}
void sortR(TypeR *blk)
{
    int i=0;
    int j=0;
    TypeR temp;
    for(i=0;i<7;i++)
    {
        for(j=i;j<7;j++)
        {
            if((blk+i)->A>(blk+j)->A)
            {
                temp.A = (blk+i)->A;
                temp.B = (blk+i)->B;
                (blk+i)->A = (blk+j)->A;
                (blk+i)->B = (blk+j)->B;
                (blk+j)->A = temp.A;
                (blk+j)->B = temp.B;
            }
        }
    }
    return;
}
void MyMergeSortR(Buffer *buf)
{
    int m=0;
    int l=0;
    int k=0;
    int i=0;
    int j=0;
    int n=0;
    int Rresult =300;
    TypeR *blk1;
    TypeR *blk2;
    TypeR *temp1;
    TypeR *temp2;
    TypeR *tempBlock;
    int index = 0;
    tempBlock = getNewBlockInBuffer(buf);

    for(m=1;m<=1;m++)
    {

        for(n=0;n<16;n=n+(int)pow(2.0,m))
        {
                printf("%d\n\n",n);
                int round = (int)(pow(2.0,m)/2);

                blk1 = readBlockFromDisk(Raddress+n*Block_size,buf);
                blk2 = readBlockFromDisk(Raddress+(n+round)*Block_size,buf);
                i=0;
                j=0;
                temp1 = blk1 ;
                temp2 = blk2 ;
                int index1 = 0;
                int index2 = 0;
                while(index1<round&&index2<round)
                {

                    if(temp1->A > temp2->A)
                    {
                        (tempBlock+index)->A = temp2->A;
                        (tempBlock+index)->B = temp2->B;
                        j++;
                        index ++;
                        temp2 = blk2+j;
                    }
                    else
                    {
                        (tempBlock+index)->A = temp1->A;
                        (tempBlock+index)->B = temp1->B;
                        i++;
                        index++;
                        temp1 = blk1+i;
                    }

                    if(j>6)
                    {
                        freeBlockInBuffer(blk2,buf);
                        index2 ++;
                        if(index2<round)
                        {
                            blk2 = readBlockFromDisk(Raddress+(n+round+index2)*Block_size,buf);
                            j = 0;
                            temp2 = blk2;
                        }
                        else break;
                    }

                    if(i>6)
                    {
                        freeBlockInBuffer(blk1,buf);
                        index1 ++;
                        if(index1<round)
                        {
                            blk1 = readBlockFromDisk(Raddress+(n+index1)*Block_size,buf);
                            i = 0;
                            temp1 = blk1;
                        }
                        else break;
                    }
                    if(index>6)
                    {
                        for(k=0;k<7;k++)
                        {
                            printf("%d\n",(tempBlock+k)->A);

                        }
                        printf("\n");
                        writeBlockToDisk(tempBlock,Rresult,buf);
                        tempBlock = getNewBlockInBuffer(buf);
                        index = 0;

                    }
                }
                for(index2=index2;index2<round;index2++)
                {
                    for(j=j;j<7;j++)
                    {
                        (tempBlock+index)->A = (blk2+j)->A;
                        (tempBlock+index)->B = (blk2+j)->B;
                        index ++;
                        if(index>6)
                        {
                            for(k=0;k<7;k++)
                            {
                                printf("%d\n",(tempBlock+k)->A);

                            }
                            printf("\n");
                            writeBlockToDisk(tempBlock,Rresult,buf);
                            tempBlock = getNewBlockInBuffer(buf);
                            index =0;
                        }
                    }
                    j = 0;
                    if(index2+1<round)
                    {
                        freeBlockInBuffer(blk2,buf);
                        blk2 =readBlockFromDisk(Raddress+n+round+index2+1,buf);
                    }
                    else break;
                }
                for(index1=index1;index1<round;index1++)
                {
                    for(i=i;i<7;i++)
                    {
                        (tempBlock+index)->A = (blk1+i)->A;
                        (tempBlock+index)->B = (blk1+i)->B;
                        index ++;
                        if(index>6)
                        {
                            for(k=0;k<7;k++)
                            {
                                printf("%d\n",(tempBlock+k)->A);

                            }
                            printf("\n");
                            writeBlockToDisk(tempBlock,Rresult,buf);
                            tempBlock = getNewBlockInBuffer(buf);
                            index =0;
                        }
                    }
                    i = 0;
                    if(index1+1<round)
                    {
                        freeBlockInBuffer(blk1,buf);
                        blk2 =readBlockFromDisk(Raddress+n+index1+1,buf);
                    }
                    else break;
                }

                freeBlockInBuffer(temp1,buf);
                freeBlockInBuffer(temp2,buf);
            }

    }
}
void MergeSortR(Buffer *buf)
{
    int m=0;
    int l=0;
    int k=0;
    int i=0;
    int j=0;
    int n=0;
    int Rresult =300;
    TypeR *blk1;
    TypeR *blk2;
    TypeR *temp1;
    TypeR *temp2;
    TypeR *tempBlock;
    TypeS *blk3;
    TypeS *blk4;
    int index = 0;
    tempBlock = getNewBlockInBuffer(buf);
    int arr1[400];
    int arr2[400];
    int t =0;
    int temp;
    for(i=0;i<16;i++)
    {
        blk1 = readBlockFromDisk(Raddress+i*Block_size,buf);
        for(j=0;j<7;j++)
        {

            arr1[t] = (blk1+j)->A;
            arr2[t] = (blk1+j)->B;
            t++;
        }
        freeBlockInBuffer(blk1,buf);
    }
    for(i=0;i<16*7;i++)
    {
        for(j=i;j<16*7;j++)
        {
            if(arr1[i]>arr1[j])
            {
                temp = arr1[i];
                arr1[i] = arr1[j];
                arr1[j]=temp;
                temp = arr2[i];
                arr2[i] = arr2[j];
                arr2[j]=temp;
            }
        }
    }
    t=0;
    for(i=0;i<16;i++)
    {
        blk1 = getNewBlockInBuffer(buf);
        for(j=0;j<7;j++)
        {

            (blk1+j)->A=arr1[t];
            (blk1+j)->B=arr2[t];
            printf("%d,%d\n",(blk1+j)->A,(blk1+j)->B);
            t++;
        }
        writeBlockToDisk(blk1,Raddress+i*Block_size,buf);
        freeBlockInBuffer(blk1,buf);
    }
    for(i=0;i<32;i++)
    {
        blk3 = readBlockFromDisk(Saddress+i*Block_size,buf);
        for(j=0;j<7;j++)
        {

            arr1[t] = (blk3+j)->C;
            arr2[t] = (blk3+j)->D;
            t++;
        }
        freeBlockInBuffer(blk3,buf);
    }
    for(i=0;i<32*7;i++)
    {
        for(j=i;j<32*7;j++)
        {
            if(arr1[i]>arr1[j])
            {
                temp = arr1[i];
                arr1[i] = arr1[j];
                arr1[j]=temp;
                temp = arr2[i];
                arr2[i] = arr2[j];
                arr2[j]=temp;
            }
        }
    }
    t=0;
    for(i=0;i<32;i++)
    {
        blk3 = getNewBlockInBuffer(buf);
        for(j=0;j<7;j++)
        {

            (blk3+j)->C=arr1[t];
            (blk3+j)->D=arr2[t];
            printf("%d,%d\n",(blk3+j)->C,(blk3+j)->D);
            t++;
        }
        writeBlockToDisk(blk3,Saddress+i*Block_size,buf);
        freeBlockInBuffer(blk3,buf);
    }
    /*    for(n=0;n<16;n=n+2)
        {
                printf("%d\n\n",n);
                blk1 = readBlockFromDisk(Raddress+n*Block_size,buf);
                blk2 = readBlockFromDisk(Raddress+(n+1)*Block_size,buf);
                i=0;
                j=0;
                temp1 = blk1 ;
                temp2 = blk2 ;
                int index1 = 0;
                int index2 = 0;
                while(i<7&&j<7)
                {

                    if(temp1->A > temp2->A)
                    {
                        (tempBlock+index)->A = temp2->A;
                        (tempBlock+index)->B = temp2->B;
                        j++;
                        index ++;
                        temp2 = blk2+j;
                    }
                    else
                    {
                        (tempBlock+index)->A = temp1->A;
                        (tempBlock+index)->B = temp1->B;
                        i++;
                        index++;
                        temp1 = blk1+i;
                    }
                    if(index>6)
                    {
                        for(k=0;k<7;k++)
                        {
                            printf("%d\n",(tempBlock+k)->A);

                        }
                        printf("\n");
                        writeBlockToDisk(tempBlock,Rresult,buf);
                        freeBlockInBuffer(tempBlock,buf);
                        tempBlock = getNewBlockInBuffer(buf);
                        index = 0;

                    }
                }
                for(j=j;j<7;j++)
                {
                        (tempBlock+index)->A = (blk2+j)->A;
                        (tempBlock+index)->B = (blk2+j)->B;
                        index ++;
                        if(index>6)
                        {
                            for(k=0;k<7;k++)
                            {
                                printf("%d\n",(tempBlock+k)->A);

                            }
                            printf("\n");
                            writeBlockToDisk(tempBlock,Rresult,buf);
                            freeBlockInBuffer(tempBlock,buf);
                            tempBlock = getNewBlockInBuffer(buf);
                            index =0;
                        }
                    }

                for(i=i;i<7;i++)
                {
                    (tempBlock+index)->A = (blk1+i)->A;
                    (tempBlock+index)->B = (blk1+i)->B;
                    index++;
                    if(index>6)
                    {
                        for(k=0;k<7;k++)
                        {
                            printf("%d\n",(tempBlock+k)->A);

                        }
                        printf("\n");
                        writeBlockToDisk(tempBlock,Rresult,buf);
                        freeBlockInBuffer(tempBlock,buf);
                        tempBlock = getNewBlockInBuffer(buf);
                        index = 0;
                    }
                }
                freeBlockInBuffer(temp1,buf);
                freeBlockInBuffer(temp2,buf);
        }
         */
}
void SelectByBinary(Buffer *buf)
{
    int i=0;
    int j;
    TypeR *blkR;
    TypeS *blkS;
    for(i=0;i<16;i++)
    {
        blkR = readBlockFromDisk(Raddress+i*Block_size,buf);
        sortR(blkR);
        writeBlockToDisk(blkR,Raddress+i*Block_size,buf);
        freeBlockInBuffer(blkR,buf);
    }

    for(i=0;i<32;i++)
    {
        blkS = readBlockFromDisk(Saddress+i*Block_size,buf);
        sortS(blkS);
        writeBlockToDisk(blkS,Saddress+i*Block_size,buf);
        freeBlockInBuffer(blkS,buf);
    }
    MergeSortR(buf);
}
int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    TypeR R;
    TypeS S;
    TypeR *blk; /* A pointer to a block */
    int i = 0;
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    getRandomS(&buf);
    getRandomR(&buf);
    SelectByBinary(&buf);

    /*
    selectByOrder(&buf); */
    /*
    blk = readBlockFromDisk(Saddress,&buf);
    for(i=0;i<7;i++)
    {
        printf("%d\n",(blk+i)->A);
    }
    printf("\n");
    sortR(blk);
    for(i=0;i<7;i++)
    {
        printf("%d\n",(blk+i)->A);
    }  */
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

