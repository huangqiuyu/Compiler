#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define MAXSTRLENG 2000  //max length of string's size 字符串的最大数量
#define MAXMEDI     8000 //max medial size 中间表达式的最大数量
#define MAXTEMP     4000  //中间临时变量的数量
#define MAXMSTACK   10000     //
#define MAXLABELS   300 //最大标签数



//四元式操作符定义
#define	ADD	0
#define	SUB	1
#define	MUL	2
#define	DIV	3
#define	BIG	4
#define	SMA	5
#define	EQU	6
#define	BEQU	7
#define	SEQU	8
#define	NOT	9
#define	NEQU	10
#define	ASN	11
#define	BEQ	12
#define	GOTO	13
#define	LAB	14
#define	GLOBEG	15
#define	BEGIN	16
#define	END	17
#define	SW	18
#define	LW	19
#define	CALL	20
#define	FORM	21
#define	RETURN	22
#define PARA 23
#define	ADDI	24
#define	SUBI	25
#define	RD	26
#define	WRT	27
#define JR 28







char* file; //= (char*)malloc(sizeof(char)*MAXPATH);//file of input
char* file1;

char linebuf[MAXLS];  //line buffer to storage one line of the input file 保存读入文件的一行，行缓冲区
int lineIndex=MAXLS-1;  //index of the linebuffer 行缓冲区指针

//char ch;    //last charactor read 最近一次读入的字符
int symbol=0;     //last symbol read 最近一次识别出来的单词类型
char id[MAXIDS];    //last identifier read 最近一次识别出来的标识符名称

int num=0;    //last const number read 最近一次读入的整常数的值
char chconst;       //last const character read 最近一次读入的字符常量的值
char str[MAXSTR];   //last output string read 最近一次读入的字符串

int isFileEnd;

int linenum=0;    //current line of read file 当前读到的行，用于出错处理的定位

int infunc;

int curscope=0;//当前的作用域

//symbol栈，int或char
int tmpstack[50]={-1};
int stackindex=1;


//分配寄存器编号，轮盘制
int regid=0;


int curcompifunid=0;//当前编译的函数的medsindex

//int haveretuval=FALSE;


//return 栈，return的返回值所在的medstack索引
struct{
    int id;
    int scope;
}returnstack;



//全局变量的偏移量
int offset=0;
//局部变量的偏移量
int tmpoffset = 0;


int calmyparasum = 0;

//table 符号表
struct{
    char name[MAXIDS];//名字
    int obj;   //constant variable function
    int type;   //int char arrays
    int valuei; //value of const integer
    char valuec;    //value of const char
    int offset;     //var or array or function 's offset
    int scope;
    int length;     //
    int locate;     //
    int index;  //在中间栈中的索引id
    int tmpbasic;
    int myparasum;//the sum of function parameter

}table[MAXTAB];


//四元式
struct{
    int op; //option
    int rs;
    int rt;
    int rd;
}medial[MAXMEDI];
int medialindex=0;


//临时变量表
struct{
    int type;//1:int,2:char
    int valuei;
    char valuec;
}tempv[MAXTEMP];
int tempvindex=0;


//总表
struct{
    int id;
    int tbindex;
    int tpindex;
    int lableindex;
}medstack[MAXMSTACK];
int medsindex=0;


//临时寄存器
struct{
    int distribute;//是否已分配
    int index;
}registers[21];


//标签，主要用于if-else，for等，函数不需要
struct{
    char name[10];
    int index;
}labels[MAXLABELS];

int labelindex=0;


//标签栈，保存已经生成但未使用的标签，生成时进栈，使用后退栈
int labelstack[MAXLABELS];
int labstaindex = 0;



int tableindex=0;//index of table

char strings[MAXSTRLENG][MAXSTR];
int strindex=0;

int isjmain = TRUE;




//function declare

int findtable(char name[MAXIDS],int obj);
int ftableindex(char name[MAXIDS]);
void genlabel();
void malloctb(int index);
void mallocint();
void mallocchar();
void malloclab(int index);
void genmedi(int o,int rs,int rt,int rd);

void error(int i);
char getch();
void goback();
void getsym();
int openfile();
void printgetsym();
void constopro();
void constdec();
void tovardec();
void vardec();
void mixsta();
void statements();
void statemt();

int condition();
void ifsta();
void loopsta();
void callsta(int idenid);
void assignsta(int idenid);
void readsta();
void writesta();
void returnsta();
int expression();
int term();
int factor();
int isint();
void inttoconst();
void vartopro();
void parameter();
void fictopro();
void fvotopro();
void ismain();
void program();







//主要用于查重复定义，全局变量和局部变量可以重名
//若是局部变量只需要查函数内部是否重复定义，不需要查函数外部的全局变量
int findtable(char name[MAXIDS],int obj)
{
    int i;
    if(tableindex<2)
        return -1;

    switch(obj)
    {
        case 1:
            for(i=tableindex-1;i>=0&&table[i].scope==curscope;i--)
            {
                if(strcmp(name,table[i].name)==0)
                    return i;
            }
            return -1;

            break;
        case 2:
            for(i=tableindex-1;i>=0&&table[i].scope==curscope;i--)
            {
                if(strcmp(name,table[i].name)==0)
                    return i;
            }
            return -1;
            break;
        case 3:
            for(i = tableindex-1;i>=0;i--)
            {
                if(table[i].obj==3)
                {
                    if(strcmp(name,table[i].name)==0)
                        return i;
                }
            }
            return -1;
            break;
        case 4:
            for(i=tableindex-1;i>=0&&table[i].scope==curscope;i--)
            {
                if(strcmp(name,table[i].name)==0)
                        return i;
            }
            return -1;
            break;
    }



    return -1;

}

//主要用于运算的时候查表，查最靠近栈顶的位置，
//若是出现在函数内，不仅要查局部变量还要查全局变量。
int ftableindex(char name[MAXIDS])
{
    int i;
    if(tableindex==0)
        return -1;

    for(i=tableindex-1;i>=0&&table[i].scope==curscope;i--)
    {
        if(strcmp(name,table[i].name)==0)
            return i;
    }

    for(i=0;i<=MAXTAB&&table[i].scope==0;i++)
    {
        if(strcmp(name,table[i].name)==0)
        {
            return i;
        }
    }

    for(i=0;i<=MAXTAB&&i<=tableindex;i++)
    {
        if(table[i].obj==3)
        {
            if(strcmp(name,table[i].name)==0)
            {
                return i;
            }
        }
    }


    return -1;


}

//生成label，label+数字，数字每次加1
void genlabel()
{
    int i;
    char j[5];

    char tmp[10]={"$labe"};

    //
    strcpy(labels[++labelindex].name,tmp);
    itoa(labelindex,j,10);
    for(i=5;i<10&&j[i-5]<='9'&&j[i-5]>='0';i++)
        labels[labelindex].name[i] = j[i-5];
    labels[labelindex].name[i] = '\0';



    //生成的标签未使用，进入标签栈中等待使用
    //labelstack[labstaindex++] = labelindex;
}


void malloctb(int index)
{
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = index;
    medstack[medsindex].tpindex = -1;
    medstack[medsindex].lableindex = -1;

    table[index].index = medsindex;

    medsindex++;
}



void mallocint()
{
    tempv[tempvindex].type = 1;
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = tempvindex;
    medstack[medsindex].lableindex = -1;
    tempvindex++;
    medsindex++;

}

void mallocchar()
{
    tempv[tempvindex].type = 2;
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = tempvindex;
    medstack[medsindex].lableindex = -1;
    tempvindex++;
    medsindex++;
}


void malloclab(int index)
{
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = -1;
    medstack[medsindex].lableindex = index;

    labels[index].index = medsindex;

    medsindex++;
}

void genmedi(int o,int rs,int rt,int rd)
{
    medial[medialindex].op = o;
    medial[medialindex].rs = rs;
    medial[medialindex].rt = rt;
    medial[medialindex].rd = rd;
    medialindex++;
}









void error(int i)
{
    switch(i)
    {
        case 0:
            printf("ERROR!!!!!Can't open the file!\n");
            exit(0);
            break;
        case 1:
            printf("ERROR!!!!!类型标识符后面没有接标识符\n");
            break;

        case 2:
            printf("ERROR!!!!!常量或变量说明中逗号后面没有接标识符\n");
            getsym();
            break;

        case 3:
            printf("ERROR!!!!!数组声明中[]内应该是无符号整数\n");
            getsym();
            break;

        case 4:
            printf("ERROR!!!!!数组声明时长度不能为0\n");
            break;

        case 5:
            printf("ERROR!!!!!数组少了右边的方括号\n");
            break;

        case 6:
            printf("ERROR!!!!!变量说明标识符后面没有跟逗号方括号或分号\n");
            getsym();
            break;

        case 7:
            printf("ERROR!!!!!函数定义中没有左括号\(\n");
            break;

        case 8:
            printf("ERROR!!!!!参数表逗号后面没有接int或char\n");
            getsym();
            break;

        case 9:
            printf("ERROR!!!!!函数定义中没有右括号)\n");
            break;

        case 10:
            printf("ERROR!!!!!函数定义中没有左大括号{\n");
            break;

        case 11:
            printf("ERROR!!!!!函数定义中没有右大括号}\n");
            break;

        case 12:
            printf("ERROR!!!!!函数定义中缺乏返回类型（void int char）\n");
            break;

        case 13:
            printf("ERROR!!!!!无返回值函数定义中缺乏函数名\n");
            break;

        case 14:
            printf("ERROR!!!!!缺乏主函数\n");
            break;

        case 15:
            printf("ERROR!!!!!整数的第一个字符非法\n");
            break;

        case 16:
            printf("ERROR!!!!!整数的正负号后面紧跟着不是数字\n");
            break;

        case 17:
            printf("ERROR!!!!!常量定义中没有int或char\n");
            break;

        case 18:
            printf("ERROR!!!!!常量定义中标识符后面没有等号=\n");
            break;

        case 19:
            printf("ERROR!!!!!常量定义中分隔符非法，不是逗号或分号\n");
            getsym();
            break;

        case 20:
            printf("ERROR!!!!!字符常量定义中缺少字符值\n");
            break;

        case 21:
            printf("ERROR!!!!!标识符超出最大长度，自动截取前20个字符\n");
            break;

        case 22:
            printf("ERROR!!!!!无符号整数不存在前导零\n");
            break;

        case 23:
            printf("ERROR!!!!!标识符出现非法字符\n");
            break;

        case 24:
            printf("ERROR!!!!!字符常量或字符串常量缺少右单引号\n");
            break;

        case 25:
            printf("ERROR!!!!!出现非法字符\n");
            break;

        case 26:
            printf("ERROR!!!!!语句推出语句列时没有右边的大括号}\n");
            break;

        case 27:
            printf("ERROR!!!!!因子推出表达式时缺少右括号\)\n");
            break;

        case 28:
            printf("ERROR!!!!!if后面少了左括号\(\n");
            break;

        case 29:
            printf("ERROR!!!!!if后面少了右括号\)\n");
            break;

        case 30:
            printf("ERROR!!!!!do后面没有while\n");
            break;

        case 31:
            printf("ERROR!!!!!while后面少了左括号\n");
            break;

        case 32:
            printf("ERROR!!!!!while后面少了右括号\n");
            break;

        case 33:
            printf("ERROR!!!!!for后面少了左括号\n");
            break;

        case 34:
            printf("ERROR!!!!!for的左括号后面没有标识符\n");
            break;

        case 35:
            printf("ERROR!!!!!for后面少了右括号\n");
            break;

        case 36:
            printf("ERROR!!!!!for的标识符后面没有等号\n");
            getsym();
            break;

        case 37:
            printf("ERROR!!!!!for赋初值后没有加分号\n");
            getsym();
            break;

        case 38:
            printf("ERROR!!!!!for条件之后没有加分号\n");
            getsym();
            break;

        case 39:
            printf("ERROR!!!!!for的增长值中没有标识符\n");
            break;

        case 40:
            printf("ERROR!!!!!for的增长值中没有加减号\n");
            break;

        case 41:
            printf("ERROR!!!!!for的步长中不是无符号整数\n");
            break;

        case 42:
            printf("ERROR!!!!!步长不能为0\n");
            break;

        case 43:
            printf("ERROR!!!!!函数调用少了右括号\n");
            break;

        case 44:
            printf("ERROR!!!!!读写语句缺少左括号\n");
            break;

        case 45:
            printf("ERROR!!!!!读写语句缺少右括号\n");
            break;

        case 46:
            printf("ERROR!!!!!scanf缺少标识符\n");
            break;

        case 47:
            printf("ERROR!!!!!return缺少右括号\n");
            break;

        case 48:
            printf("ERROR!!!!!语句没有以分号结尾\n");
            break;
        case 49:
            printf("ERROR!!!!!重复定义\n");
            break;

        case 50:
            printf("ERROR!!!!!函数有返回值但是没有return语句\n");
            break;
        case 51:
            printf("ERROR!!!!!使用前未声明\n");
            break;

        case 52:
            printf("ERROR!!!!!不能给常量赋值\n");
            break;

        case 53:
            printf("ERROR!!!!!不能给函数赋值\n");
            break;

        case 54:
            //printf("ERROR!!!!!读语句不能对参数赋值\n");
            break;

        case 55:
            printf("ERROR!!!!!数组的标识符不对，而且其他常量或函数名\n");
            break;

        case 56:
            printf("ERROR!!!!!函数调用中函数名不是函数名，而是其他常量或变量或参数名\n");
            break;

        case 57:
            printf("ERROR!!!!!因子中的函数调用没有返回值\n");
            break;

        case 58:
            printf("ERROR!!!!!void的函数定义中return后面有返回值\n");
            break;

        case 59:
            printf("ERROR!!!!!函数调用与定义的参数个数不符\n");
            break;
    }

    printf("code is %d   error is in the line of %d !!!!\n",i,linenum);
}





