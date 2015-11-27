#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
typedef struct prog
{
    int lno,address;
    char label[20],com[20],op[20];
    struct prog *next;

}prog;
typedef struct optab
{
    char mnem[20];
    int code;
}optab;
optab o[30];
prog l[150];

int optabcall()
{
    FILE *f;
    int i=0;
    f=fopen("optable.txt","r");
    if(f!=NULL)
    {
        while(!feof(f))
        {
            fscanf(f,"%s ",o[i].mnem);
            fscanf(f,"%d\n",&o[i].code);
            i++;
        }

    }
    else
    {
        printf("\n No Optab");
        getch();
        exit(1);
    }
    return i;
    fclose(f);
}
int loadcode(char progname[],int *locctr)
{
    FILE *f;
    f=fopen(progname,"r");
    int i=0,li=1;
    if(f!=NULL)
    {
        while(!feof(f))
        {
            l[i].lno=li;
            fscanf(f,"%s ",l[i].label);
            fscanf(f,"%s ",l[i].com);
            if(strcmp(l[i].com,"START")!=0)
            {
                printf("\n Should Start with START");
            }
            else
                fscanf(f,"%X\n",locctr);
            //printf("\n%d %s %s %s",l[i].lno,l[i].label,l[i].com,l[i].op);
            li++;
            i++;
            while(!feof(f))
            {
                l[i].lno=li;
                fscanf(f,"%s ",l[i].label);
                fscanf(f,"%s ",l[i].com);
                if(strcmp(l[i].com,"RSUB")!=0)
                    fscanf(f,"%s\n",l[i].op);
                else
                    strcpy(l[i].op,"");
                //printf("\n%d %s %s %s",l[i].lno,l[i].label,l[i].com,l[i].op);
                i++;
                li++;
            }
        }
    }
    else
    {
        printf("\n No file Exist");
        getch();
        exit(1);
        return 0;
    }
    return li;

}

int Passone(int locctr,int li,int ol)
{
    int i=0,start=locctr,ln,n,j=0,f=0,s=0,add;
    char c[25];
    FILE *fl;
    fl=fopen("symtab.txt","w");
    fclose(fl);
    printf("%d\n\n",li);
    for(i=0;i<li-1;i++)
    {
        l[i].address=locctr;
        if(strcmp(l[i].com,"START")==0)
             locctr+=0;
        else if(strcmp(l[i].com,"WORD")==0)
            locctr+=3;
        else if(strcmp(l[i].com,"BYTE")==0)
        {
            ln=strlen(l[i].op);
            locctr+=ln;
        }
        else if(strcmp(l[i].com,"RESB")==0)
        {
            n=atoi(l[i].op);
            locctr+=n;
        }
        else if(strcmp(l[i].com,"RESW")==0)
        {
            n=atoi(l[i].op);
            locctr+=n*3;
        }
        else
            locctr+=3;
        printf("%d %s %s %s\n",l[i].lno,l[i].label,l[i].com,l[i].op);

    }

    for(i=0;i<li-1;i++)
    {
        //
        if(strcmp(l[i].com,"START")!=0 && strcmp(l[i].com,"END")!=0 && strcmp(l[i].com,"RESW")!=0 && strcmp(l[i].com,"RESB")!=0 && strcmp(l[i].com,"BYTE")!=0 && strcmp(l[i].com,"RSUB")!=0 && strcmp(l[i].com,"WORD")!=0)
        {
            for(j=0;j<ol;j++)
            {
                if(strcmp(l[i].com,o[j].mnem)==0 )
                {
                    f=0;
                    break;
                }
                else
                {
                    f=1;
                }
            }
            if(f==1)
            {
                printf("\nINVALID MNEMONIC %s in line %d",l[i].com,l[i].lno);
                getch();
                exit(1);
            }
        }
        fl=fopen("symtab.txt","r");
        while(!feof(fl))
        {
            fscanf(fl,"%s ",c);
            fscanf(fl,"%d\n",&add);
            if(strcmp(l[i].label,c)==0)
            {
                s=1;
                break;
            }
        }
        fclose(fl);
        if(s==0 && strcmp(l[i].label,".")!=0)
        {
            fl=fopen("symtab.txt","a");
            printf("\n%s ",l[i].label);
            fprintf(fl,"%s ",l[i].label);
            fprintf(fl,"%X\n",l[i].address);
            fclose(fl);
        }
        else if(s==1)
        {
            printf("\nDuplicate symbols %s in line",l[i].label,l[i].lno);
            getch();
            exit(1);
        }
        s=0;
    }
    interfile(li);
}

