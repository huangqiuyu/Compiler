#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//symbol 类型定义
#define CONSTSYM	1
#define INTSYM	2
#define CHARSYM	3
#define VOIDSYM	4
#define MAINSYM	5
#define IFSYM	6
#define ELSESYM	7
#define DOSYM	8
#define WHILESYM	9
#define FORSYM	10
#define SCANFSYM	11
#define PRINTFSYM	12
#define RETURNSYM	13
#define PLUS	14
#define MINUS	15
#define TIMES	16
#define DIVI	17
#define LSS	18
#define LEQ	19
#define GRT	20
#define GEQ	21
#define NEQ	22
#define EQL	23
#define ASSIGN	24
#define LPAREN	25
#define RPAREN	26
#define LBRACE	27
#define RBRACE	28
#define LBKET	29
#define RBKET	30
#define COMMA	31
#define SEMICOLON	32
#define LSINQ	33
#define RSINQ	34
#define LDOUQ	35
#define RDOUQ	36
#define IDEN	37
#define NUMBER	38
#define CHARCST	39
#define STRING	40

#define ERRORSYM 111
#define FALSE -1
#define TRUE 0

#define MAXPATH 60  //the string of file path's max size 输入文件的路径字符串的最大值
#define MAXLS   1000 //max one line size of input file 输入文件中一行的字符数的最大值
#define MAXIDS  20  //max size of identifier 一个标识符长度的最大值
#define MAXSTR  100 //max size of output string 输出字符串的最大长度
#define MAXTAB  10000   //max size of table 符号表的最大长度

char* file; //= (char*)malloc(sizeof(char)*MAXPATH);//file of input

char linebuf[MAXLS];  //line buffer to storage one line of the input file 保存读入文件的一行，行缓冲区
int lineIndex=MAXLS-1;  //index of the linebuffer 行缓冲区指针

//char ch;    //last charactor read 最近一次读入的字符
int symbol=0;     //last symbol read 最近一次识别出来的单词类型
char id[MAXIDS];    //last identifier read 最近一次识别出来的标识符名称

int num=0;    //last const number read 最近一次读入的整常数的值
char chconst;       //last const character read 最近一次读入的字符常量的值
char str[MAXSTR];   //last output string read 最近一次读入的字符串

int isFileEnd;

//table 符号表
struct{
    char name[20];
    int obj;   //constant variable function
    int type;   //int char arrays



}table[MAXT];

void error(int i)
{
    printf("error!!!!\n");
}

//read one character 读入一个字符，不直接用getchar因为不能回退
char getch()
{

    if((lineIndex>=MAXLS-1)||linebuf[lineIndex]=='\n'||linebuf[lineIndex]=='\0')
    {
        if((gets(linebuf))==NULL)
        {
            isFileEnd = TRUE;
            printf("read the end of file\n");
            return EOF;////////////
        }
        else
        {
            if(linebuf[0]=='\0')
            {
                linebuf[0]='\n';
                linebuf[1]='\0';

            }

            lineIndex=0;
            return linebuf[lineIndex++];
        }
    }

    else
    {
        return linebuf[lineIndex++];
    }
}

//go back to read previous character 回退读入的字符
void goback()
{
    if(lineIndex>0)
        lineIndex--;
}