//read one character 读入一个字符，不直接用getchar因为不能回退
char getch()
{

    if((lineIndex>=MAXLS-1)||linebuf[lineIndex]=='\n'||linebuf[lineIndex]=='\0')
    {
        linenum++;
        if((gets(linebuf))==NULL)
        {

            isFileEnd = TRUE;
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
    if(isFileEnd==TRUE)
        return;

    while(c==' '||c=='\n'||c=='\t')
    {
        c=getch();
        if(isFileEnd==TRUE)
        {
            printf("read the end of file\n");
			return;
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
                error(21);
                break;
            }
            id[i++]=c;
            c=getch();
			if(isFileEnd==TRUE)
				return;
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
			if(isFileEnd==TRUE)
				return;
            if(c>='0'&&c<='9')
            {
                error(22);
            }
            while(c>='0'&&c<='9')
            {
                num = num*10+(c-'0');
                c = getch();
				if(isFileEnd==TRUE)
					return;
            }
        }
        else
        {
            while(c>='0'&&c<='9')
            {
                num = num*10+(c-'0');
                c = getch();
				if(isFileEnd==TRUE)
					return;
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
				if(isFileEnd==TRUE)
					return;

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
				if(isFileEnd==TRUE)
					return;

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
				if(isFileEnd==TRUE)
					return;

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
				if(isFileEnd==TRUE)
					return;

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
                        error(21);
                        break;
                    }
                    id[i++]=c;
                    c=getch();
					if(isFileEnd==TRUE)
						return;
                }
                id[i]='\0';
                goback();
                symbol = IDEN;
                break;

            case '\'':
                chconst = getch();
				if(isFileEnd==TRUE)
					return;

                c = getch();
				if(isFileEnd==TRUE)
					return;

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
                        error(23);
                        symbol = ERRORSYM;
                    }
                }
                else
                {
                    goback();
                    symbol = ERRORSYM;
                    error(24);
                }
                break;

            case '\"':
                j = 0;
                c = getch();
				if(isFileEnd==TRUE)
					return;
                while(c!='\"'&&j<MAXSTR-1)
                {
                    str[j++] = c;
                    c = getch();
					if(isFileEnd==TRUE)
						return;
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
                    error(24);
                }

                break;

            default:
                symbol = ERRORSYM;
                error(25);
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




void constopro()
{
    if(symbol==CONSTSYM)
    {
        getsym();
		if(isFileEnd==TRUE)
			return;

		if(symbol!=INTSYM&&symbol!=CHARSYM)
		{
			error(17);
			while(symbol!=INTSYM&&symbol!=CHARSYM&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return;
			}

		}

        if(symbol==INTSYM)
        {
            getsym();
			if(isFileEnd==TRUE)
					return;

            if(symbol!=IDEN)
            {
				error(1);
                while(symbol!=IDEN&&isFileEnd==FALSE)
				{
					getsym();
					if(isFileEnd==TRUE)
						return;
				}
            }

			if(findtable(id,1)>=0)
				error(49);

			strcpy(table[tableindex].name,id);



			getsym();
			if(isFileEnd==TRUE)
						return;
            if(symbol==ASSIGN)
            {
                    getsym();
					if(isFileEnd==TRUE)
						return;

            }
            else
            {
                    error(18);
            }
            inttoconst();

            table[tableindex].obj = 1;
            table[tableindex].type = 1;
            table[tableindex].valuei = num;
            table[tableindex].scope = curscope;
            malloctb(tableindex);
            tableindex++;

            if(symbol==COMMA)
            {
                while(symbol==COMMA)
                {
                    getsym();
                    if(isFileEnd==TRUE)
                        return;

                    if(symbol==IDEN)
                    {
						if(findtable(id,1)>=0)
							error(49);
                        strcpy(table[tableindex].name,id);

                        getsym();
                        if(isFileEnd==TRUE)
                            return;


                        if(symbol==ASSIGN)
                        {
                            getsym();
                            if(isFileEnd==TRUE)
                                return;

                            inttoconst();


                        }
                        else
                        {
                            error(18);
                            inttoconst();
                        }

                        table[tableindex].obj = 1;
                        table[tableindex].type = 1;
                        table[tableindex].valuei = num;
                        table[tableindex].scope = curscope;
                        malloctb(tableindex);
                        tableindex++;
                    }
                    else
                    {
                        error(2);
                    }
                }
                if(symbol!=SEMICOLON)
                {
                    error(19);
                }
            }
            if(symbol==SEMICOLON)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;

                if(symbol==CONSTSYM)
                {
                    constopro();
                }
            }
            else
            {
                error(19);
            }

        }
        else if(symbol==CHARSYM)
        {
            getsym();
			if(isFileEnd==TRUE)
				return;

            if(symbol!=IDEN)
            {
                error(1);
                while(symbol!=IDEN&&isFileEnd==FALSE)
				{
					getsym();
					if(isFileEnd==TRUE)
						return;
				}
            }

			if(findtable(id,1)>=0)
				error(49);
            strcpy(table[tableindex].name,id);


			getsym();
			if(isFileEnd==TRUE)
				return;
                if(symbol==ASSIGN)
                {
                    getsym();
					if(isFileEnd==TRUE)
						return;

                }
                else
                {
                    error(18);
                }

                if(symbol!=CHARCST)
                {
						error(20);
                        while(symbol!=CHARCST&&isFileEnd==FALSE)
						{
							getsym();
							if(isFileEnd==TRUE)
								return;
						}


                }

                table[tableindex].obj = 1;
                table[tableindex].type = 2;
                table[tableindex].valuec = chconst;
                table[tableindex].scope = curscope;
                malloctb(tableindex);
                tableindex++;

                   getsym();
				   if(isFileEnd==TRUE)
									return;
                        if(symbol==COMMA)
                        {
                            while(symbol==COMMA)
                            {
                                getsym();
								if(isFileEnd==TRUE)
									return;
                                if(symbol==IDEN)
                                {
									if(findtable(id,1)>=0)
										error(49);
                                    strcpy(table[tableindex].name,id);

                                    getsym();
									if(isFileEnd==TRUE)
										return;
                                    if(symbol==ASSIGN)
                                    {
                                        getsym();
										if(isFileEnd==TRUE)
											return;

                                    }
                                    else
                                    {
                                        error(18);
                                    }

									 if(symbol!=CHARCST)
                                    {
											error(20);
                                            while(symbol!=CHARCST&&isFileEnd==FALSE)
											{
												getsym();
												if(isFileEnd==TRUE)
													return;
											}
                                    }

                                    table[tableindex].obj = 1;
                                    table[tableindex].type = 2;
                                    table[tableindex].valuec = chconst;
                                    table[tableindex].scope = curscope;
                                    malloctb(tableindex);
                                    tableindex++;
                                        getsym();
										if(isFileEnd==TRUE)
											return;
                                }
                                else
                                {
                                    error(2);
                                }
                            }
                            if(symbol!=SEMICOLON)
                            {
                                error(19);
                            }
                        }
                        if(symbol==SEMICOLON)
                        {
                            getsym();
							if(isFileEnd==TRUE)
									return;
                            if(symbol==CONSTSYM)
                            {
                                constopro();
                            }
                        }
                        else
                        {
                            error(19);
                        }
        }

       // printf("This is const declaration.\n");

    }
}