//read one token and identify the symbol 词法分析过程
void getsym()
{
    int i=0,j=0;
    char c;
    c=getch();
    while(c==' '||c=='\n'||c=='\t')
    {
        c=getch();
        if(isFileEnd==TRUE)
        {
            printf("read the end of file\n");
        }
    }

    if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
    {
        i = 0;
        while((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c=='_')||(c>='0'&&c<='9'))
        {
            //out of range
            if(i>=MAXIDS-1)
            {
                error();
                break;
            }
            id[i++]=c;
            c=getch();
        }
        id[i]='\0';
        goback();

            if(id[0]=='c'&&id[1]=='o'&&id[2]=='n'&&id[3]=='s'&&id[4]=='t'&&id[5]=='\0')
            {
                symbol = CONSTSYM;
            }
            else if(id[0]=='i'&&id[1]=='n'&&id[2]=='t'&&id[3]=='\0')
            {
                symbol = INTSYM;
            }
            else if(id[0]=='c'&&id[1]=='h'&&id[2]=='a'&&id[3]=='r'&&id[4]=='\0')
            {
                symbol = CHARSYM;
            }
            else if(id[0]=='v'&&id[1]=='o'&&id[2]=='i'&&id[3]=='d'&&id[4]=='\0')
            {
                symbol = VOIDSYM;
            }
            else if(id[0]=='m'&&id[1]=='a'&&id[2]=='i'&&id[3]=='n'&&id[4]=='\0')
            {
                symbol = MAINSYM;
            }
            else if(id[0]=='i'&&id[1]=='f'&&id[2]=='\0')
            {
                symbol = IFSYM;
            }
            else if(id[0]=='e'&&id[1]=='l'&&id[2]=='s'&&id[3]=='e'&&id[4]=='\0')
            {
                symbol = ELSESYM;
            }
            else if(id[0]=='d'&&id[1]=='o'&&id[2]=='\0')
            {
                symbol = DOSYM;
            }
            else if(id[0]=='w'&&id[1]=='h'&&id[2]=='i'&&id[3]=='l'&&id[4]=='e'&&id[5]=='\0')
            {
                symbol = WHILESYM;
            }
            else if(id[0]=='f'&&id[1]=='o'&&id[2]=='r'&&id[3]=='\0')
            {
                symbol = FORSYM;
            }
            else if(id[0]=='s'&&id[1]=='c'&&id[2]=='a'&&id[3]=='n'&&id[4]=='f'&&id[5]=='\0')
            {
                symbol = SCANFSYM;
            }
            else if(id[0]=='p'&&id[1]=='r'&&id[2]=='i'&&id[3]=='n'&&id[4]=='t'&&id[5]=='f'&&id[6]=='\0')
            {
                symbol = PRINTFSYM;
            }
            else if(id[0]=='r'&&id[1]=='e'&&id[2]=='t'&&id[3]=='u'&&id[4]=='r'&&id[5]=='n'&&id[6]=='\0')
            {
                symbol = RETURNSYM;
            }
            else
            {
                symbol = IDEN;
            }


    }

    else if(c>='0'&&c<='9')
    {
        num = 0;
        if(c=='0')
        {
            c = getch();
            if(c>='0'&&c<='9')
            {
                error();
            }
            while(c>='0'&&c<='9')
            {
                num = num*10+(c-'0');
                c = getch();
            }
        }
        else
        {
            while(c>='0'&&c<='9')
            {
                num = num*10+(c-'0');
                c = getch();
            }
        }
        goback();
        symbol = NUMBER;

    }

    else
    {
        switch(c)
        {
            case '+':
                symbol = PLUS;
                break;
            case '-':
                symbol = MINUS;
                break;

            case '*':
                symbol = TIMES;
                break;

            case '/':
                symbol = DIVI;
                break;

            case '<':
                c = getch();
                if(c=='=')
                    symbol = LEQ;
                else
                {
                    symbol = LSS;
                    goback();
                }
                break;

            case '>':
                c = getch();
                if(c=='=')
                    symbol = GEQ;
                else
                {
                    symbol = GRT;
                    goback();
                }
                break;

            case '!':
                c = getch();
                if(c=='=')
                    symbol = NEQ;
                else
                {
                    symbol = ERRORSYM;
                    goback();
                }
                break;

            case '=':
                c = getch();
                if(c=='=')
                    symbol = EQL;
                else
                {
                    symbol = ASSIGN;
                    goback();
                }
                break;
            case '(':
                symbol = LPAREN;
                break;

            case ')':
                symbol = RPAREN;
                break;
            case '{':
                symbol = LBRACE;
                break;
            case '}':
                symbol = RBRACE;
                break;
            case '[':
                symbol = LBKET;
                break;
            case ']':
                symbol = RBKET;
                break;
            case ',':
                symbol = COMMA;
                break;

            case ';':
                symbol = SEMICOLON;
                break;

            case '_':
                i = 0;
                while((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c=='_')||(c>='0'&&c<='9'))
                {
                    //out of range
                    if(i>=MAXIDS-1)
                    {
                        error();
                        break;
                    }
                    id[i++]=c;
                    c=getch();
                }
                id[i]='\0';
                goback();
                symbol = IDEN;
                break;

            case '\'':
                chconst = getch();
                c = getch();
                if(c=='\'')
                {
                    if(chconst=='+'||chconst=='-'||chconst=='*'||chconst=='/'||
                       (chconst>='a'&&chconst<='z')||(chconst>='A'&&chconst<='Z')||
                       (chconst>='0'&&chconst<='9'))
                    {
                         symbol = CHARCST;
                    }
                    else
                    {
                        error();
                        symbol = ERRORSYM;
                    }
                }
                else
                {
                    goback();
                    symbol = ERRORSYM;
                    error();
                }
                break;

            case '\"':
                j = 0;
                c = getch();
                while(c!='\"'&&j<MAXSTR-1)
                {
                    str[j++] = c;
                    c = getch();
                }
                str[j]='\0';

                if(c=='\"')
                {
                    symbol = STRING;
                }
                else
                {
                    symbol = ERRORSYM;
                    goback();
                    error();
                }

                break;

            default:
                symbol = ERRORSYM;
                error();
                break;

        }


    }

}


int openfile()
{
    file = (char*)malloc((sizeof(char))*MAXPATH);
    printf("please input the test file path\n");

    gets(file);
    if(freopen(file,"r",stdin)==NULL)
    {
        printf("file to open the test file\n");
        return 0;
    }
    else
    {
         isFileEnd = FALSE;
         printf("success to open the test file\n");
         return 1;
    }
}


void printgetsym()
{
    printf("类别码\t\t单词值\n");
    getsym();
    while(isFileEnd==FALSE)
    {
        switch(symbol)
        {
            case CONSTSYM:
                printf("CONSTSYM\tconst\n");
                break;
            case INTSYM:
                printf("INTSYM\tint\n");
                break;
            case CHARSYM:
                printf("CHARSYM\tchar\n");
                break;
            case VOIDSYM:
                printf("VOIDSYM\tvoid\n");
                break;
            case MAINSYM:
                printf("MAINSYM\tmain\n");
                break;
            case IFSYM:
                printf("IFSYM\tif\n");
                break;
            case ELSESYM:
                printf("ELSESYM\telse\n");
                break;
            case DOSYM:
                printf("DOSYM\tdo\n");
                break;
            case WHILESYM:
                printf("WHILESYM\twhile\n");
                break;
            case FORSYM:
                printf("FORSYM\tfor\n");
                break;
            case SCANFSYM:
                printf("SCANFSYM\tscanf\n");
                break;
            case PRINTFSYM:
                printf("PRINTFSYM\tprintf\n");
                break;
            case RETURNSYM:
                printf("RETURNSYM\treturn\n");
                break;
            case PLUS:
                printf("PLUS\t+\n");
                break;
            case MINUS:
                printf("MINUS\t-\n");
                break;
            case TIMES:
                printf("TIMES\t*\n");
                break;
            case DIVI:
                printf("DIVI\t/\n");
                break;
            case LSS:
                printf("LSS\t<\n");
                break;
            case LEQ:
                printf("LEQ\t<=\n");
                break;
            case GRT:
                printf("GRT\t>\n");
                break;
            case GEQ:
                printf("GEQ\t>=\n");
                break;
            case NEQ:
                printf("NEQ\t!=\n");
                break;
            case EQL:
                printf("EQL\t==\n");
                break;
            case ASSIGN:
                printf("ASSIGN\t=\n");
                break;
            case LPAREN:
                printf("LPAREN\t(\n");
                break;
            case RPAREN:
                printf("RPAREN\t)\n");
                break;
            case LBRACE:
                printf("LBRACE\t{\n");
                break;
            case RBRACE:
                printf("RBRACE\t}\n");
                break;
            case LBKET:
                printf("LBKET\t[\n");
                break;
            case RBKET:
                printf("RBKET\t]\n");
                break;
            case COMMA:
                printf("COMMA\t,\n");
                break;
            case SEMICOLON:
                printf("SEMICOLON\t;\n");
                break;
            case IDEN:
                printf("IDEN\t%s\n",id);
                break;
            case NUMBER:
                printf("NUMBER\t%d\n",num);
                break;
            case CHARCST:
                printf("CHARCST\t%c\n",chconst);
                break;
            case STRING:
                printf("STRING\t%s\n",str);
                break;
            case ERRORSYM:
                printf("ERRORSYM\terror\n");
                break;

        }

        getsym();
    }



}