void constdec()
{
    if(symbol==CONSTSYM)
    {

        getsym();
		if(isFileEnd==TRUE)
			return;

		if(symbol!=INTSYM&&symbol!=CHARSYM)
		{
			error(17);
			while(symbol!=INTSYM&&symbol!=CHARSYM&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return;
			}

		}

        if(symbol==INTSYM)
        {


            getsym();
			if(isFileEnd==TRUE)
					return;

            if(symbol!=IDEN)
            {
				error(1);
                while(symbol!=IDEN&&isFileEnd==FALSE)
				{
					getsym();
					if(isFileEnd==TRUE)
						return;
				}
            }

			if(findtable(id,1)>=0)
				error(49);

            strcpy(table[tableindex].name,id);
            table[tableindex].obj = 1;
            table[tableindex].type = 1;


			getsym();
			if(isFileEnd==TRUE)
						return;
            if(symbol==ASSIGN)
            {
                    getsym();
					if(isFileEnd==TRUE)
						return;

            }
            else
            {
                    error(18);
            }
            inttoconst();

            table[tableindex].valuei = num;
            table[tableindex].scope = curscope;

            malloctb(tableindex);

            tableindex++;


            if(symbol==COMMA)
            {
                while(symbol==COMMA)
                {
                    getsym();
                    if(isFileEnd==TRUE)
                        return;

                    if(symbol==IDEN)
                    {
						if(findtable(id,1)>=0)
							error(49);
                        strcpy(table[tableindex].name,id);
                        table[tableindex].obj = 1;
                        table[tableindex].type = 1;

                        getsym();
                        if(isFileEnd==TRUE)
                            return;


                        if(symbol==ASSIGN)
                        {
                            getsym();
                            if(isFileEnd==TRUE)
                                return;

                            inttoconst();
                        }
                        else
                        {
                            error(18);
                            inttoconst();
                        }

                        table[tableindex].valuei = num;
                        table[tableindex].scope = curscope;
                        malloctb(tableindex);
                        tableindex++;

                    }
                    else
                    {
                        error(2);
                    }
                }
                if(symbol!=SEMICOLON)
                {
                    error(19);
                }
            }
            if(symbol==SEMICOLON)
            {


                getsym();
                if(isFileEnd==TRUE)
                    return;

                if(symbol==CONSTSYM)
                {
                    constdec();
                }
            }
            else
            {
                error(19);
            }

        }
        else if(symbol==CHARSYM)
        {

            getsym();
			if(isFileEnd==TRUE)
				return;

            if(symbol!=IDEN)
            {
                error(1);
                while(symbol!=IDEN&&isFileEnd==FALSE)
				{
					getsym();
					if(isFileEnd==TRUE)
						return;
				}
            }

			if(findtable(id,1)>=0)
				error(49);
            strcpy(table[tableindex].name,id);
            table[tableindex].obj = 1;
            table[tableindex].type = 2;


			getsym();
			if(isFileEnd==TRUE)
				return;
                if(symbol==ASSIGN)
                {
                    getsym();
					if(isFileEnd==TRUE)
						return;

                }
                else
                {
                    error(18);
                }

                if(symbol!=CHARCST)
                    {
						error(20);
                        while(symbol!=CHARCST&&isFileEnd==FALSE)
						{
							getsym();
							if(isFileEnd==TRUE)
								return;
						}
                    }

                    table[tableindex].valuec = chconst;
                    table[tableindex].scope = curscope;
                    malloctb(tableindex);
                    tableindex++;


                   getsym();
				   if(isFileEnd==TRUE)
									return;
                        if(symbol==COMMA)
                        {
                            while(symbol==COMMA)
                            {
                                getsym();
								if(isFileEnd==TRUE)
									return;
                                if(symbol==IDEN)
                                {
									if(findtable(id,1)>=0)
										error(49);
                                    strcpy(table[tableindex].name,id);
                                table[tableindex].obj = 1;
                                table[tableindex].type = 2;

                                    getsym();
									if(isFileEnd==TRUE)
										return;
                                    if(symbol==ASSIGN)
                                    {
                                        getsym();
										if(isFileEnd==TRUE)
											return;

                                    }
                                    else
                                    {
                                        error(18);
                                    }

									 if(symbol!=CHARCST)
                                        {
											error(20);
                                            while(symbol!=CHARCST&&isFileEnd==FALSE)
											{
												getsym();
												if(isFileEnd==TRUE)
													return;
											}
                                        }

                                    table[tableindex].valuec = chconst;
                                    table[tableindex].scope = curscope;
                                    malloctb(tableindex);
                                    tableindex++;


                                        getsym();
										if(isFileEnd==TRUE)
											return;
                                }
                                else
                                {
                                    error(2);
                                }
                            }
                            if(symbol!=SEMICOLON)
                            {
                                error(19);
                            }
                        }
                        if(symbol==SEMICOLON)
                        {
                            getsym();
							if(isFileEnd==TRUE)
									return;
                            if(symbol==CONSTSYM)
                            {
                                constdec();
                            }
                        }
                        else
                        {
                            error(19);
                        }
        }

       // printf("This is const declaration.\n");

    }
}




void tovardec()
{
    if(symbol==COMMA)
    {
        if(table[tableindex].type==0)
        {
            if(tmpstack[stackindex]==INTSYM)
            {
                table[tableindex].type = 1;

            }

            else if(tmpstack[stackindex]==CHARSYM)
            {
                table[tableindex].type = 2;
            }

            malloctb(tableindex);
            //genmedi(SW,medsindex-1,0,0);



        }

        tableindex++;


        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==IDEN)
        {
			if(findtable(id,2)>=0)
				error(49);
            strcpy(table[tableindex].name,id);
            table[tableindex].obj = 2;
            if(tmpstack[stackindex]==INTSYM)
                table[tableindex].type = 1;
            else if(tmpstack[stackindex]==CHARSYM)
                table[tableindex].type = 2;

            malloctb(tableindex);
            //genmedi(SW,medsindex-1,0,0);

            table[tableindex].offset = tmpoffset++;
            table[tableindex].scope = curscope;

           // tableindex++;


            getsym();
			if(isFileEnd==TRUE)
				return;
            tovardec();
        }
        else
        {
            error(2);
        }
    }

    if(symbol==LBKET)
    {
        if(tmpstack[stackindex]==INTSYM)
            table[tableindex].type = 3;
        else if(tmpstack[stackindex]==CHARSYM)
            table[tableindex].type = 4;

        tmpoffset--;

        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==NUMBER)
        {
            if(num==0)
            {

                error(4);
                num = 10;
            }
            getsym();
			if(isFileEnd==TRUE)
				return;
        }

		else
        {

            error(3);
            num = 10;

        }

        table[tableindex].length = num;



        if(symbol!=RBKET)
        {
				error(5);
        }
		else
		{
			getsym();
			if(isFileEnd==TRUE)
				return;
		}

		malloctb(tableindex);
        //genmedi(SW,medsindex-1,0,0);

        //tableindex++;
        tmpoffset += num;

         if(symbol==COMMA)
        {
            tableindex++;

                getsym();
				if(isFileEnd==TRUE)
					return;
                if(symbol==IDEN)
                {
					if(findtable(id,2)>=0)
						error(49);
                    strcpy(table[tableindex].name,id);
                    table[tableindex].obj = 2;

                    table[tableindex].offset = tmpoffset++;
                    table[tableindex].scope = curscope;

                    table[tableindex].type = 0;


                    getsym();
						if(isFileEnd==TRUE)
							return;
                        tovardec();
                }
                else
                {
                        error(2);
                }
         }

    }

    if(symbol==SEMICOLON)
    {
        if(table[tableindex].type==0)
        {
            if(tmpstack[stackindex]==INTSYM)
            {
                table[tableindex].type = 1;

            }

            else if(tmpstack[stackindex]==CHARSYM)
            {
                table[tableindex].type = 2;
            }

            malloctb(tableindex);
        }


        stackindex--;
        tableindex++;

        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==INTSYM||symbol==CHARSYM)
        {
            vardec();
        }
    }



}

void vardec()
{
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        tmpstack[++stackindex] = symbol;

        table[tableindex].obj = 2;
        table[tableindex].type = 0;


        table[tableindex].offset = tmpoffset++;
        table[tableindex].scope = curscope;



        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol!=IDEN)
        {
            error(1);
            while(symbol!=IDEN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }

        }

		if(findtable(id,2)>=0)
				error(49);
        strcpy(table[tableindex].name,id);


        getsym();
        if(isFileEnd==TRUE)
            return;



        tovardec();

       // printf("This is variation declaration.\n");

    }


}





void mixsta()
{
    if(symbol==CONSTSYM)
    {
        constdec();
    }

    if(symbol==INTSYM||symbol==CHARSYM)
    {
        vardec();
    }

    statements();

   // printf("This is mixture statements.\n");
}

void statements()
{
    if(symbol==IFSYM||symbol==DOSYM||symbol==FORSYM||symbol==LBRACE||symbol==IDEN||
       symbol==SCANFSYM||symbol==PRINTFSYM||symbol==SEMICOLON||symbol==RETURNSYM)
    {
        while((symbol==IFSYM||symbol==DOSYM||symbol==FORSYM||symbol==LBRACE||symbol==IDEN||
              symbol==SCANFSYM||symbol==PRINTFSYM||symbol==SEMICOLON||symbol==RETURNSYM)
              &&isFileEnd==FALSE)
        {
             statemt();

        }
         //printf("This is statements.\n");
    }



}

void statemt()
{
    int idenid;
    if(symbol==IFSYM)
    {
        ifsta();

    }
    else if(symbol==DOSYM||symbol==FORSYM)
    {
        loopsta();

    }
    else if(symbol==LBRACE)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        statements();
        if(symbol!=RBRACE)
        {
            error(26);
            while(symbol!=RBRACE&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;

            }
        }
        getsym();
        if(isFileEnd==TRUE)
            return;
                 ///////////////
    }

    else if(symbol==IDEN)
    {

        idenid = ftableindex(id);

        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol==LPAREN)
        {
            if(idenid<0)
            {
                error(51);

            }
            else
            {
                if(table[idenid].obj!=3)
                {
                    error(56);
                }


                idenid = table[idenid].index;

            }

            callsta(idenid);
            if(symbol!=SEMICOLON)
            {
                error(48);
            }
            getsym();
            if(isFileEnd==TRUE)
					 return;
        }
        else if(symbol==LBKET||symbol==ASSIGN)
        {

            if(idenid<0)
            {
                error(51);

            }
            else
            {
                if(table[idenid].obj==1)
                {
                    error(52);
                }
                else if(table[idenid].obj==3)
                {
                    error(53);
                }

                idenid = table[idenid].index;

            }

            assignsta(idenid);
            if(symbol!=SEMICOLON)
            {
                error(48);
            }
            getsym();
             if(isFileEnd==TRUE)
					 return;
        }
    }

    else if(symbol==SCANFSYM)
    {
        readsta();
        if(symbol!=SEMICOLON)
        {
            error(48);
        }
        getsym();
        if(isFileEnd==TRUE)
			return;
    }

    else if(symbol==PRINTFSYM)
    {
        writesta();
        if(symbol!=SEMICOLON)
        {
            error(48);
        }
        getsym();
        if(isFileEnd==TRUE)
			return;

    }
    else if(symbol==SEMICOLON)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

    }
    else if(symbol==RETURNSYM)
    {
        returnsta();
        if(symbol!=SEMICOLON)
        {
            error(48);
        }
        getsym();
        if(isFileEnd==TRUE)
			return;
    }


}

int condition()
{
    int tmp1,tmp2,tmp3,flag;
    tmp1 = expression();

    if(symbol==LSS||symbol==LEQ||symbol==GRT||symbol==GEQ||symbol==NEQ||symbol==EQL)
    {
        switch(symbol)
        {
            case LSS:
                flag = 1;//<
                break;
            case LEQ:
                flag = 2;//<=
                break;
            case GRT:
                flag = 3;//>
                break;
            case GEQ:
                flag = 4;//>=
                break;
            case NEQ:
                flag = 5; //!=
                break;
            case EQL:
                flag = 6;//==
                break;
        }


        getsym();
        if(isFileEnd==TRUE)
                return 0;
        tmp2 = expression();

        mallocint();
        tmp3 = medsindex-1;

        switch(flag)
        {
            //<
            case 1:
                genmedi(SMA,tmp1,tmp2,tmp3);
                break;
            //<=
            case 2:
                genmedi(SEQU,tmp1,tmp2,tmp3);
                break;
            //>
            case 3:
                genmedi(BIG,tmp1,tmp2,tmp3);
                break;
            //>=
            case 4:
                genmedi(BEQU,tmp1,tmp2,tmp3);
                break;
            //!=
            case 5:
                genmedi(NEQU,tmp1,tmp2,tmp3);
                break;
            //==
            case 6:
                genmedi(EQU,tmp1,tmp2,tmp3);
                break;


        }


        return tmp3;

    }

    return tmp1;

}

void ifsta()
{
    int beqs1,iflabel,endlabel12;

    if(symbol==IFSYM)
    {
        genlabel();
        malloclab(labelindex);
        iflabel = medsindex-1;



        getsym();
		if(isFileEnd==TRUE)
			return;

        if(symbol!=LPAREN)
        {
            error(28);
        }
        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }



        beqs1 = condition();//beqs1 is id

        genmedi(BEQ,beqs1,0,iflabel);



        if(symbol!=RPAREN)
        {
            error(29);
        }
        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }
        statemt();

        if(symbol==ELSESYM)
        {
            genlabel();
            malloclab(labelindex);
            endlabel12 = medsindex-1;

            genmedi(GOTO,0,0,endlabel12);

            genmedi(LAB,iflabel,0,0);


            getsym();
            if(isFileEnd==TRUE)
                return;



            statemt();


            genmedi(LAB,endlabel12,0,0);

           // genmedi(LAB,endlabel12,0,0);
        }

        else
        {
            genmedi(LAB,iflabel,0,0);
        }

       //  printf("This is if statement.\n");
    }


}

void loopsta()
{
    int dolabel,beqs,forlabel1,forlabel2;
    int tmp1,tmp2,idenid,tmpnum;
    int addorsub=0;

    if(symbol==DOSYM)
    {
        genlabel();
        malloclab(labelindex);
        dolabel = medsindex - 1;

        genmedi(LAB,dolabel,0,0);

        getsym();
        if(isFileEnd==TRUE)
            return;

        statemt();


        if(symbol!=WHILESYM)
        {
            error(30);
            while(symbol!=WHILESYM&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }
        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol!=LPAREN)
        {
            error(31);
        }

        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }

        beqs = condition();

        genmedi(BEQ,beqs,0,dolabel);

        if(symbol!=RPAREN)
        {
            error(32);
        }
        getsym();
        if(isFileEnd==TRUE)
            return;

        //printf("This is do-while statement.\n");

    }
    else if(symbol==FORSYM)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol!=LPAREN)
        {
            error(33);
        }
        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }

        if(symbol!=IDEN)
        {
            error(34);
            while(symbol!=IDEN)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }

        idenid = ftableindex(id);

        if(idenid<0)
        {
            error(51);

        }
        else
        {
            if(table[idenid].obj==1)
            {
                error(52);
            }
            else if(table[idenid].obj==3)
            {
                error(53);
            }

            idenid = table[idenid].index;

        }

         getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol!=ASSIGN)
        {
            error(36);
        }

        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }
        tmp1 = expression();

        genmedi(ASN,tmp1,0,idenid);

        genlabel();
        malloclab(labelindex);
        forlabel1 = medsindex -1;

        genlabel();
        malloclab(labelindex);
        forlabel2 = medsindex -1;

        genmedi(LAB,forlabel1,0,0);

        if(symbol!=SEMICOLON)
        {
            error(37);

        }

        else
        {


             getsym();
            if(isFileEnd==TRUE)
                return;

            beqs = condition();



            genmedi(BEQ,beqs,0,forlabel2);



            if(symbol!=SEMICOLON)
            {
                error(38);
            }
            else
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }

            if(symbol!=IDEN)
            {
                error(39);
                while(symbol!=IDEN&&isFileEnd==FALSE)
                {
                    getsym();
                    if(isFileEnd==TRUE)
                        return;
                }
            }

            idenid = ftableindex(id);

            if(idenid<0)
            {
                error(51);

            }
            else
            {
                if(table[idenid].obj==1)
                {
                    error(52);
                }
                else if(table[idenid].obj==3)
                {
                    error(53);
                }

                idenid = table[idenid].index;

            }


            getsym();
            if(isFileEnd==TRUE)
                  return;

            if(symbol!=ASSIGN)
            {
                error(36);
            }

            else
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
                if(symbol!=IDEN)
                {
                    error(39);
                    while(symbol!=IDEN&&isFileEnd==FALSE)
                    {
                        getsym();
                        if(isFileEnd==TRUE)
                            return;
                    }
                }


                tmp2 = ftableindex(id);
                if(tmp2<0)
                {
                    error(51);
                }
                else
                {
                    if(table[idenid].obj==3)
                    {
                        error(53);
                    }
                    tmp2 = table[tmp2].index;
                }


                getsym();
                if(isFileEnd==TRUE)
                    return;

                if(!(symbol==PLUS||symbol==MINUS))
                {
                    error(40);
                    while(symbol!=PLUS&&symbol!=MINUS&&isFileEnd==FALSE)
                    {
                        getsym();
                        if(isFileEnd==TRUE)
                            return;
                    }
                }

                if(symbol==PLUS)
                    addorsub = 1;
                else
                    addorsub = 2;


                 getsym();
                if(isFileEnd==TRUE)
                    return;
                if(symbol!=NUMBER)
                {
                    error(41);
                    while(symbol!=NUMBER&&isFileEnd==FALSE)
                    {
                        getsym();
                        if(isFileEnd==TRUE)
                            return;
                    }
                }

                if(num==0)
                {
                    error(42);
                    num = 1;
                }

                tmpnum = num;


                getsym();
                if(isFileEnd==TRUE)
                    return;

                if(symbol!=RPAREN)
                {
                    error(35);
                }

                else
                {
                    getsym();
                    if(isFileEnd==TRUE)
                        return;
                }



                statemt();

                //add
                if(addorsub==1)
                {
                    genmedi(ADDI,tmp2,tmpnum,idenid);
                }
                //sub
                else if(addorsub==2)
                {
                    genmedi(SUBI,tmp2,tmpnum,idenid);
                }

                genmedi(GOTO,0,0,forlabel1);


            }

        }

        genmedi(LAB,forlabel2,0,0);

       // printf("This is for statement.\n");

    }

}


void callsta(int idenid)
{

    int tmp1,i =0,callftableid;

    if(symbol==LPAREN)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol!=RPAREN)
        {
            tmp1 = expression();

            genmedi(FORM,tmp1,i++,0);

            while(symbol==COMMA&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
                tmp1 = expression();
                genmedi(FORM,tmp1,i++,0);
            }
            callftableid = medstack[idenid].tbindex;
            if(table[callftableid].myparasum!=(i))
            {
                error(59);
            }


        }

        if(symbol!=RPAREN)
        {
            error(43);
            while(symbol!=RPAREN)
            {
                 getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }

        genmedi(CALL,idenid,0,0);

        getsym();
        if(isFileEnd==TRUE)
            return;

      //  printf("This is call statement.\n");
    }

}

void assignsta(int idenid)
{
    int tmp1,tmp2;
    if(symbol==LBKET)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        tmp1 = expression();
        if(symbol!=RBKET)
        {
            error(5);
        }
        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }

        if(symbol==ASSIGN)
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
            tmp2 = expression();

            genmedi(SW,idenid,tmp1,tmp2);

        }

    }

    else if(symbol==ASSIGN)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;
        tmp2 = expression();
        genmedi(ASN,tmp2,0,idenid);

    }

   // printf("This is assign statement.\n");
}

void readsta()
{
    int tmp1;
    if(symbol==SCANFSYM)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol!=LPAREN)
        {
            error(44);
            while(symbol!=LPAREN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }
        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol!=IDEN)
        {
            error(46);
            while(symbol!=IDEN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }

        tmp1 = ftableindex(id);
        if(tmp1<0)
        {
            error(51);

        }
        else
        {
            if(table[tmp1].obj==1)
            {
                error(52);
            }
            else if(table[tmp1].obj==3)
            {
                error(53);
            }

            tmp1 = table[tmp1].index;

        }

        genmedi(RD,tmp1,0,0);


        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol==COMMA)
        {
            while(symbol==COMMA&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
                if(symbol!=IDEN)
                {
                    error(46);
                    break;
                }

                tmp1 = ftableindex(id);
                if(tmp1<0)
                {
                    error(51);

                }
                else
                {
                    if(table[tmp1].obj==1)
                    {
                        error(52);
                    }
                    else if(table[tmp1].obj==3)
                    {
                        error(53);
                    }
                    else if(table[tmp1].obj==4)
                    {
                        error(54);
                    }

                    tmp1 = table[tmp1].index;

                }

                genmedi(RD,tmp1,0,0);


                getsym();
                if(isFileEnd==TRUE)
                    return;
            }

        }
        if(symbol!=RPAREN)
        {
            error(45);
            while(symbol!=RPAREN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }

        getsym();
        if(isFileEnd==TRUE)
            return;

        //printf("This is read statement.\n");

    }
}

void writesta()
{
    int tmp1;
    if(symbol==PRINTFSYM)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol!=LPAREN)
        {
            error(44);
            while(symbol!=LPAREN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }
        }
        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol==STRING)
        {
            strcpy(strings[strindex],str);
            table[tableindex].obj = 5;
            table[tableindex].scope = curscope;
            table[tableindex].locate = strindex;

            malloctb(tableindex);

            genmedi(WRT,medsindex-1,0,0);

            tableindex++;
            strindex++;


            getsym();
            if(isFileEnd==TRUE)
                return;
            if(symbol==COMMA)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
                tmp1 = expression();
                genmedi(WRT,tmp1,0,0);
            }

        }

        else
        {
            tmp1 = expression();
            genmedi(WRT,tmp1,0,0);

        }

          if(symbol!=RPAREN)
            {
                error(45);
                while(symbol!=RPAREN&&isFileEnd==FALSE)
                {
                    getsym();
                    if(isFileEnd==TRUE)
                        return;
                }
            }

            getsym();
            if(isFileEnd==TRUE)
                return;

     //   printf("This is write statement.\n");
    }
}

void returnsta()
{
    int tmp1,tmpfuntableid;
    if(symbol==RETURNSYM)
    {

        returnstack.id = curcompifunid;
        returnstack.scope = curscope;


        getsym();
        if(isFileEnd==TRUE)
            return;




        if(symbol==LPAREN)
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
            //returnstack.id = expression();
            //returnstack.scope = curscope;



            tmp1 = expression();
            if(symbol!=RPAREN)
            {
                error(47);
                while(symbol!=RPAREN)
                {
                    getsym();
                    if(isFileEnd==TRUE)
                        return;
                }
            }


            //void but have return values
            tmpfuntableid = medstack[curcompifunid].tbindex;
            if(table[tmpfuntableid].type==3)
            {
                error(58);
            }


            genmedi(RETURN,tmp1,0,0);
            genmedi(END,curcompifunid,0,0);

            getsym();
            if(isFileEnd==TRUE)
                return;

        }

        else
        {
            //have return value but return;
            tmpfuntableid = medstack[curcompifunid].tbindex;
            if(table[tmpfuntableid].type!=3)
            {
                error(50);
                genmedi(RETURN,0,0,0);
            }

            genmedi(END,curcompifunid,0,0);
        }

        genmedi(JR,0,0,0);

       // printf("This is return statement.\n");

    }
}

int expression()
{
    int tmp1,tmp2,tmp3,flag;
    if(symbol==PLUS||symbol==MINUS)
    {
        if(symbol==PLUS)
            flag = 1;
        else
            flag = 2;
        getsym();
		if(isFileEnd==TRUE)
			return 0;
    }

    tmp1 = term();

    if(flag==2)
    {
        mallocint();
        tmp2 = medsindex-1;
        genmedi(NOT,tmp1,0,tmp2);
    }
    else
    {
        tmp2 = tmp1;
    }


    if(symbol==PLUS||symbol==MINUS)
    {
        while((symbol==PLUS||symbol==MINUS)&&isFileEnd==FALSE)
        {
            if(symbol==PLUS)
                flag = 1;
            else
                flag = 2;

            getsym();
            if(isFileEnd==TRUE)
                return 0;

            tmp1 = term();

            if(flag==2)
            {
                mallocint();
                tmp3 = medsindex-1;
                genmedi(SUB,tmp2,tmp1,tmp3);
            }
            else
            {
                mallocint();
                tmp3 = medsindex-1;
                genmedi(ADD,tmp2,tmp1,tmp3);
            }

            tmp2 = tmp3;
        }

    }

    return tmp2;


}