void constdec()
{

}

void vardec()
{

}

void funcdecic()
{

}

void funcdecvo()
{

}



void mixsta()
{

}

void statements()
{

}

void statemt()
{

}

void ifsta()
{

}

void loopsta()
{

}


void callsta()
{

}

void assignsta()
{

}

void readsta()
{

}

void writesta()
{

}

void returnsta()
{

}

void expression()
{

}

void term()
{

}

void factor()
{

}

void isint()
{

}

void vartopro()
{
    if(symbol==COMMA)
    {
        getsym();
        if(symbol==IDEN)
        {
            getsym();
            vartopro();
        }
        else
        {
            error(2);
        }

    }

    if(symbol==LBKET)
    {
        getsym();
        if(symbol==NUMBER)
        {
            if(num==0)
            {
                num = 10;
                error(4);
            }
            getsym();

            if(symbol==RBKET)
            {
                getsym();
                if(symbol==COMMA)
                {
                    getsym();
                    if(symbol==IDEN)
                    {
                        getsym();
                        vartopro();
                    }
                    else
                    {
                        error(2);
                    }
                }

            }
            else
            {
                error(5);
            }
        }
        else
        {
            error(3);
        }
    }

    if(symbol==SEMICOLON)
    {
        getsym();
        if(symbol==INTSYM||symbol==CHARSYM)
        {
            getsym();
            if(symbol==IDEN)
            {
                getsym();
                if(symbol==COMMA||symbol==SEMICOLON||symbol==LBKET)
                {
                    vartopro();
                }

                if(symbol==LPAREN)
                {
                    fictopro();
                }
            }

            else
            {
                error(1);
            }
        }
    }

    else
    {
        error(6);
    }
}

void parameter()
{
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        getsym();
        if(symbol==IDEN)
        {
            getsym();
            if(symbol==COMMA)
            {
                getsym();
                parameter();
            }
        }
        else
        {
            error(1);
        }
    }
    else
    {
        error(8);
    }
}

void fictopro()
{
    if(symbol==LPAREN)
    {
        getsym();
        if(symbol==INTSYM||symbol==CHARSYM)
        {
            parameter();
        }

        if(symbol==RPAREN)
        {
            getsym();
            if(symbol==LBRACE)
            {
                mixsta();
                getsym();
                if(symbol==RBRACE)
                {
                    getsym();
                    if(symbol==INTSYM||CHARSYM)
                    {
                        getsym();
                        if(symbol==IDEN)
                        {
                            getsym();
                            fictopro();
                        }
                        else
                        {
                            error(1);
                        }
                    }
                    else if(symbol==VOIDSYM)
                    {
                        getsym();
                        if(symbol==IDEN)
                        {
                            fvotopro();
                        }
                    }
                    else
                    {
                        error(12);
                    }
                }
                else
                {
                    error(11);
                }
            }
            else
            {
                error(10);
            }
        }
        else
        {
            error(9);
        }
    }

    else
    {
        error(7);
    }
}

void fvotopro()
{
    if(symbol==IDEN)
    {
        getsym();
        fictopro();
    }
    else
    {
        error(13);
    }
}


void ismain()
{
    if(symbol==MAINSYM)
    {

    }
    else
    {
        error(14);
    }
}

//注意声明的顺序
void program()
{
    getsym();
    if(symbol==CONSTSYM)
    {
        constdec();
    }
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        getsym();
        if(symbol==IDEN)
        {
            getsym();
            if(symbol==COMMA||symbol==SEMICOLON||symbol==LBKET)
            {
                vartopro();
            }

            if(symbol==LPAREN)
            {
                fictopro();
            }
        }

        else
        {
            error(1);
        }
    }
}




int main()
{

    int i;



    i = openfile();
    if(i==0)
        return 0;

    printgetsym();

    /*
    if(freopen("output.txt","w",stdout)==NULL)
    {
        printf("file to open the output file\n");
        return 1;
    }
    */

    free(file);
       return 0;
}