int term()
{
    int tmp1,tmp2,tmp3,flag;
    tmp1 = factor();
    if(symbol==TIMES||symbol==DIVI)
    {

        while((symbol==TIMES||symbol==DIVI)&&isFileEnd==FALSE)
        {
            if(symbol==TIMES)
                flag = 1;
            else
                flag = 2;

            getsym();
            if(isFileEnd==TRUE)
                return 0;

            tmp2 = factor();
            mallocint();
            tmp3 = medsindex-1;

            if(flag==1)
            {
                genmedi(MUL,tmp1,tmp2,tmp3);
            }
            else
            {
                genmedi(DIV,tmp1,tmp2,tmp3);
            }

            tmp1 = tmp3;

        }

    }

    return tmp1;
}

int factor()
{
    int tmp1,tmp2,tmp3;

    if(symbol==IDEN)
    {

        tmp1 = ftableindex(id);

         getsym();
         if(isFileEnd==TRUE)
			return 0;

         if(symbol==LPAREN)
         {
             if(tmp1<0)
            {
                error(51);

            }
            else
            {
                if(table[tmp1].obj!=3)
                {
                    error(56);
                }

                else if(table[tmp1].obj==3&&table[tmp1].type==3)
                {
                    error(57);
                }


                tmp1 = table[tmp1].index;

            }




            callsta(tmp1);
            mallocint();
            genmedi(ASN,1,0,medsindex-1);

            return (medsindex-1);
         }

         else if(symbol==LBKET)
         {

             if(tmp1<0)
            {
                error(51);

            }
            else
            {
                if(table[tmp1].obj!=2)
                {
                    error(55);
                }


                tmp1 = table[tmp1].index;

            }

             getsym();
             if(isFileEnd==TRUE)
                return 0;
             tmp2 = expression();

             mallocint();
             tmp3 = medsindex-1;

             genmedi(LW,tmp1,tmp2,tmp3);

             if(symbol!=RBKET)
             {
                 error(5);
             }
             else
			{
				getsym();
				if(isFileEnd==TRUE)
					return 0;
			}

			return tmp3;

         }

         else
         {
            if(tmp1<0)
            {
                error(51);
				tmp1=0;
            }
            else
            {
                if(table[tmp1].obj==3)
                {
                    error(53);
                }


                tmp1 = table[tmp1].index;

            }


            return tmp1;
         }
    }

    else if(symbol==PLUS||symbol==MINUS||symbol==NUMBER)
    {
        tmp1 = isint();
        return tmp1;
    }

    else if(symbol==CHARCST)
    {
        table[tableindex].name[0] = '\0';
        table[tableindex].obj = 1;
        table[tableindex].type = 2;
        table[tableindex].valuec = chconst;
        table[tableindex].scope = curscope;

        malloctb(tableindex);
        tmp1 = medsindex-1;



        tableindex++;



        getsym();
         if(isFileEnd==TRUE)
			return 0;

        return tmp1;
    }

    else if(symbol==LPAREN)
    {
         getsym();
         if(isFileEnd==TRUE)
			return 0;

         tmp1=expression();

         if(symbol!=RPAREN)
         {
             error(27);
             while(symbol!=RPAREN&&isFileEnd==FALSE)
             {
                 getsym();
                 if(isFileEnd==TRUE)
                    return 0;
             }
         }
         getsym();
         if(isFileEnd==TRUE)
			return 0;

        return tmp1;
    }

    else
        return 0;
}

int isint()
{
    int tmp1;
	if(symbol!=PLUS&&symbol!=MINUS&&symbol!=NUMBER)
	{
		error(15);
		while(symbol!=PLUS&&symbol!=MINUS&&symbol!=NUMBER&&isFileEnd==FALSE)
		{
				getsym();
				if(isFileEnd==TRUE)
					return 0;
		}
	}

    if(symbol==PLUS)
    {
        getsym();
		if(isFileEnd==TRUE)
			return 0;
        if(symbol!=NUMBER)
        {
			error(16);
			while(symbol!=NUMBER&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return 0;
			}

        }

        table[tableindex].name[0] = '\0';
        table[tableindex].obj = 1;
        table[tableindex].type = 1;
        table[tableindex].valuei = num;
        table[tableindex].scope = curscope;

        malloctb(tableindex);
        tmp1 = medsindex-1;

        tableindex++;
        //tmpoffset++;

		getsym();
		if(isFileEnd==TRUE)
			return 0;

        return tmp1;

    }
    else if(symbol==MINUS)
    {
        getsym();
		if(isFileEnd==TRUE)
			return 0;
        if(symbol!=NUMBER)
        {
            error(16);
			while(symbol!=NUMBER&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return 0;
			}
        }
        num = 0-num;


        table[tableindex].name[0] = '\0';
        table[tableindex].obj = 1;
        table[tableindex].type = 1;
        table[tableindex].valuei = num;
        table[tableindex].scope = curscope;

        malloctb(tableindex);
        tmp1 = medsindex-1;

        tableindex++;

        getsym();
		if(isFileEnd==TRUE)
			return 0;

        return tmp1;
    }
    else if(symbol==NUMBER)
    {

        table[tableindex].name[0] = '\0';
        table[tableindex].obj = 1;
        table[tableindex].type = 1;
        table[tableindex].valuei = num;
        table[tableindex].scope = curscope;

        malloctb(tableindex);
        tmp1 = medsindex-1;

        tableindex++;

        getsym();
		if(isFileEnd==TRUE)
			return 0;

        return tmp1;
    }
    else
        return 0;
}


void inttoconst()
{
	if(symbol!=PLUS&&symbol!=MINUS&&symbol!=NUMBER)
	{
		error(15);
		while(symbol!=PLUS&&symbol!=MINUS&&symbol!=NUMBER&&isFileEnd==FALSE)
		{
				getsym();
				if(isFileEnd==TRUE)
					return;
		}
	}

    if(symbol==PLUS)
    {
        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol!=NUMBER)
        {
			error(16);
			while(symbol!=NUMBER&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return;
			}

        }


		getsym();
		if(isFileEnd==TRUE)
			return;


    }
    else if(symbol==MINUS)
    {
        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol!=NUMBER)
        {
            error(16);
			while(symbol!=NUMBER&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return;
			}
        }
        num = 0-num;




        getsym();
		if(isFileEnd==TRUE)
			return;

    }
    else if(symbol==NUMBER)
    {

        getsym();
		if(isFileEnd==TRUE)
			return;


    }

}

void vartopro()
{
    if(symbol==COMMA)
    {
        if(table[tableindex].obj==0)
        {
            table[tableindex].obj = 2;
            if(tmpstack[stackindex]==INTSYM)
                table[tableindex].type = 1;
            else if(tmpstack[stackindex]==CHARSYM)
                table[tableindex].type = 2;

            malloctb(tableindex);
            genmedi(GLOBEG,medsindex-1,0,0);

            //table[tableindex].offset = offset++;
            table[tableindex].scope = curscope;


        }

        tableindex++;

        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==IDEN)
        {
			if(findtable(id,2)>=0)
				error(49);
            strcpy(table[tableindex].name,id);
            table[tableindex].offset = offset++;

            getsym();
			if(isFileEnd==TRUE)
				return;
            vartopro();
        }
        else
        {
            error(2);
        }

    }

    if(symbol==LBKET)
    {
        table[tableindex].obj = 2;
        if(tmpstack[stackindex]==INTSYM)
            table[tableindex].type = 3;
        else if(tmpstack[stackindex]==CHARSYM)
            table[tableindex].type = 4;



        offset--;

        table[tableindex].scope = curscope;



        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==NUMBER)
        {
            if(num==0)
            {

                error(4);
                num = 10;
            }



            getsym();
			if(isFileEnd==TRUE)
				return;
        }

		else
        {

            error(3);
            num = 10;

        }

        malloctb(tableindex);
        genmedi(GLOBEG,medsindex-1,0,0);

        table[tableindex].length = num;
        offset += num;


        if(symbol!=RBKET)
        {
            error(5);

        }
        else
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }



        if(symbol==COMMA)
        {
            tableindex++;

            getsym();
            if(isFileEnd==TRUE)
                return;
            if(symbol==IDEN)
            {
				if(findtable(id,2)>=0)
					error(49);
                strcpy(table[tableindex].name,id);
                table[tableindex].offset = offset++;
                //flag
                table[tableindex].obj = 0;
                table[tableindex].type = 0;

                getsym();
                if(isFileEnd==TRUE)
                    return;
                vartopro();
            }
            else
            {
                error(2);
            }
        }


    }

    if(symbol==SEMICOLON)
    {
        //[];
        if(table[tableindex].obj!=0&&table[tableindex].type!=0)
        {
            tableindex++;
        }
        //标;
        else
        {
            table[tableindex].obj = 2;
            if(tmpstack[stackindex]==INTSYM)
                table[tableindex].type = 1;
            else if(tmpstack[stackindex]==CHARSYM)
                table[tableindex].type = 2;

            malloctb(tableindex);
            genmedi(GLOBEG,medsindex-1,0,0);

            //table[tableindex].offset = offset++;
            table[tableindex].scope = curscope;


            tableindex++;
        }

        stackindex--;

      //  printf("This is varaible declaration for program.\n");
        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==INTSYM||symbol==CHARSYM)
        {
            tmpstack[++stackindex] = symbol;

            getsym();
			if(isFileEnd==TRUE)
				return;
            if(symbol!=IDEN)
            {
				error(1);
                while(symbol!=IDEN&&isFileEnd==FALSE)
				{
					getsym();
					if(isFileEnd==TRUE)
						return;
				}

            }

            strcpy(table[tableindex].name,id);
            table[tableindex].offset = offset++;

            //flag
            table[tableindex].obj = 0;
            table[tableindex].type = 0;

			getsym();
			if(isFileEnd==TRUE)
				return;
            if(symbol==COMMA||symbol==SEMICOLON||symbol==LBKET)
            {
                if(findtable(table[tableindex].name,2)>=0)
                    error(49);
                vartopro();
            }

            if(symbol==LPAREN)
            {
                ///////////////////////////////////
                if(isjmain==FALSE)
                {
                    genmedi(GOTO,0,0,2);
                    isjmain = TRUE;
                }
                tmpoffset = 0;
                curscope++;
                if(findtable(table[tableindex].name,3)>=0)
                    error(49);
                fictopro();
            }
        }
    }


}

void parameter()
{
    int paraid;
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        table[tableindex].obj = 4;
        if(symbol==INTSYM)
        {
            table[tableindex].type = 1;

        }

        else if(symbol==CHARSYM)
        {
            table[tableindex].type = 2;
        }




        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol!=IDEN)
        {
			error(1);
            while(symbol!=IDEN&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return;
			}

        }

		if(findtable(id,4)>=0)
				error(49);
        strcpy(table[tableindex].name,id);
        table[tableindex].scope = curscope;
        table[tableindex].offset = tmpoffset++;

        malloctb(tableindex);
        paraid = medsindex-1;

        calmyparasum++;

        genmedi(PARA,paraid,0,0);


        tableindex++;

        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==COMMA)
        {
            getsym();
			if(isFileEnd==TRUE)
				return;
            parameter();
        }
    }
    else
    {
        error(8);
    }
}

void fictopro()
{
    int tableid,funid;
    int tmpbegin,tmpend;
    if(symbol!=LPAREN)
    {
		error(7);
        while(symbol!=LPAREN&&isFileEnd==FALSE)
		{
				getsym();
				if(isFileEnd==TRUE)
					return;
		}
    }

    malloctb(tableindex);
    funid = medsindex-1;
    curcompifunid = funid;
    genmedi(LAB,funid,0,0);
    genmedi(BEGIN,funid,0,0);

    tmpoffset = 0;
    table[tableindex].obj = 3;
    if(tmpstack[stackindex]==INTSYM)
        table[tableindex].type = 1;
    else if(tmpstack[stackindex]==CHARSYM)
        table[tableindex].type = 2;
    table[tableindex].scope = curscope;

    tableid = tableindex;


    calmyparasum=0;





    tmpbegin = tempvindex;
	table[tableindex].tmpbasic = tmpbegin;
	tableindex++;


	getsym();
	if(isFileEnd==TRUE)
			return;
    if(symbol==INTSYM||symbol==CHARSYM)
    {

        parameter();
    }

    if(symbol!=RPAREN)
    {
        error(9);
        while(symbol!=RPAREN&&isFileEnd==FALSE)
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }
    }

    table[tableid].myparasum = calmyparasum;
    calmyparasum = 0;


    getsym();
    if(isFileEnd==TRUE)
        return;
    if(symbol!=LBRACE)
    {
            error(10);
            while(symbol!=LBRACE&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                        return;
            }


    }
    getsym();
    if(isFileEnd==TRUE)
        return;


    mixsta();


    if(symbol!=RBRACE)
    {
        error(11);
        while(symbol!=RBRACE&&isFileEnd==FALSE)
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }

    }


    //have return values

    if(table[tableid].type!=3)
    {

        //no return statement
        if(returnstack.scope!=curscope||returnstack.id!=funid)
        {
            error(50);
            genmedi(RETURN,0,0,0);
            genmedi(END,funid,0,0);
            genmedi(JR,0,0,0);

        }



    }
    else
    {
        //void no return statement
        if(returnstack.scope!=curscope||returnstack.id!=funid)
        {

            genmedi(END,funid,0,0);
            genmedi(JR,0,0,0);

        }
    }





    table[tableid].offset = tmpoffset;

    tmpend = tempvindex;
    table[tableid].length = tmpoffset+tmpend-tmpbegin;



   // printf("This is valued function's declaration for program.\n");
    getsym();
    if(isFileEnd==TRUE)
    return;

    if(symbol!=INTSYM&&symbol!=CHARSYM&&symbol!=VOIDSYM)
	{
        error(12);
        while(symbol!=INTSYM&&symbol!=CHARSYM&&symbol!=VOIDSYM&&isFileEnd==FALSE)
        {
            getsym();
            if(isFileEnd==TRUE)
                    return;
        }
    }
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        tmpstack[++stackindex] = symbol;

        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol!=IDEN)
        {
            error(1);
            while(symbol!=IDEN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }

        }

        if(findtable(id,3)>=0)
				error(49);
        strcpy(table[tableindex].name,id);
        table[tableindex].offset = offset++;

        table[tableindex].obj = 3;

        getsym();
        if(isFileEnd==TRUE)
            return;
        curscope++;
        fictopro();
    }
    else if(symbol==VOIDSYM)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol==IDEN)
        {
            if(isjmain==FALSE)
            {
                genmedi(GOTO,0,0,2);
                isjmain = TRUE;
            }
            curscope++;
            fvotopro();
        }
    }



}

void fvotopro()
{
    int tableid,funid;
    int tmpbegin,tmpend;
    if(symbol!=IDEN)
    {
		error(13);
		while(symbol!=IDEN&&isFileEnd==FALSE)
		{
				getsym();
				if(isFileEnd==TRUE)
					return;
		}

    }

    if(findtable(id,3)>=0)
        error(49);
    strcpy(table[tableindex].name,id);

    malloctb(tableindex);
    funid = medsindex-1;
    curcompifunid = funid;
    genmedi(LAB,funid,0,0);
    genmedi(BEGIN,funid,0,0);

    tmpoffset=0;

    table[tableindex].obj = 3;
    table[tableindex].type = 3;
    table[tableindex].scope = curscope;

    tableid = tableindex;
    tmpbegin = tempvindex;
    table[tableindex].tmpbasic = tmpbegin;

    calmyparasum=0;

    tableindex++;



    getsym();
	if(isFileEnd==TRUE)
		return;

    if(symbol!=LPAREN)
    {
		error(7);
        while(symbol!=LPAREN&&isFileEnd==FALSE)
		{
				getsym();
				if(isFileEnd==TRUE)
					return;
		}
    }

	getsym();
	if(isFileEnd==TRUE)
			return;
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        parameter();
    }



    if(symbol!=RPAREN)
    {
        error(9);
        while(symbol!=RPAREN&&isFileEnd==FALSE)
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }
    }


    table[tableid].myparasum = calmyparasum;
    calmyparasum = 0;

    getsym();
    if(isFileEnd==TRUE)
        return;
    if(symbol!=LBRACE)
    {
            error(10);
            while(symbol!=LBRACE&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                        return;
            }


    }
    getsym();
    if(isFileEnd==TRUE)
        return;
    mixsta();
    if(symbol!=RBRACE)
    {
        error(11);
        while(symbol!=RBRACE&&isFileEnd==FALSE)
        {
            getsym();
            if(isFileEnd==TRUE)
                return;
        }

    }

    //have return values

    if(table[tableid].type!=3)
    {

        //no return statement
        if(returnstack.scope!=curscope||returnstack.id!=funid)
        {
            error(50);
            genmedi(RETURN,0,0,0);
            genmedi(END,funid,0,0);
            genmedi(JR,0,0,0);

        }



    }
    else
    {
        //void no return statement
        if(returnstack.scope!=curscope||returnstack.id!=funid)
        {

            genmedi(END,funid,0,0);
            genmedi(JR,0,0,0);

        }
    }








    table[tableid].offset = tmpoffset;
    tmpend = tempvindex;
    table[tableid].length = tmpoffset+tmpend-tmpbegin;

    //printf("This is void function's declaration for program.\n");
    getsym();
    if(isFileEnd==TRUE)
    return;

    if(symbol!=INTSYM&&symbol!=CHARSYM&&symbol!=VOIDSYM)
	{
        error(12);
        while(symbol!=INTSYM&&symbol!=CHARSYM&&symbol!=VOIDSYM&&isFileEnd==FALSE)
        {
            getsym();
            if(isFileEnd==TRUE)
                    return;
        }
    }
    if(symbol==INTSYM||symbol==CHARSYM)
    {
        tmpstack[++stackindex] = symbol;

        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol!=IDEN)
        {
            error(1);
            while(symbol!=IDEN&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }

        }

        if(findtable(id,3)>=0)
				error(49);
        strcpy(table[tableindex].name,id);
        table[tableindex].offset = offset++;

        table[tableindex].obj = 3;


        getsym();
        if(isFileEnd==TRUE)
            return;
        curscope++;

        if(isjmain==FALSE)
        {
                genmedi(GOTO,0,0,2);
                isjmain = TRUE;
        }

        fictopro();
    }
    else if(symbol==VOIDSYM)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol==IDEN)
        {

            if(isjmain==FALSE)
            {
                genmedi(GOTO,0,0,2);
                isjmain = TRUE;
            }

            curscope++;
            fvotopro();
        }
    }




}


void ismain()
{
    int tableid,funid,tmpbegin,tmpend;
    char name[10] = {"main"};
    name[5] = '\0';

    curscope++;

    if(symbol!=MAINSYM)
    {
       error(14);
	   while(symbol!=MAINSYM&&isFileEnd==FALSE)
		{
				getsym();
				if(isFileEnd==TRUE)
					return;
		}

    }

    if(findtable(name,3)>=0)
        error(49);
    strcpy(table[tableindex].name,name);
    malloctb(tableindex);
    funid = medsindex-1;
    curcompifunid = funid;
    genmedi(LAB,funid,0,0);
    genmedi(BEGIN,funid,0,0);

    tmpoffset=0;

    table[tableindex].obj = 3;
    table[tableindex].type = 3;
    table[tableindex].scope = curscope;

    tableid = tableindex;
    tmpbegin = tempvindex;
    table[tableindex].tmpbasic = tmpbegin;

    tableindex++;

	 getsym();
	 if(isFileEnd==TRUE)
		return;
     if(symbol!=LPAREN)
     {
			error(7);
			while(symbol!=LPAREN&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
					return;
			}

     }

     getsym();
	 if(isFileEnd==TRUE)
		return;
     if(symbol!=RPAREN)
     {
				error(9);
				while(symbol!=RPAREN&&isFileEnd==FALSE)
				{
					getsym();
					if(isFileEnd==TRUE)
						return;
				}

     }
     getsym();
	 if(isFileEnd==TRUE)
		return;
     if(symbol!=LBRACE)
     {
                    error(10);
					while(symbol!=LBRACE&&isFileEnd==FALSE)
					{
						getsym();
						if(isFileEnd==TRUE)
							return;
					}
      }

      getsym();
	  if(isFileEnd==TRUE)
		return;
      mixsta();
      if(symbol!=RBRACE)
      {
            error(11);
            while(symbol!=RBRACE&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
            }

      }


      //void no return statement
        if(returnstack.scope!=curscope||returnstack.id!=funid)
        {

            genmedi(END,funid,0,0);
            genmedi(JR,0,0,0);

        }



      table[tableid].offset = tmpoffset;
      tmpend = tempvindex;
      table[tableid].length = tmpoffset+tmpend-tmpbegin;
      //table[tableid].length = medsindex - funid;

     // printf("This is main function.\n");
      return;




}

//注意声明的顺序
void program()
{
    getsym();
	if(isFileEnd==TRUE)
    {
        error(14);
        return;
    }

    labels[labelindex].name[0] = '$';
    labels[labelindex].name[1]='e';
    labels[labelindex].name[2]='n';
    labels[labelindex].name[3]='d';
    labels[labelindex].name[4]='\0';


    table[tableindex].name[0] = '\0';
    table[tableindex].obj = 1;
    table[tableindex].type = 1;
    table[tableindex].valuei = 0;
    table[tableindex].scope = curscope;
    malloctb(tableindex);
    tableindex++;

    /*
    tempv[tempvindex].type = 1;
    tempv[tempvindex].valuei = 0;
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = tempvindex;
    tempvindex=1;
    medsindex=1;
    */

    table[tableindex].name[0] = '\0';
    table[tableindex].obj = 6;
    table[tableindex].scope = curscope;
    malloctb(tableindex);
    tableindex++;
    medsindex = 2;

    malloclab(labelindex);
    labelindex=1;
    medsindex = 3;


    if(symbol==CONSTSYM)
    {
        constopro();
    }
    if(symbol==INTSYM||symbol==CHARSYM)
    {

        tmpstack[stackindex] = symbol;


        getsym();
		if(isFileEnd==TRUE)
		{
            error(14);
            return;
        }
        if(symbol!=IDEN)
        {
			error(1);
			while(symbol!=IDEN&&isFileEnd==FALSE)
			{
				getsym();
				if(isFileEnd==TRUE)
				{
                    error(14);
                    return;
                }
			}

        }

		if(findtable(id,2)>=0)
				error(49);

        strcpy(table[tableindex].name,id);
        table[tableindex].offset = offset++;

        //flag
        table[tableindex].obj = 0;
        table[tableindex].type = 0;


        getsym();
		if(isFileEnd==TRUE)
		{
            error(14);
            return;
        }
        if(symbol==COMMA||symbol==SEMICOLON||symbol==LBKET)
        {
            if(findtable(table[tableindex].name,2)>=0)
				error(49);
            vartopro();
        }

        if(symbol==LPAREN)
        {
            if(isjmain==FALSE)
            {
                genmedi(GOTO,0,0,2);
                isjmain = TRUE;
            }

            tmpoffset = 0;
            curscope++;
            if(findtable(table[tableindex].name,3)>=0)
				error(49);
            fictopro();
        }
    }

    if(symbol==VOIDSYM)
    {

        getsym();
		if(isFileEnd==TRUE)
		{
            error(14);
            return;
        }
        if(symbol==IDEN)
        {
            if(isjmain==FALSE)
            {
                genmedi(GOTO,0,0,2);
                isjmain = TRUE;
            }
            tmpoffset = 0;
            curscope++;
            fvotopro();
        }

    }

    if(symbol==MAINSYM)
    {
        tmpoffset = 0;
        curscope++;
        ismain();
    }





}


int openoutput()
{

    file1 = (char*)malloc((sizeof(char))*MAXPATH);
    printf("please input the test file path\n");
    gets(file1);
    if(freopen(file1,"w",stdout)==NULL)
    {
        printf("file to open the output file\n");
        return 0;
    }
    else
    {
         printf("success to open the output file\n");
         return 1;
    }
}




void initreg()
{
    int i;
    for(i=0;i<21;i++)
    {
        registers[i].distribute=FALSE;
    }

}


//分配寄存器
int allocreg(int index,int funid)
{
    int i,tablefuncid,basic,addr,prev;
    int pretableid,curtableid,tmp,pretmpindex,curtmpindex;
    for(i=0;i<21;i++)
        if(registers[i].distribute==FALSE)
            break;

    if(i==21)
    {

        if(medstack[index].tbindex!=-1)
        {
            tmp = medstack[index].tbindex;
            if(table[tmp].obj==6)
                return 2;
        }


        regid++;
        regid %= 21;
        if(registers[regid].index==-1)
        {
            i = regid;
        }
        else
        {
             prev = registers[regid].index;


            //8号寄存器原来存的是局部变量或是全局变量
            if(medstack[prev].tbindex!=-1)
            {
                pretableid = medstack[prev].tbindex;
                //参数或是变量需要写回内存
                if(table[pretableid].obj==4)
                {
                    addr = table[pretableid].offset;
                    addr *= 4;
                    addr = -addr;
                    printf("sw $%d %d($fp)\n",(regid+5),addr);

                }
                else if(table[pretableid].obj==2)
                {
                    //局部变量
                    if(table[pretableid].scope!=0)
                    {
                        addr = table[pretableid].offset;
                        addr *= 4;
                        addr = -addr;
                        printf("sw $%d %d($fp)\n",(regid+5),addr);

                    }
                    //全局变量
                    else
                    {

                        printf("sw $%d %s($0)\n",(regid+5),table[pretableid].name);

                    }
                }
            }
            //8号寄存器原来存的是临时变量
            else if(medstack[prev].tpindex!=-1)
            {
                tablefuncid = medstack[funid].tbindex;
                basic = table[tablefuncid].tmpbasic;
                pretmpindex = medstack[prev].tpindex;
                addr = pretmpindex-basic+table[tablefuncid].offset;
                addr *= 4;
                addr = -addr;
                printf("sw $%d %d($fp)\n",(regid+5),addr);

            }

            i = regid;
        }


    }




        //是符号表中的东西
        if(medstack[index].tbindex!=-1)
        {
            curtableid = medstack[index].tbindex;

            if(table[curtableid].obj==4)
            {
                addr = table[curtableid].offset;
                addr *= 4;
                addr = -addr;
                printf("lw $%d %d($fp)\n",(i+5),addr);

            }
            else if(table[curtableid].obj==2)
            {
                //局部变量
                if(table[curtableid].scope!=0)
                {
                    addr = table[curtableid].offset;
                    addr *= 4;
                    addr = -addr;
                    printf("lw $%d %d($fp)\n",(i+5),addr);
                }
                //全局变量
                else
                {
                    printf("lw $%d %s($0)\n",(i+5),table[curtableid].name);

                }
            }


        }
        //即将要分配的变量是临时变量
        else if(medstack[index].tpindex!=-1)
        {
            tablefuncid = medstack[funid].tbindex;
            basic = table[tablefuncid].tmpbasic;
            curtmpindex = medstack[index].tpindex;
            addr = curtmpindex-basic+table[tablefuncid].offset;
            addr *= 4;
            addr = -addr;
            printf("lw $%d %d($fp)\n",(i+5),addr);
        }

        registers[i].distribute = TRUE;
        registers[i].index = index;
        return (i+5);





}



int allocregforint(int value,int funid)
{
    int i,tablefuncid,basic,addr,prev;
    int pretableid,curtableid,pretmpindex;
    for(i=0;i<21;i++)
        if(registers[i].distribute==FALSE)
            break;
    if(i==21)
    {
        regid++;
        regid %= 21;
        prev = registers[regid].index;


        //8号寄存器原来存的是局部变量或是全局变量
        if(medstack[prev].tbindex!=-1)
        {
            pretableid = medstack[prev].tbindex;
            //参数或是变量需要写回内存
            if(table[pretableid].obj==4)
            {
                addr = table[pretableid].offset;
                addr *= 4;
                addr = -addr;
                printf("sw $%d %d($fp)\n",(regid+5),addr);

            }
            else if(table[pretableid].obj==2)
            {
                //局部变量
                if(table[pretableid].scope!=0)
                {
                    addr = table[pretableid].offset;
                    addr *= 4;
                    addr = -addr;
                    printf("sw $%d %d($fp)\n",(regid+5),addr);

                }
                //全局变量
                else
                {

                    printf("sw $%d %s($0)\n",(regid+5),table[pretableid].name);

                }
            }
        }
        //8号寄存器原来存的是临时变量
        else if(medstack[prev].tpindex!=-1)
        {
            tablefuncid = medstack[funid].tbindex;
            basic = table[tablefuncid].tmpbasic;
            pretmpindex = medstack[prev].tpindex;
            addr = pretmpindex-basic+table[tablefuncid].offset;
            addr *= 4;
            addr = -addr;
            printf("sw $%d %d($fp)\n",(regid+5),addr);

        }


    i = regid;


    }
    registers[i].distribute = TRUE;
    registers[i].index = -1;



    printf("addi $%d $0 %d\n",(i+5),value);

    return (i+5);

}


int isalloc(int index)
{
    int i;

    for(i=0;i<21;i++)
        if(registers[i].index==index)
            return (i+5);

    if(medstack[index].tbindex!=-1)
    {
        i = medstack[index].tbindex;
        if(table[i].obj==6)
            return 2;
    }

    return -1;
}

void printsw()
{
    int i = 5,j = 152,k=0;
    printf("sw $ra 160($sp)\n");
    printf("sw $fp 156($sp)\n");

    for(k=0;k<21;k++,i++,j=j-4)
    {
        printf("sw $%d %d($sp)\n",i,j);
    }

}


void writebackmem(int funid)
{
    int i,tablefuncid,basic,addr,prev;
    int pretableid,curtableid,pretmpindex;
    for(i=0;i<21;i++)
    {
        if(registers[i].distribute==TRUE&&registers[i].index!=-1)
        {

            prev = registers[i].index;
            //8号寄存器原来存的是临时变量
            if(medstack[prev].tpindex!=-1)
            {
                tablefuncid = medstack[funid].id;
                basic = table[tablefuncid].tmpbasic;
                pretmpindex = medstack[prev].tpindex;
                addr = pretmpindex-basic+table[tablefuncid].offset;
                addr *= 4;
                addr = -addr;
                printf("sw $%d %d($fp)\n",(i+5),addr);

            }
            //8号寄存器原来存的是局部变量或是全局变量
            else if(medstack[prev].tbindex!=-1)
            {
                pretableid = medstack[prev].tbindex;
                //参数或是变量需要写回内存
                if(table[pretableid].obj==4)
                {
                    addr = table[pretableid].offset;
                    addr *= 4;
                    addr = -addr;
                    printf("sw $%d %d($fp)\n",(i+5),addr);

                }
                else if(table[pretableid].obj==2)
                {
                    //局部变量
                    if(table[pretableid].scope!=0)
                    {
                        addr = table[pretableid].offset;
                        addr *= 4;
                        addr = -addr;
                        printf("sw $%d %d($fp)\n",(i+5),addr);

                    }
                    //全局变量
                    else
                    {

                        printf("sw $%d %s($0)\n",(i+5),table[pretableid].name);

                    }
                }
            }



        }


    }


}

void printlw()
{
    int i = 5,j = 152,k=0;
    printf("lw $ra 160($sp)\n");
    printf("lw $fp 156($sp)\n");

    for(k=0;k<21;k++,i++,j=j-4)
    {
        printf("lw $%d %d($sp)\n",i,j);
    }
}

void genemips()
{
    int i,rs,rt,rd;
    //int rsmedid,rtmedid,rdmedid;
    int rstabid,rttabid,rdtabid;
    int rsreg,rtreg,rdreg,tmpreg;
    int rsv,rtv,rdv;
    int rsint,rtint,rdint;//是否为常量
    //int fend;//function end index
    int funid;//函数基地址index
    int funtabid;
    int j,k,t,m,n;
    int reserve = 40;//保留区的长度
    char tmp[10]={"str"};
    char no[5];


    //i = openoutput();

    //if(i==0)
       // return;

    printf(".data\n");

    i = 0;
    while(i<medialindex&&medial[i].op==GLOBEG)
    {
        rs = medial[i].rs;

        if(medstack[rs].tbindex!=-1)
        {
            rstabid = medstack[rs].tbindex;
            //全局变量
            if(table[rstabid].type==1||table[rstabid].type==2)
            {
                printf("%s: .word 0 \n",table[rstabid].name);
            }
            //数组
            else
            {
                printf("%s: .word 0:%d \n",table[rstabid].name,table[rstabid].length);
            }

        }
        i++;
    }

    for(j=0;j<strindex;j++)
    {
        printf("str%d: .asciiz \"%s\"\n",j,strings[j]);

    }


    printf(".text\n");
    printf("j $end\n");

    for(;i<medialindex;i++)
    {
        rs = medial[i].rs;
        rt = medial[i].rt;
        rd = medial[i].rd;





        switch(medial[i].op)
        {
            case ADD:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        j = rsv+rtv;

                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        printf("addi $%d $%d %d\n",rdreg,rtreg,rsv);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("addi $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("add $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;
            case SUB:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = rsv-rtv;


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        printf("subi $%d $%d %d\n",rdreg,rtreg,rsv);
                        printf("sub $%d $0 $%d\n",rdreg,rdreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("subi $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sub $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;
            case MUL:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        j = rsv*rtv;

                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        printf("mul $%d $%d %d\n",rdreg,rtreg,rsv);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("mul $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("mul $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;
            case DIV:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = rsv/rtv;


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = allocregforint(rsv,funid);

                        printf("div $%d $%d $%d\n",rdreg,rsreg,rtreg);


                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("div $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("div $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;
            case BIG:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = ((rsv>rtv)?1:0);


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = allocregforint(rsv,funid);

                        printf("sgt $%d $%d $%d\n",rdreg,rsreg,rtreg);


                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sgt $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sgt $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;


            case SMA:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = ((rsv<rtv)?1:0);


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = allocregforint(rsv,funid);

                        printf("slt $%d $%d $%d\n",rdreg,rsreg,rtreg);


                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("slti $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("slt $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;
            case EQU:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = ((rsv==rtv)?1:0);


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = allocregforint(rsv,funid);

                        printf("seq $%d $%d $%d\n",rdreg,rsreg,rtreg);


                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("seq $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("seq $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }


                break;
            case BEQU:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = ((rsv>=rtv)?1:0);


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = allocregforint(rsv,funid);

                        printf("sge $%d $%d $%d\n",rdreg,rsreg,rtreg);


                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sge $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sge $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                break;
            case SEQU:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        j = ((rsv<=rtv)?1:0);


                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = allocregforint(rsv,funid);

                        printf("sle $%d $%d $%d\n",rdreg,rsreg,rtreg);


                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sle $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sle $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }




                break;
            case NOT:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;


                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }


                if(rsint==TRUE)
                {

                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);



                        printf("subi $%d $0 %d\n",rdreg,rsv);


                }

                else
                {

                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sub $%d $0 $%d\n",rdreg,rsreg);

                }


                break;


            case NEQU:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }
                if(medstack[rt].tbindex!=-1)
                {
                    rttabid = medstack[rt].tbindex;
                    //rt为常数
                    if(table[rttabid].obj==1)
                    {
                        rtint = TRUE;
                        if(table[rttabid].type==1)
                            rtv = table[rttabid].valuei;
                        else
                            rtv = table[rttabid].valuec;
                    }
                }


                if(rsint==TRUE)
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        if(rsv==rtv)
                            printf("ori $%d $0 0\n",rdreg);
                        else
                            printf("ori $%d $0 1\n",rdreg);
                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        printf("sne $%d $%d %d\n",rdreg,rtreg,rsv);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sne $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);
                        rtreg = isalloc(rt);
                        if(rtreg==-1)
                            rtreg = allocreg(rt,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("sne $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }



                break;
            case ASN:

                k = isalloc(rd);
                if(k==-1)
                {
                    k = allocreg(rd,funid);
                }

                if(medstack[rs].tbindex!=-1)
                {
                    j = medstack[rs].tbindex;
                    if(table[j].obj==1)
                    {
                        if(table[j].type==1)
                            printf("addi $%d $0 %d\n",k,table[j].valuei);
                        else
                            printf("addi $%d $0 %d\n",k,table[j].valuec);
                    }
                    else
                    {
                        t = isalloc(rs);
                        if(t==-1)
                            t = allocreg(rs,funid);
                        printf("move $%d $%d\n",k,t);

                    }
                }

                else
                {
                        t = isalloc(rs);
                        if(t==-1)
                            t = allocreg(rs,funid);
                        printf("move $%d $%d\n",k,t);

                }


                break;
            case BEQ:

                k = isalloc(rs);
                if(k==-1)
                {
                    k = allocreg(rs,funid);
                }
                //跳转到函数
                if(medstack[rd].tbindex!=-1)
                {
                    funtabid = medstack[rd].tbindex;
                    printf("beq $%d $0 %s\n",k,table[funtabid].name);
                }
                //跳转到条件标签
                else
                {
                    t = medstack[rd].lableindex;
                    printf("beq $%d $0 %s\n",k,labels[t].name);
                }


                break;
            case GOTO:
                //跳转到函数
                if(medstack[rd].tbindex!=-1)
                {
                    funtabid = medstack[rd].tbindex;
                    printf("j %s\n",table[funtabid].name);
                }
                //跳转到条件标签
                else
                {
                    k = medstack[rd].lableindex;
                    printf("j %s\n",labels[k].name);
                    //writebackmem(funid);
                    //initreg();
                }

                break;
            case LAB:
                //设置函数标签
                if(medstack[rs].tbindex!=-1)
                {
                    funtabid = medstack[rs].tbindex;
                    printf("%s:\n",table[funtabid].name);
                }
                //设置条件跳转标签
                else
                {
                    k = medstack[rs].lableindex;
                    printf("%s:\n",labels[k].name);
                }


                break;

            case BEGIN:

                funid = rs;
                funtabid = medstack[funid].tbindex;
                j = table[funtabid].length+reserve;

                j*=4;

                printf("move $fp $sp\n");
                printf("subi $sp $sp %d\n",j);
                printsw();
                initreg();

                break;
            case END:

                funid = rs;
                funtabid = medstack[funid].tbindex;
                j = table[funtabid].length+reserve;

                j*=4;
                //write将寄存器里的值写回内存
                writebackmem(funid);

                printlw();

                printf("addi $sp $sp %d\n",j);

                break;
            case SW:


                rstabid = medstack[rs].tbindex;

                //全局变量数组
                if(table[rstabid].scope==0)
                {
                    rtreg = isalloc(rt);
                    if(rtreg==-1)
                        rtreg = allocreg(rt,funid);

                    rdreg = isalloc(rd);
                    if(rdreg==-1)
                        rdreg = allocreg(rd,funid);

                    tmpreg = allocregforint(0,funid);

                    printf("sll $%d $%d 2\n",tmpreg,rtreg);

                    printf("sw $%d %s($%d)\n",rdreg,table[rstabid].name,tmpreg);
                }

                //局部变量数组
                else
                {
                    rsv = (table[rstabid].offset)*4;

                    //rsreg = allocregforint(rsv,funid);

                    rtreg = isalloc(rt);
                    if(rtreg==-1)
                        rtreg = allocreg(rt,funid);

                    rdreg = isalloc(rd);
                    if(rdreg==-1)
                        rdreg = allocreg(rd,funid);

                    tmpreg = allocregforint(0,funid);

                    printf("sll $%d $%d 2\n",tmpreg,rtreg);
                    printf("add $%d $%d $fp\n",tmpreg,tmpreg);

                    printf("sw $%d %d($%d)\n",rdreg,rsv,tmpreg);

                }


                break;
            case LW:
                rstabid = medstack[rs].tbindex;

                //全局变量数组
                if(table[rstabid].scope==0)
                {
                    rtreg = isalloc(rt);
                    if(rtreg==-1)
                        rtreg = allocreg(rt,funid);

                    rdreg = isalloc(rd);
                    if(rdreg==-1)
                        rdreg = allocreg(rd,funid);

                    tmpreg = allocregforint(0,funid);

                    printf("sll $%d $%d 2\n",tmpreg,rtreg);
                    printf("lw $%d %s($%d)\n",rdreg,table[rstabid].name,tmpreg);
                }

                //局部变量数组
                else
                {
                    rsv = (table[rstabid].offset)*4;

                    //rsreg = allocregforint(rsv,funid);

                    rtreg = isalloc(rt);
                    if(rtreg==-1)
                        rtreg = allocreg(rt,funid);

                    rdreg = isalloc(rd);
                    if(rdreg==-1)
                        rdreg = allocreg(rd,funid);

                    tmpreg = allocregforint(0,funid);

                    printf("sll $%d $%d 2\n",tmpreg,rtreg);
                     printf("add $%d $%d $fp\n",tmpreg,tmpreg);

                    printf("lw $%d %d($%d)\n",rdreg,rsv,tmpreg);

                }



                break;


            case CALL:

                rstabid = medstack[rs].tbindex;


                printf("jal %s\n",table[rstabid].name);

                break;
            case FORM:

                while(i<medialindex&&medial[i].op==FORM)
                {
                    rsint = FALSE;
                    rs = medial[i].rs;
                    rt = medial[i].rt;

                    if(medstack[rs].tbindex!=-1)
                    {
                        rstabid = medstack[rs].tbindex;
                        //rs为常数
                        if(table[rstabid].obj==1)
                        {
                            rsint = TRUE;
                            if(table[rstabid].type==1)
                                rsv = table[rstabid].valuei;
                            else
                                rsv = table[rstabid].valuec;
                        }
                    }

                    if(rsint==TRUE)
                    {
                        rsreg = allocregforint(rsv,funid);
                    }
                    else
                    {
                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);
                    }


                    m = -(rt*4);

                    printf("sw $%d %d($sp)\n",rsreg,m);
                    i++;
                }
                i--;

                /*
                j = i;

                while(j<medialindex&&medial[j].op==FORM)
                    j++;
                t = j-i;

                if(t<4)
                {
                    for(k=0;i<j;i++,k++)
                    {
                        rs = medial[i].rs;
                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("move $a%d $%d\n",k,rsreg);
                    }

                }

                else
                {
                    for(k=0;k<4;i++,k++)
                    {
                        rs = medial[i].rs;
                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("move $a%d $%d\n",k,rsreg);
                    }
                    t-=4;

                    for(k=1;k<=t&&medial[i].op==FORM;k++,i++)
                    {
                        rs = medial[i].rs;
                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        m = k*4;

                        printf("sw $%d %d($sp)\n",rsreg,m);
                    }

                }

                i = j-1;
                */
                break;
            case RETURN:

                if(rs==0)
                    printf("move $v0 $0\n");

                else
                {
                    if(medstack[rs].tbindex!=-1)
                    {
                        rstabid = medstack[rs].tbindex;
                        //rs为常数
                        if(table[rstabid].obj==1)
                        {
                            rsint = TRUE;
                            if(table[rstabid].type==1)
                                printf("addi $v0 $0 %d\n",table[rstabid].valuei);
                            else
                                printf("addi $v0 $0 %d\n",table[rstabid].valuec);

                        }
                        else
                        {
                            rsreg = isalloc(rs);
                            if(rsreg==-1)
                                rsreg = allocreg(rs,funid);
                            printf("move $v0 $%d\n",rsreg);
                        }

                    }

                    else
                    {
                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);
                         printf("move $v0 $%d\n",rsreg);
                    }


                }

                break;
            case PARA:


                break;
            case ADDI:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }



                if(rsint==TRUE)
                {

                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        j = rsv+rt;

                        printf("addi $%d $0 %d\n",rdreg,j);


                }

                else
                {

                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("addi $%d $%d %d\n",rdreg,rsreg,rt);


                }


                break;


            case SUBI:
                rsint = FALSE;
                rtint = FALSE;
                rdint = FALSE;

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //rs为常数
                    if(table[rstabid].obj==1)
                    {
                        rsint = TRUE;
                        if(table[rstabid].type==1)
                            rsv = table[rstabid].valuei;
                        else
                            rsv = table[rstabid].valuec;

                    }

                }



                if(rsint==TRUE)
                {

                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        j = rsv-rt;

                        printf("addi $%d $0 %d\n",rdreg,j);


                }

                else
                {

                        rdreg = isalloc(rd);
                        if(rdreg==-1)
                            rdreg = allocreg(rd,funid);

                        rsreg = isalloc(rs);
                        if(rsreg==-1)
                            rsreg = allocreg(rs,funid);

                        printf("addi $%d $%d %d\n",rdreg,rsreg,rt);


                }

                break;
            case RD:

                rsreg = isalloc(rs);
                if(rsreg==-1)
                    rsreg = allocreg(rs,funid);

                //read
                printf("li $v0 5\n");
                printf("syscall\n");
                printf("move $%d $v0\n",rsreg);



                break;
            case WRT:

                //write string
                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    if(table[rstabid].obj!=5)
                    {
                         rsreg = isalloc(rs);
                         if(rsreg==-1)
                             rsreg = allocreg(rs,funid);
                        printf("li $v0 1\n");
                        printf("move $a0 $%d\n",rsreg);
                        printf("syscall\n");
                    }
                    else
                    {
                        j = table[rstabid].locate;
                        printf("li $v0 4\n");
                        printf("la $a0 str%d\n",j);
                        printf("syscall\n");
                    }
                }
                else
                {
                    rsreg = isalloc(rs);
                    if(rsreg==-1)
                        rsreg = allocreg(rs,funid);
                     printf("li $v0 1\n");
                    printf("move $a0 $%d\n",rsreg);
                    printf("syscall\n");
                }



                break;
            case JR:
                printf("jr $ra\n");
                break;


        }
    }


    printf("$end: jal main\n");

}


int main()
{

    int i;



    i = openfile();
    if(i==0)
        return 0;

    program();

    genemips();

    //printgetsym();

    /*
    if(freopen("output.txt","w",stdout)==NULL)
    {
        printf("file to open the output file\n");
        return 1;
    }
    */

    //printf("hello world\n");
    free(file);
    //free(file1);
    return 0;
}

