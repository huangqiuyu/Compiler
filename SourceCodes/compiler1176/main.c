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


#define REGGSUM    12
#define REGGBASIC   14

#define REGLWSUM    6
#define REGLWBASIC  8

#define REGARRAYSUM 3
#define REGARRAYBASIC   5

#define RESERVE 16



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
#define MAXBLOCKS   3000 //最大块数
#define MAXDAGNODES 1000    //dag图节点数最大值


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
#define DEL 23
#define	ADDI	24
#define	SUBI	25
#define	RD	26
#define	WRT	27
#define JR 28
#define	LOOPBEGIN	29
#define	LOOPEND	30







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
//int regid=0;


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


int calmyparasum = 0;//本函数定义时的参数个数

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
    int distru;
    int myparasum;//the sum of function parameter
    int callparasum;
    int paraid;
    int num;
    int beginid;
    int endid;

}table[MAXTAB];


struct{
    int id;
    int op;
    int left;
    int right;
}dagnodes[MAXDAGNODES];
int dagnodindex = 0;

struct{
    int medsid;
    int nodeid;
}dagtable[MAXDAGNODES];
int dagtabindex = 0;


//四元式
struct{
    int op; //option
    int rs;
    int rt;
    int rd;
    int dagnodeid;
}medial[MAXMEDI];
int medialindex=0;


//临时变量表
struct{
    int type;//1:int,2:char
    int num;
    int regid;
    int addr;
    int scope;
}tempv[MAXTEMP];
int tempvindex=0;


//总表
struct{
    int id;
    int tbindex;
    int tpindex;
    int lableindex;
    int num;//被引用次数
}medstack[MAXMSTACK];
int medsindex=0;



struct{
    int id;
    int num;
}statis[MAXMSTACK];


//全局寄存器
struct{
    int distribute;//是否已分配
    int index;
}globalregs[REGGSUM];

//用于访问内存寻址
struct{
    int distribute;
    int index;
}lwregs[REGLWSUM];


//用于数组的操作
struct{
    int distribute;
    int index;
}arrayregs[REGARRAYSUM];


//标签，主要用于if-else，for等，函数不需要
struct{
    char name[10];
    int index;
}labels[MAXLABELS];

int labelindex=0;



struct{
    int beginid;//指向四元式
    int endid;//指向四元式
    int scope;//当前作用的函数
}blocks[MAXBLOCKS];
int blocksindex = 0;


//标签栈，保存已经生成但未使用的标签，生成时进栈，使用后退栈
//int labelstack[MAXLABELS];
//int labstaindex = 0;



int tableindex=0;//index of table

char strings[MAXSTRLENG][MAXSTR];
int strindex=0;

//int isjmain = TRUE;




//function declare

int findtable(char name[MAXIDS],int obj);
int ftableindex(char name[MAXIDS],int type);
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

//优化相关
void genblocks();
int finddagtable(int index);
void finddagnode1(int op,int left,int right,int result,int medindex);
void finddagnode2(int op,int left,int right,int result,int medindex);
void finddagnode3(int op,int left,int result,int medindex);
void dag();
void alloc();
int openoutput();
void printsw();
void printlw();
void checkpara(int funtabid);
int calreg(int num,int medid);
void swreg(int medid,int regid);


void genemips();








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
        case 7:
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
//type为3，函数；1：变量或常量或参数（全局局部）；2：数组
int ftableindex(char name[MAXIDS],int type)
{
    int i;
    if(tableindex==0)
        return -1;


    switch(type)
    {
        case 1:
            for(i=tableindex-1;i>=0&&table[i].scope==curscope;i--)
            {
                if(table[i].obj==1||table[i].obj==2||table[i].obj==4)
                {
                    if(strcmp(name,table[i].name)==0)
                        return i;
                }

            }

            for(i=0;i<=MAXTAB&&table[i].scope==0;i++)
            {
                if(table[i].obj==1||table[i].obj==2)
                {
                    if(strcmp(name,table[i].name)==0)
                        return i;
                }
            }


            break;

        case 2:
            for(i=tableindex-1;i>=0&&table[i].scope==curscope;i--)
            {
                if(table[i].obj==7)
                {
                    if(strcmp(name,table[i].name)==0)
                        return i;
                }

            }

            for(i=0;i<=MAXTAB&&table[i].scope==0;i++)
            {
                if(table[i].obj==7)
                {
                    if(strcmp(name,table[i].name)==0)
                        return i;
                }
            }
            break;

        case 3:
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
            break;
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
    medstack[medsindex].num = 0;
    table[index].index = medsindex;

    medsindex++;
}



void mallocint()
{
    tempv[tempvindex].type = 1;
    tempv[tempvindex].num = 0;
    tempv[tempvindex].regid = -1;
    tempv[tempvindex].addr = -1;
    tempv[tempvindex].scope = curscope;
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = tempvindex;
    medstack[medsindex].lableindex = -1;
    medstack[medsindex].num = 0;
    tempvindex++;
    medsindex++;

}

void mallocchar()
{
    tempv[tempvindex].type = 2;
    tempv[tempvindex].num = 0;
    tempv[tempvindex].regid = -1;
    tempv[tempvindex].addr = -1;
    tempv[tempvindex].scope = curscope;
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = tempvindex;
    medstack[medsindex].lableindex = -1;
    medstack[medsindex].num = 0;
    tempvindex++;
    medsindex++;
}


void malloclab(int index)
{
    medstack[medsindex].id = medsindex;
    medstack[medsindex].tbindex = -1;
    medstack[medsindex].tpindex = -1;
    medstack[medsindex].lableindex = index;
    medstack[medsindex].num = 0;
    labels[index].index = medsindex;

    medsindex++;
}

void genmedi(int o,int rs,int rt,int rd)
{
    medial[medialindex].op = o;
    medial[medialindex].rs = rs;
    medial[medialindex].rt = rt;
    medial[medialindex].rd = rd;
    medial[medialindex].dagnodeid = -1;
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
            printf("ERROR!!!!!因子推出表达式时缺少右括号)\n");
            break;

        case 28:
            printf("ERROR!!!!!if后面少了左括号(\n");
            break;

        case 29:
            printf("ERROR!!!!!if后面少了右括号)\n");
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

        //case 54:
         //   printf("ERROR!!!!!读语句不能对参数赋值\n");
           // break;

        case 55:
            printf("ERROR!!!!!数组的标识符不对，不能为其他常量或函数名\n");
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

        case 60:
            printf("ERROR!!!!!函数调用时参数声明类型与调用时不符\n");
            break;

        case 61:
            printf("ERROR!!!!!赋值语句标识符声明的类型和赋值的类型不符\n");
            break;

        case 62:
            printf("ERROR!!!!!数组下标不能为字符，应该是数字\n");
            break;

        case 63:
            printf("ERROR!!!!!函数返回值定义的类型与实际返回的不一致\n");
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
            table[tableindex].length = 0;
            table[tableindex].distru = -1;
            table[tableindex].num = 0;

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
                        table[tableindex].length = 0;
                        table[tableindex].distru = -1;
                        table[tableindex].num = 0;
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
                table[tableindex].length = 0;
                table[tableindex].distru = -1;

                table[tableindex].num = 0;
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
                                    table[tableindex].length = 0;
                                    table[tableindex].distru = -1;

                                    table[tableindex].num = 0;
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
            table[tableindex].length = 0;
            table[tableindex].distru = -1;

            table[tableindex].num = 0;

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
                        table[tableindex].length = 0;
                        table[tableindex].distru = -1;
                        table[tableindex].num = 0;
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
                    table[tableindex].length = 0;
                    table[tableindex].distru = -1;
                    table[tableindex].num = 0;
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
                                    table[tableindex].length = 0;
                                    table[tableindex].distru = -1;
                                    table[tableindex].num = 0;
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


        malloctb(tableindex);
            //genmedi(SW,medsindex-1,0,0);


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

            //malloctb(tableindex);
            //genmedi(SW,medsindex-1,0,0);

            table[tableindex].offset = tmpoffset++;
            table[tableindex].scope = curscope;
            table[tableindex].length = 0;
            table[tableindex].distru = -1;
            table[tableindex].num = 0;

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
        table[tableindex].obj = 7;

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

		//malloctb(tableindex);
        //genmedi(SW,medsindex-1,0,0);

        //tableindex++;
        tmpoffset += num;

         if(symbol==COMMA)
        {
                malloctb(tableindex);
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

                    table[tableindex].offset = tmpoffset++;
                    table[tableindex].scope = curscope;

                    table[tableindex].length = 0;
                    table[tableindex].distru = -1;
                    table[tableindex].num = 0;

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

        malloctb(tableindex);



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
        if(symbol==INTSYM)
            table[tableindex].type = 1;
        else if(symbol==CHARSYM)
            table[tableindex].type = 2;

        table[tableindex].obj = 2;

        table[tableindex].offset = tmpoffset++;
        table[tableindex].scope = curscope;

        table[tableindex].length = 0;
        table[tableindex].distru = -1;
        table[tableindex].num = 0;



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
    char tmpid[MAXIDS];
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

        strcpy(tmpid,id);

        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol==LPAREN)
        {
            idenid = ftableindex(tmpid,3);
            if(idenid<0)
            {
                error(51);
                idenid = 0;
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
            if(symbol==LBKET)
                idenid = ftableindex(tmpid,2);
            else
                idenid = ftableindex(tmpid,1);
            if(idenid<0)
            {
                error(51);
                idenid = 0;
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
    int idenidtype;//idenidtablid,idenidtmpid,
    int tmp1tableid,tmp1tmpid,tmp1type;
    int tmp2type;//tmp2tableid,tmp2tmpid,
    int addorsub=0;

    if(symbol==DOSYM)
    {
        genlabel();
        malloclab(labelindex);
        dolabel = medsindex - 1;

        genmedi(LAB,dolabel,0,0);
        genmedi(LOOPBEGIN,0,0,0);

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

        genmedi(BEQ,beqs,1,dolabel);
        genmedi(LOOPEND,0,0,0);

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

        idenid = ftableindex(id,1);

        if(idenid<0)
        {
            error(51);
            idenid = 0;
            idenidtype = table[idenid].type;
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
            idenidtype = table[idenid].type;
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

        if(medstack[tmp1].tbindex!=-1)
        {
            tmp1tableid = medstack[tmp1].tbindex;
            tmp1type = table[tmp1tableid].type;
        }
        else if(medstack[tmp1].tpindex!=-1)
        {
            tmp1tmpid = medstack[tmp1].tpindex;
            tmp1type = tempv[tmp1tmpid].type;
        }

        if(idenidtype!=tmp1type)
        {
            error(61);

        }

        genmedi(ASN,tmp1,0,idenid);

        genlabel();
        malloclab(labelindex);
        forlabel1 = medsindex -1;

        genlabel();
        malloclab(labelindex);
        forlabel2 = medsindex -1;

        genmedi(LAB,forlabel1,0,0);
        genmedi(LOOPBEGIN,0,0,0);

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

            idenid = ftableindex(id,1);

            if(idenid<0)
            {
                error(51);
                idenid = 0;
                idenidtype = table[idenid].type;
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
                idenidtype = table[idenid].type;
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


                tmp2 = ftableindex(id,1);
                if(tmp2<0)
                {
                    error(51);
                    tmp2 = 0;
                    tmp2type = table[tmp2].type;
                }
                else
                {
                    if(table[idenid].obj==3)
                    {
                        error(53);
                    }
                    tmp2type = table[tmp2].type;
                    tmp2 = table[tmp2].index;

                }

                if(tmp2type!=idenidtype)
                {
                    error(61);
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
                genmedi(LOOPEND,0,0,0);

            }

        }

        genmedi(LAB,forlabel2,0,0);

       // printf("This is for statement.\n");

    }

}


void callsta(int idenid)
{

    int tmp1,i =0,callftableid,tmp1tableid,tmp1tmpid,tmp1type;

    if(symbol==LPAREN)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        if(symbol!=RPAREN)
        {
            tmp1 = expression();
            callftableid = medstack[idenid].tbindex;

            if(medstack[tmp1].tbindex!=-1)
            {
                tmp1tableid = medstack[tmp1].tbindex;
                tmp1type = table[tmp1tableid].type;//1:int 2:char
            }
            else if(medstack[tmp1].tpindex!=-1)
            {
                tmp1tmpid = medstack[tmp1].tpindex;
                tmp1type = tempv[tmp1tmpid].type;
            }

            if(table[callftableid].myparasum==0)
            {
                error(59);
            }
            if(table[callftableid+1].obj==4&&table[callftableid+1].paraid==0&&tmp1type!=table[callftableid+1].type)
            {
                error(60);
                if(medstack[tmp1].tbindex!=-1)
                {

                    table[tmp1tableid].type = table[callftableid+1].type;//1:int 2:char
                }
                else if(medstack[tmp1].tpindex!=-1)
                {
                    tempv[tmp1tmpid].type = table[callftableid+1].type;
                }
            }

            genmedi(FORM,tmp1,i++,0);

            while(symbol==COMMA&&isFileEnd==FALSE)
            {
                getsym();
                if(isFileEnd==TRUE)
                    return;
                tmp1 = expression();

                if(medstack[tmp1].tbindex!=-1)
                {
                    tmp1tableid = medstack[tmp1].tbindex;
                    tmp1type = table[tmp1tableid].type;//1:int 2:char
                }
                else if(medstack[tmp1].tpindex!=-1)
                {
                    tmp1tmpid = medstack[tmp1].tpindex;
                    tmp1type = tempv[tmp1tmpid].type;
                }

                //强制转换
                if(table[callftableid+1+i].obj==4&&table[callftableid+1+i].paraid==i&&tmp1type!=table[callftableid+1+i].type)
                {
                    error(60);
                    if(medstack[tmp1].tbindex!=-1)
                    {

                        table[tmp1tableid].type = table[callftableid+1+i].type;//1:int 2:char
                    }
                    else if(medstack[tmp1].tpindex!=-1)
                    {
                        tempv[tmp1tmpid].type = table[callftableid+1+i].type;
                    }
                }

                genmedi(FORM,tmp1,i++,0);
            }

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
    int tmp1tabid,tmp1tmpid,tmp1type;
    int tmp2tabid,tmp2tmpid,tmp2type;
    if(symbol==LBKET)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        tmp1 = expression();
        if(medstack[tmp1].tbindex!=-1)
        {
            tmp1tabid = medstack[tmp1].tbindex;
            tmp1type = table[tmp1tabid].type;
        }
        else if(medstack[tmp1].tpindex!=-1)
        {
            tmp1tmpid = medstack[tmp1].tpindex;
            tmp1type = tempv[tmp1tmpid].type;
        }

        if(tmp1type!=1)
        {
            error(62);
        }

        if(medstack[idenid].tbindex!=-1)
        {
            tmp1tabid = medstack[idenid].tbindex;
            tmp1type = table[tmp1tabid].type;
        }
        else if(medstack[idenid].tpindex!=-1)
        {
            tmp1tmpid = medstack[idenid].tpindex;
            tmp1type = tempv[tmp1tmpid].type;
        }


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

            if(medstack[tmp2].tbindex!=-1)
            {
                tmp2tabid = medstack[tmp2].tbindex;
                tmp2type = table[tmp2tabid].type;
            }
            else if(medstack[tmp2].tpindex!=-1)
            {
                tmp2tmpid = medstack[tmp2].tpindex;
                tmp2type = tempv[tmp2tmpid].type;
            }

            if(tmp1type!=tmp2type)
            {
                error(61);
            }

            genmedi(SW,idenid,tmp1,tmp2);

        }

    }

    else if(symbol==ASSIGN)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;

        if(medstack[idenid].tbindex!=-1)
        {
            tmp1tabid = medstack[idenid].tbindex;
            tmp1type = table[tmp1tabid].type;
        }
        else if(medstack[idenid].tpindex!=-1)
        {
            tmp1tmpid = medstack[idenid].tpindex;
            tmp1type = tempv[tmp1tmpid].type;
        }



        tmp2 = expression();

        if(medstack[tmp2].tbindex!=-1)
        {
                tmp2tabid = medstack[tmp2].tbindex;
                tmp2type = table[tmp2tabid].type;
        }
        else if(medstack[tmp2].tpindex!=-1)
        {
                tmp2tmpid = medstack[tmp2].tpindex;
                tmp2type = tempv[tmp2tmpid].type;
        }
        if(tmp1type!=tmp2type)
        {
                error(61);
        }


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

        tmp1 = ftableindex(id,1);
        if(tmp1<0)
        {
            error(51);
            tmp1 = 0;
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

                tmp1 = ftableindex(id,1);
                if(tmp1<0)
                {
                    error(51);
                    tmp1 = 0;

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
    int tmp1tabid,tmp1tmpid,tmp1type;
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

            if(medstack[tmp1].tbindex!=-1)
            {
                tmp1tabid = medstack[tmp1].tbindex;
                tmp1type = table[tmp1tabid].type;
            }
            else if(medstack[tmp1].tpindex!=-1)
            {
                tmp1tmpid = medstack[tmp1].tpindex;
                tmp1type = tempv[tmp1tmpid].type;
            }

            //void but have return values
            tmpfuntableid = medstack[curcompifunid].tbindex;
            if(table[tmpfuntableid].type==3)
            {
                error(58);
            }

            if(table[tmpfuntableid].type!=tmp1type)
            {
                error(63);
                //强制转换
                if(medstack[tmp1].tbindex!=-1)
                {
                    table[tmp1tabid].type = table[tmpfuntableid].type;
                }
                else if(medstack[tmp1].tpindex!=-1)
                {
                    tempv[tmp1tmpid].type = table[tmpfuntableid].type;
                }

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
    int tmp1tabid,tmp1tmpid,tmp1type;
    int tmp2tabid,tmp2tmpid,tmp2type;
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

            if(medstack[tmp1].tbindex!=-1)
            {
                tmp1tabid = medstack[tmp1].tbindex;
                tmp1type = table[tmp1tabid].type;
            }
            else if(medstack[tmp1].tpindex!=-1)
            {
                tmp1tmpid = medstack[tmp1].tpindex;
                tmp1type = tempv[tmp1tmpid].type;
            }

            if(medstack[tmp2].tbindex!=-1)
            {
                tmp2tabid = medstack[tmp2].tbindex;
                tmp2type = table[tmp2tabid].type;
            }
            else if(medstack[tmp2].tpindex!=-1)
            {
                tmp2tmpid = medstack[tmp2].tpindex;
                tmp2type = tempv[tmp2tmpid].type;
            }

            if(tmp1type==tmp2type&&tmp1type==2)//都是char
            {
                mallocchar();
                tmp3 = medsindex-1;
            }
            else
            {
                mallocint();
                tmp3 = medsindex-1;
            }


            if(flag==2)
            {

                genmedi(SUB,tmp2,tmp1,tmp3);
            }
            else
            {

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
    int tmp1tabid,tmp1tmpid,tmp1type;
    int tmp2tabid,tmp2tmpid,tmp2type;
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

            if(medstack[tmp1].tbindex!=-1)
            {
                tmp1tabid = medstack[tmp1].tbindex;
                tmp1type = table[tmp1tabid].type;
            }
            else if(medstack[tmp1].tpindex!=-1)
            {
                tmp1tmpid = medstack[tmp1].tpindex;
                tmp1type = tempv[tmp1tmpid].type;
            }

            if(medstack[tmp2].tbindex!=-1)
            {
                tmp2tabid = medstack[tmp2].tbindex;
                tmp2type = table[tmp2tabid].type;
            }
            else if(medstack[tmp2].tpindex!=-1)
            {
                tmp2tmpid = medstack[tmp2].tpindex;
                tmp2type = tempv[tmp2tmpid].type;
            }


            if(tmp1type==tmp2type&&tmp1type==2)//都是char
            {
                mallocchar();
                tmp3 = medsindex-1;
            }
            else
            {
                mallocint();
                tmp3 = medsindex-1;
            }



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
    int tmp1type;
    int tmp2tabid,tmp2tmpid,tmp2type;

    if(symbol==IDEN)
    {



         getsym();
         if(isFileEnd==TRUE)
			return 0;

         if(symbol==LPAREN)
         {

             tmp1 = ftableindex(id,3);
             if(tmp1<0)
            {
                error(51);
                tmp1 = 0;
                tmp1type = table[tmp1].type;
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

                tmp1type = table[tmp1].type;
                tmp1 = table[tmp1].index;

            }


            callsta(tmp1);
            if(tmp1type==1)
                mallocint();
            else if(tmp1type==2)
                mallocchar();

            genmedi(ASN,1,0,medsindex-1);

            return (medsindex-1);
         }

         else if(symbol==LBKET)
         {
             tmp1 = ftableindex(id,2);

             if(tmp1<0)
            {
                error(51);
                tmp1 = 0;
                tmp1type = table[tmp1].type;
            }
            else
            {
                if(table[tmp1].obj!=7)
                {
                    error(55);
                }

                tmp1type = table[tmp1].type;
                tmp1 = table[tmp1].index;

            }

             getsym();
             if(isFileEnd==TRUE)
                return 0;
             tmp2 = expression();
             if(medstack[tmp2].tbindex!=-1)
             {
                 tmp2tabid = medstack[tmp2].tbindex;
                 tmp2type = table[tmp2tabid].type;
             }
             else if(medstack[tmp2].tpindex!=-1)
             {
                 tmp2tmpid = medstack[tmp2].tpindex;
                 tmp2type = tempv[tmp2tmpid].type;
             }

             if(tmp2type==2)
                error(62);

             if(tmp1type==1)
             {
                 mallocint();
             }
             else if(tmp1type==2)
             {
                 mallocchar();
             }

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
             tmp1 = ftableindex(id,1);
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

        malloctb(tableindex);
        genmedi(GLOBEG,medsindex-1,0,0);

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

            table[tableindex].offset = offset++;
            table[tableindex].scope = curscope;
            table[tableindex].length = 0;
            table[tableindex].distru = -1;
            table[tableindex].num = 0;

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
        table[tableindex].obj = 7;

        offset--;



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
            malloctb(tableindex);
            genmedi(GLOBEG,medsindex-1,0,0);

            tableindex++;

            getsym();
            if(isFileEnd==TRUE)
                return;
            if(symbol==IDEN)
            {
				if(findtable(id,2)>=0)
					error(49);
                strcpy(table[tableindex].name,id);


                //flag
                table[tableindex].obj = 2;
                if(tmpstack[stackindex]==INTSYM)
                        table[tableindex].type = 1;
                else if(tmpstack[stackindex]==CHARSYM)
                        table[tableindex].type = 2;

                table[tableindex].offset = offset++;

                table[tableindex].scope = curscope;

                table[tableindex].length = 0;
                table[tableindex].distru = -1;
                table[tableindex].num = 0;

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

        malloctb(tableindex);
        genmedi(GLOBEG,medsindex-1,0,0);

            //table[tableindex].offset = offset++;

        tableindex++;


        stackindex--;

      //  printf("This is varaible declaration for program.\n");
        getsym();
		if(isFileEnd==TRUE)
			return;
        if(symbol==INTSYM||symbol==CHARSYM)
        {
            tmpstack[++stackindex] = symbol;
            if(symbol==INTSYM)
                table[tableindex].type = 1;
            else if(symbol==CHARSYM)
                table[tableindex].type = 2;

            table[tableindex].obj = 2;
            table[tableindex].offset = offset++;
            table[tableindex].scope = curscope;

            table[tableindex].length = 0;
            table[tableindex].distru = -1;
            table[tableindex].num = 0;

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


            //flag



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
    //int paraid;
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
        //paraid = medsindex-1;

        table[tableindex].paraid = calmyparasum++;

        //genmedi(PARA,paraid,0,0);


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

    table[tableindex].beginid = medsindex-1;


    calmyparasum=0;





    tmpbegin = tempvindex;
	//table[tableindex].tmpbasic = tmpbegin;
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

    table[tableid].endid = medsindex-1;


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

    table[tableindex].beginid = medsindex-1;

    tableid = tableindex;
    tmpbegin = tempvindex;
   // table[tableindex].tmpbasic = tmpbegin;

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
    table[tableid].endid = medsindex-1;

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


        fictopro();
    }
    else if(symbol==VOIDSYM)
    {
        getsym();
        if(isFileEnd==TRUE)
            return;
        if(symbol==IDEN)
        {

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

    table[tableindex].beginid = medsindex-1;

    tableid = tableindex;
    tmpbegin = tempvindex;
    //table[tableindex].tmpbasic = tmpbegin;


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
      table[tableid].endid = medsindex-1;
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
/*
    labels[labelindex].name[0] = '$';
    labels[labelindex].name[1]='e';
    labels[labelindex].name[2]='n';
    labels[labelindex].name[3]='d';
    labels[labelindex].name[4]='\0';
*/

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

        if(symbol==INTSYM)
            table[tableindex].type = 1;
        else if(symbol==CHARSYM)
            table[tableindex].type = 2;

        table[tableindex].obj = 2;

        table[tableindex].offset = offset++;
        table[tableindex].scope = curscope;

        table[tableindex].length = 0;
        table[tableindex].distru = -1;
        table[tableindex].num = 0;

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


        strcpy(table[tableindex].name,id);



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


void genblocks()
{
    int i;//j,k;
    int funid,funtabid,curfunscope;
    for(i=0;i<medialindex;i++)
    {
        if(medial[i].op==BEGIN)
        {
            funid = medial[i].rs;
            funtabid = medstack[funid].tbindex;
            curfunscope = table[funtabid].scope;

            blocks[blocksindex].beginid = i;
            blocks[blocksindex].scope = curfunscope;
            while(i<medialindex&&medial[i].op!=END)
            {
                if(medial[i].op==LAB)
                {
                    blocks[blocksindex].endid = i-1;
                    blocksindex++;
                    blocks[blocksindex].beginid = i;
                    blocks[blocksindex].scope = curfunscope;

                }
                else if(medial[i].op==BEQ||medial[i].op==GOTO
                        ||medial[i].op==CALL)
                {
                    blocks[blocksindex].endid = i;
                    blocksindex++;
                    blocks[blocksindex].beginid = i+1;
                    blocks[blocksindex].scope = curfunscope;

                }
                i++;
            }
            blocks[blocksindex].endid = i;
        }
    }



}


int finddagtable(int index)
{
    int i,j;
    for(i=0;i<dagtabindex;i++)
    {
        if(dagtable[i].medsid==index)
            break;
    }

    //未找到，则新建一个节点
    if(i==dagtabindex)
    {
        //建立节点
        dagnodes[dagnodindex].id = dagnodindex;
        dagnodes[dagnodindex].op = -1;
        dagnodes[dagnodindex].left = -1;
        dagnodes[dagnodindex].right = -1;


        //填节点表
        dagtable[dagtabindex].medsid = index;
        dagtable[dagtabindex].nodeid = dagnodindex;

        j = dagnodindex;

        dagnodindex++;
        dagtabindex++;

        return j;

    }

    return dagtable[i].nodeid;
}


//add,mul,equ,nequ
void finddagnode1(int op,int left,int right,int result,int medindex)
{
    int i,j;


    for(i=0;i<dagnodindex;i++)
    {
        if(dagnodes[i].op==op&&((dagnodes[i].left==left&&dagnodes[i].right==right)
               ||(dagnodes[i].left==right&&dagnodes[i].right==left)))
        {
                for(j=0;j<dagtabindex;j++)
                {
                    if(dagtable[j].medsid==result)
                    {
                        dagtable[j].nodeid = dagnodes[i].id;
                        medial[medindex].dagnodeid = dagnodes[i].id;
                        break;
                    }

                }
                if(j==dagtabindex)
                {
                    dagtable[dagtabindex].medsid = result;
                    dagtable[dagtabindex].nodeid = dagnodes[i].id;
                    medial[medindex].dagnodeid = dagnodes[i].id;
                    dagtabindex++;

                }
                break;



        }
    }
    if(i==dagnodindex)
    {
        dagnodes[dagnodindex].id = dagnodindex;
        dagnodes[dagnodindex].left = left;
        dagnodes[dagnodindex].right = right;
        dagnodes[dagnodindex].op = op;

        for(j = 0;j<dagtabindex;j++)
        {
            if(dagtable[j].medsid==result)
            {
                dagtable[j].nodeid = dagnodindex;
                medial[medindex].dagnodeid = dagnodindex;
                break;
            }
        }
        if(j==dagtabindex)
        {
            dagtable[dagtabindex].medsid = result;
            dagtable[dagtabindex].nodeid = dagnodindex;
            medial[medindex].dagnodeid = dagnodindex;
            dagtabindex++;
        }

        dagnodindex++;
    }

}

//sub,div,big,sma,bequ,sequ
void finddagnode2(int op,int left,int right,int result,int medindex)
{
    int i,j;


    for(i=0;i<dagnodindex;i++)
    {
        if(dagnodes[i].op==op&&dagnodes[i].left==left&&dagnodes[i].right==right)
        {
                for(j=0;j<dagtabindex;j++)
                {
                    if(dagtable[j].medsid==result)
                    {
                        dagtable[j].nodeid = dagnodes[i].id;
                        medial[medindex].dagnodeid = dagnodes[i].id;
                        break;
                    }

                }
                if(j==dagtabindex)
                {
                    dagtable[dagtabindex].medsid = result;
                    dagtable[dagtabindex].nodeid = dagnodes[i].id;
                    medial[medindex].dagnodeid = dagnodes[i].id;
                    dagtabindex++;

                }
                break;



        }
    }
    if(i==dagnodindex)
    {
        dagnodes[dagnodindex].id = dagnodindex;
        dagnodes[dagnodindex].left = left;
        dagnodes[dagnodindex].right = right;
        dagnodes[dagnodindex].op = op;

        for(j = 0;j<dagtabindex;j++)
        {
            if(dagtable[j].medsid==result)
            {
                dagtable[j].nodeid = dagnodindex;
                medial[medindex].dagnodeid = dagnodindex;
                break;
            }
        }
        if(j==dagtabindex)
        {
            dagtable[dagtabindex].medsid = result;
            dagtable[dagtabindex].nodeid = dagnodindex;
            medial[medindex].dagnodeid = dagnodindex;
            dagtabindex++;
        }

        dagnodindex++;
    }

}


//not,assign
void finddagnode3(int op,int left,int result,int medindex)
{
    int i,j;


    for(i=0;i<dagnodindex;i++)
    {
        if(dagnodes[i].op==op&&dagnodes[i].left==left)
        {
                for(j=0;j<dagtabindex;j++)
                {
                    if(dagtable[j].medsid==result)
                    {
                        dagtable[j].nodeid = dagnodes[i].id;
                        medial[medindex].dagnodeid = dagnodes[i].id;
                        break;
                    }

                }
                if(j==dagtabindex)
                {
                    dagtable[dagtabindex].medsid = result;
                    dagtable[dagtabindex].nodeid = dagnodes[i].id;
                    medial[medindex].dagnodeid = dagnodes[i].id;
                    dagtabindex++;

                }
                break;



        }
    }
    if(i==dagnodindex)
    {
        dagnodes[dagnodindex].id = dagnodindex;
        dagnodes[dagnodindex].left = left;
        dagnodes[dagnodindex].right = -1;
        dagnodes[dagnodindex].op = op;

        for(j = 0;j<dagtabindex;j++)
        {
            if(dagtable[j].medsid==result)
            {
                dagtable[j].nodeid = dagnodindex;
                medial[medindex].dagnodeid = dagnodindex;
                break;
            }
        }
        if(j==dagtabindex)
        {
            dagtable[dagtabindex].medsid = result;
            dagtable[dagtabindex].nodeid = dagnodindex;
            medial[medindex].dagnodeid = dagnodindex;
            dagtabindex++;
        }

        dagnodindex++;
    }

}






void dag()
{
    int i,j,k,t,blobegin,bloend;
    int leftnodid;
    int rightnodid;
    int rs,rt,rd;
    int fir,sec,temp;
    int firtmp,sectmp;
    for(i=0;i<=blocksindex;i++)
    {
        //initial
        dagnodindex = 0;
        dagtabindex = 0;


        blobegin = blocks[i].beginid;
        bloend = blocks[i].endid;

        //构建dag图
        for(j=blobegin;j<bloend;j++)
        {
            if(medial[j].op==ADD||medial[j].op==MUL
               ||medial[j].op==EQU||medial[j].op==NEQU)
               {
                   rs = medial[j].rs;
                   rt = medial[j].rt;
                   rd = medial[j].rd;
                   leftnodid = finddagtable(rs);
                   rightnodid = finddagtable(rt);
                   finddagnode1(medial[j].op,leftnodid,rightnodid,rd,j);

               }
            else if(medial[j].op==SUB||medial[j].op==DIV
               ||medial[j].op==BIG||medial[j].op==SMA
               ||medial[j].op==BEQU||medial[j].op==SEQU)
               {
                   rs = medial[j].rs;
                   rt = medial[j].rt;
                   rd = medial[j].rd;
                   leftnodid = finddagtable(rs);
                   rightnodid = finddagtable(rt);
                   finddagnode2(medial[j].op,leftnodid,rightnodid,rd,j);
               }
            else if(medial[j].op==NOT)
            {
                rs = medial[j].rs;
                rd = medial[j].rd;
                leftnodid = finddagtable(rs);
                finddagnode3(medial[j].op,leftnodid,rd,j);
            }
            else if(medial[j].op==ASN)
            {
                rs = medial[j].rs;
                rd = medial[j].rd;
                leftnodid = finddagtable(rs);
                finddagnode3(medial[j].op,leftnodid,rd,j);
            }

        }

        //消除公共子表达式
        for(k=0;k<dagnodindex;k++)
        {
            for(t=blobegin;t<bloend&&!(medial[t].dagnodeid==k&&medstack[medial[t].rd].tpindex!=-1);t++);

            if(t!=bloend)
            {
                fir = t;
                firtmp = medial[fir].rd;
                while(t<bloend)
                {
                    for(t=t+1;t<bloend&&!(medial[t].dagnodeid==k&&medstack[medial[t].rd].tpindex!=-1);t++);
                    if(t!=bloend)
                    {
                        sec = t;
                        sectmp = medial[sec].rd;

                        medial[t].op = DEL;
                        medial[t].dagnodeid = -1;
                        medial[t].rs = 0;
                        medial[t].rt = 0;
                        medial[t].rd = 0;
                        for(temp = t+1;temp<bloend;temp++)
                        {
                            if(medial[temp].op==ADD||medial[temp].op==SUB||medial[temp].op==MUL
                               ||medial[temp].op==DIV||medial[temp].op==BIG||medial[temp].op==SMA
                               ||medial[temp].op==EQU||medial[temp].op==BEQU||medial[temp].op==SEQU
                               ||medial[temp].op==NEQU)
                            {
                                if(medial[temp].rs==sectmp)
                                    medial[temp].rs = firtmp;
                                if(medial[temp].rt==sectmp)
                                    medial[temp].rt = firtmp;
                            }

                            else if(medial[temp].op==NOT||medial[temp].op==ASN||medial[temp].op==FORM
                                    ||medial[temp].op==RETURN||medial[temp].op==ADDI||medial[temp].op==SUBI)
                            {
                                if(medial[temp].rs==sectmp)
                                    medial[temp].rs = firtmp;
                            }
                        }
                    }
                }

            }

        }



    }
}


//分配的时候地址已经是4的倍数，已经留出了参数的位置
void alloc()
{
    int i,j,k,t;
    int rs,rt;//rd;
    int weight=1;//权值
    int beginid,endid,funtabid;//scope;
    int temp1,parasum;
    int addr;
    int medid,tabid,tmpid;

    for(i=0;i<medialindex;i++)
    {
        if(medial[i].op==BEGIN)
        {
            rs = medial[i].rs;
            funtabid = medstack[rs].tbindex;
            beginid = table[funtabid].beginid;
            endid = table[funtabid].endid;
            //scope = table[funtabid].scope;

            parasum = table[funtabid].myparasum;
            addr = parasum*4;

            i++;
            //引用计数
            while(i<medialindex&&medial[i].op!=END)
            {
                if(medial[i].op==ADD||medial[i].op==SUB
                   ||medial[i].op==MUL||medial[i].op==DIV
                   ||medial[i].op==BIG||medial[i].op==SMA
                   ||medial[i].op==EQU||medial[i].op==BEQU
                   ||medial[i].op==SEQU||medial[i].op==NEQU
                   ||medial[i].op==SW||medial[i].op==LW)
                {
                    rs = medial[i].rs;
                    rt = medial[i].rt;
                    medstack[rs].num+=weight;
                    medstack[rt].num+=weight;
                }

                else if(medial[i].op==NOT||medial[i].op==ASN
                   ||medial[i].op==FORM||medial[i].op==RETURN
                   ||medial[i].op==ADDI||medial[i].op==SUBI
                   ||medial[i].op==BEQ||medial[i].op==RD||medial[i].op==WRT)
                {
                    rs = medial[i].rs;
                    medstack[rs].num+=weight;
                }
                else if(medial[i].op==LOOPBEGIN)
                    weight*=10;
                else if(medial[i].op==LOOPEND)
                    weight/=10;

                i++;
            }

            //复制
            for(k = 0,j=beginid;j<=endid;j++,k++)
            {
                statis[k].id = medstack[j].id;
                statis[k].num = medstack[j].num;
            }

            //排序
            for(j=0;j<k;j++)
            {
                for(t=j+1;t<k;t++)
                {
                    if(statis[t].num>statis[j].num)
                    {
                        temp1 = statis[t].id;
                        statis[t].id = statis[j].id;
                        statis[j].id = temp1;
                        temp1 = statis[t].num;
                        statis[t].num = statis[j].num;
                        statis[j].num = temp1;
                    }
                }
            }


            //alloc reg and memory
            for(j=0,t=0;j<k;j++)
            {
                medid = statis[j].id;
                if(t<REGGSUM)
                {
                    if(medstack[medid].tbindex!=-1)
                    {
                        //全局变量和数组不用考虑
                        tabid = medstack[medid].tbindex;
                        //参数
                        if(table[tabid].obj==4)
                        {
                            table[tabid].offset = -1;
                            table[tabid].distru = t+REGGBASIC;
                            t++;

                        }
                        //局部变量
                        else if(table[tabid].obj==2&&table[tabid].scope!=0)
                        {
                            table[tabid].offset = -1;
                            table[tabid].distru = t+REGGBASIC;
                            t++;
                        }
                        //局部数组
                        else if(table[tabid].obj==7&&table[tabid].scope!=0)
                        {
                            table[tabid].offset = addr;
                            addr = addr+(table[tabid].length*4);
                        }
                    }

                    else if(medstack[medid].tpindex!=-1)
                    {
                        tmpid = medstack[medid].tpindex;
                        tempv[tmpid].regid = t+REGGBASIC;
                        tempv[tmpid].addr = -1;
                        t++;
                    }
                }

                //全局寄存器已经分配完毕
                else
                {
                    if(medstack[medid].tbindex!=-1)
                    {
                        //全局变量和数组和参数不用考虑
                        tabid = medstack[medid].tbindex;

                        //局部变量
                        if(table[tabid].obj==2&&table[tabid].scope!=0)
                        {
                            table[tabid].offset = addr;
                            addr += 4;
                        }
                        //局部数组
                        else if(table[tabid].obj==7&&table[tabid].scope!=0)
                        {
                            table[tabid].offset = addr;
                            addr = addr+(table[tabid].length*4);
                        }
                    }

                    else if(medstack[medid].tpindex!=-1)
                    {
                        tmpid = medstack[medid].tpindex;
                        tempv[tmpid].regid = -1;
                        tempv[tmpid].addr = addr;
                        addr += 4;
                    }
                }


            }


        }
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






void printsw()
{
    int i = REGGBASIC,j = RESERVE*4,k=0;
    printf("sw $ra %d($sp)\n",j);
    j-=4;
    printf("sw $fp %d($sp)\n",j);

    for(k=0,j=j-4;k<REGGSUM;k++,i++,j=j-4)
    {
        printf("sw $%d %d($sp)\n",i,j);
    }

}


void printlw()
{
    int i = REGGBASIC,j = RESERVE*4,k=0;
    printf("lw $ra %d($sp)\n",j);
    j-=4;
    printf("lw $fp %d($sp)\n",j);

    for(k=0,j=j-4;k<REGGSUM;k++,i++,j=j-4)
    {
        printf("lw $%d %d($sp)\n",i,j);
    }


}

void checkpara(int funtabid)
{
    int parasum,addr,parareg;
    int i,j;
    parasum = table[funtabid].myparasum;

    for(i=0,j=funtabid;i<parasum&&j<tableindex;j++)
    {
        if(table[j].obj==4)
        {
            //参数已分配寄存器
            if(table[j].offset==-1&&table[j].distru>0)
            {
                addr = table[j].paraid;
                addr *= 4;
                addr = -addr;
                parareg = table[j].distru;
                printf("lw $%d %d($fp)\n",parareg,addr);
            }

            i++;
        }
    }
}

//num:1-rs,2-rt,3-rd
int calreg(int num,int medid)
{
    int tabid,tmpid,regid,addr;
    if(medstack[medid].tbindex!=-1)
    {
        tabid = medstack[medid].tbindex;
        //局部变量
        if(table[tabid].obj==2&&table[tabid].scope!=0)
        {
            //已分配全局寄存器
            if(table[tabid].offset==-1&&table[tabid].distru>0)
            {
                return table[tabid].distru;
            }
            else
            {
                if(num==1)
                {
                    regid = 8;
                    addr = 0-table[tabid].offset;

                    printf("lw $%d %d($fp)\n",regid,addr);

                    return regid;
                }

                else if(num==2)
                {
                    regid = 9;
                    addr = 0-table[tabid].offset;
                    printf("lw $%d %d($fp)\n",regid,addr);

                    return regid;
                }

                else if(num==3)
                    return 10;

            }
        }
        //全局变量
        else if(table[tabid].obj==2&&table[tabid].scope==0)
        {
            if(num==1)
            {
                regid = 8;
                printf("lw $%d %s($0)\n",regid,table[tabid].name);

                return regid;
            }

            else if(num==2)
            {
                regid = 9;
                printf("lw $%d %s($0)\n",regid,table[tabid].name);

                return regid;
            }
            else if(num==3)
                return 10;
        }
        //参数
        else if(table[tabid].obj==4)
        {
            //已分配全局寄存器
            if(table[tabid].offset==-1&&table[tabid].distru>0)
            {
                return table[tabid].distru;
            }
            else
            {
                if(num==1)
                {
                    regid = 8;
                    addr = 0-table[tabid].paraid*4;

                    printf("lw $%d %d($fp)\n",regid,addr);

                    return regid;
                }

                else if(num==2)
                {
                    regid = 9;
                    addr = 0-table[tabid].paraid*4;

                    printf("lw $%d %d($fp)\n",regid,addr);

                    return regid;
                }

                else if(num==3)
                    return 10;
            }
        }

        //函数返回值
        else if(table[tabid].obj==6)
        {
            if(num==1||num==2)
                return 2;
        }

    }
    else if(medstack[medid].tpindex!=-1)
    {
        tmpid = medstack[medid].tpindex;
        //分配到了全局寄存器
        if(tempv[tmpid].regid>0)
        {
            return tempv[tmpid].regid;
        }
        else
        {
            if(num==1)
            {
                regid = 8;
                addr = 0-tempv[tmpid].addr;
                printf("lw $%d %d($fp)\n",regid,addr);

                return regid;
            }

            else if(num==2)
            {
                regid = 9;
                addr = 0-tempv[tmpid].addr;
                printf("lw $%d %d($fp)\n",regid,addr);

                return regid;
            }
            else if(num==3)
                return 10;
        }
    }

    return 0;

}

void swreg(int medid,int regid)
{
    int tabid,tmpid,addr;
    if(medstack[medid].tbindex!=-1)
    {
        tabid = medstack[medid].tbindex;
        //局部变量
        if(table[tabid].obj==2&&table[tabid].scope!=0)
        {

            addr = 0-table[tabid].offset;
            printf("sw $%d %d($fp)\n",regid,addr);

        }
        //全局变量
        else if(table[tabid].obj==2&&table[tabid].scope==0)
        {

            printf("sw $%d %s($0)\n",regid,table[tabid].name);

        }
        //参数
        else if(table[tabid].obj==4)
        {
            addr = 0-table[tabid].paraid*4;

            printf("sw $%d %d($fp)\n",regid,addr);

        }
    }
    else if(medstack[medid].tpindex!=-1)
    {
        tmpid = medstack[medid].tpindex;

        addr = 0-tempv[tmpid].addr;
        printf("sw $%d %d($fp)\n",regid,addr);


    }


}


void genemips()
{
    int i,rs,rt,rd;
    int rstabid,rttabid;
    int rstmpid;
    int rsreg,rtreg,rdreg;
    //int rsaddr,rtaddr,rdaddr;

    int rsv,rtv;
    int rsint,rtint;//是否为常量
    //int rsdist,rtdist,rddist;//是否已分配全局寄存器

    //int fend;//function end index
    int funid;//函数基地址index
    int funtabid;
    int labid;
    int addr;
    int addrreg;
    int basicreg;

    int j;//k,t,m,n;
    //int reserve = 16;//保留区的长度
    //char tmp[10]={"str"};
    //char no[5];



    printf(".data\n");

    i = 0;
    while(i<medialindex&&medial[i].op==GLOBEG)
    {
        rs = medial[i].rs;

        if(medstack[rs].tbindex!=-1)
        {
            rstabid = medstack[rs].tbindex;
            //全局变量
            if(table[rstabid].type==2)
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
        printf("$str%d: .asciiz \"%s\"\n",j,strings[j]);

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
                        rdreg = calreg(3,rd);
                        j = rsv+rtv;
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $%d %d\n",rdreg,rtreg,rsv);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("add $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);


                break;
            case SUB:
                rsint = FALSE;
                rtint = FALSE;


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
                        rdreg = calreg(3,rd);
                        j = rsv-rtv;
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("sub $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("subi $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("sub $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);

                break;
            case MUL:
                rsint = FALSE;
                rtint = FALSE;


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
                        rdreg = calreg(3,rd);
                        j = rsv*rtv;
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("mul $%d $%d %d\n",rdreg,rtreg,rsv);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("mul $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("mul $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);

                break;
            case DIV:
                rsint = FALSE;
                rtint = FALSE;


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
                        rdreg = calreg(3,rd);
                        j = rsv/rtv;
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("div $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("div $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("div $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);

                break;
            case BIG:
                rsint = FALSE;
                rtint = FALSE;


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
                        rdreg = calreg(3,rd);
                        j = ((rsv>rtv)?1:0);
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("sgt $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("sgt $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("sgt $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);

                break;


            case SMA:
                rsint = FALSE;
                rtint = FALSE;



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
                        rdreg = calreg(3,rd);
                        j = ((rsv<rtv)?1:0);
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("slt $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("slti $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("slt $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);





                break;
            case EQU:
                rsint = FALSE;
                rtint = FALSE;



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
                        rdreg = calreg(3,rd);
                        j = ((rsv==rtv)?1:0);
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("seq $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("seq $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("seq $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);



                break;
            case BEQU:
                rsint = FALSE;
                rtint = FALSE;



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
                        rdreg = calreg(3,rd);
                        j = ((rsv>=rtv)?1:0);
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("sge $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("sge $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("sge $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);


                break;
            case SEQU:
                rsint = FALSE;
                rtint = FALSE;



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
                        rdreg = calreg(3,rd);
                        j = ((rsv<=rtv)?1:0);
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("sle $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("sle $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("sle $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);



                break;
            case NOT:
                rsint = FALSE;
                rtint = FALSE;



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
                    rdreg = calreg(3,rd);
                    printf("subi $%d $0 %d\n",rdreg,rsv);


                }

                else
                {

                    rsreg = calreg(1,rs);
                    rdreg = calreg(3,rd);
                    printf("sub $%d $0 $%d\n",rdreg,rsreg);



                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);


                break;


            case NEQU:
                rsint = FALSE;
                rtint = FALSE;


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
                        rdreg = calreg(3,rd);
                        j = ((rsv!=rtv)?1:0);
                        printf("addi $%d $0 %d\n",rdreg,j);

                    }
                    else
                    {
                        rsreg = 13;
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        printf("sne $%d $%d $%d\n",rdreg,rsreg,rtreg);

                    }
                }

                else
                {
                    if(rtint==TRUE)
                    {
                        rsreg = calreg(1,rs);
                        rdreg = calreg(3,rd);
                        printf("sne $%d $%d %d\n",rdreg,rsreg,rtv);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);
                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        printf("sne $%d $%d $%d\n",rdreg,rsreg,rtreg);
                    }
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);




                break;
            case ASN:
                rsint = FALSE;
                rtint = FALSE;


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
                    rdreg = calreg(3,rd);
                    printf("addi $%d $0 %d\n",rdreg,rsv);


                }

                else
                {

                    rsreg = calreg(1,rs);
                    rdreg = calreg(3,rd);
                    printf("move $%d $%d\n",rdreg,rsreg);



                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);





                break;
            case BEQ:

                rsint = FALSE;
                rtint = FALSE;


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
                    rsreg = 13;
                    printf("addi $%d $0 %d\n",rsreg,rsv);

                    if(medstack[rd].tbindex!=-1)
                    {
                        funtabid = medstack[rd].tbindex;

                        if(rt==0)
                            printf("beq $%d $0 %s\n",rsreg,table[funtabid].name);
                        else if(rt==1)
                            printf("bne $%d $0 %s\n",rsreg,table[funtabid].name);
                    }
                    else if(medstack[rd].lableindex!=-1)
                    {
                        labid = medstack[rd].lableindex;

                        if(rt==0)
                            printf("beq $%d $0 %s\n",rsreg,labels[labid].name);
                        else if(rt==1)
                            printf("bne $%d $0 %s\n",rsreg,labels[labid].name);

                    }


                }

                else
                {

                    rsreg = calreg(1,rs);
                    if(medstack[rd].tbindex!=-1)
                    {
                        funtabid = medstack[rd].tbindex;

                        if(rt==0)
                            printf("beq $%d $0 %s\n",rsreg,table[funtabid].name);
                        else if(rt==1)
                            printf("bne $%d $0 %s\n",rsreg,table[funtabid].name);


                    }
                    else if(medstack[rd].lableindex!=-1)
                    {
                        labid = medstack[rd].lableindex;

                        if(rt==0)
                            printf("beq $%d $0 %s\n",rsreg,labels[labid].name);
                        else if(rt==1)
                            printf("bne $%d $0 %s\n",rsreg,labels[labid].name);

                    }



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
                    labid = medstack[rd].lableindex;
                    printf("j %s\n",labels[labid].name);
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
                    labid = medstack[rs].lableindex;
                    printf("%s:\n",labels[labid].name);
                }


                break;

            case BEGIN:

                funid = rs;
                funtabid = medstack[funid].tbindex;
                j = table[funtabid].length+RESERVE;

                j*=4;


                printf("subi $sp $sp %d\n",j);
                printsw();
                printf("addi $fp $sp %d\n",j);
                checkpara(funtabid);

                break;
            case END:

                funid = rs;
                funtabid = medstack[funid].tbindex;
                j = table[funtabid].length+RESERVE;

                j*=4;


                printlw();

                printf("addi $sp $sp %d\n",j);

                break;
            case SW:

                rtint = FALSE;

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



                rstabid = medstack[rs].tbindex;


                //基地址不需要*4，偏移量需要，不用将寄存器写回内存
                //全局变量数组
                if(table[rstabid].scope==0)
                {
                    //偏移量为常数
                    if(rtint==TRUE)
                    {
                        addrreg = 6;
                        rdreg = calreg(1,rd);
                        printf("addi $%d $0 %d\n",addrreg,(rtv*4));
                        printf("sw $%d %s($%d)\n",rdreg,table[rstabid].name,addrreg);
                    }
                    else
                    {

                        rtreg = calreg(2,rt);
                        rdreg = calreg(1,rd);
                        addrreg = 6;
                        printf("sll $%d $%d 2\n",addrreg,rtreg);
                        printf("sw $%d %s($%d)\n",rdreg,table[rstabid].name,addrreg);
                    }

                }

                //局部变量数组
                else
                {
                    //计算数组基地址
                    basicreg = 5;
                    printf("addi $%d $0 %d\n",basicreg,(table[rstabid].offset));

                    //偏移量为常数
                    if(rtint==TRUE)
                    {
                        addrreg = 6;
                        rdreg = calreg(1,rd);
                        printf("addi $%d $%d %d\n",addrreg,basicreg,(rtv*4));
                        printf("sub $%d $fp $%d\n",addrreg,addrreg);
                        printf("sw $%d 0($%d)\n",rdreg,addrreg);
                    }
                    else
                    {

                        rtreg = calreg(2,rt);
                        rdreg = calreg(1,rd);
                        addrreg = 6;

                        printf("sll $%d $%d 2\n",addrreg,rtreg);
                        printf("add $%d $%d $%d\n",addrreg,basicreg,addrreg);
                        printf("sub $%d $fp $%d\n",addrreg,addrreg);
                        printf("sw $%d 0($%d)\n",rdreg,addrreg);

                    }



                }


                break;
            case LW:
                rtint = FALSE;

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



                rstabid = medstack[rs].tbindex;


                //基地址不需要*4，偏移量需要，需要将寄存器写回内存
                //全局变量数组
                if(table[rstabid].scope==0)
                {
                    //偏移量为常数
                    if(rtint==TRUE)
                    {
                        addrreg = 6;
                        rdreg = calreg(3,rd);
                        printf("addi $%d $0 %d\n",addrreg,(rtv*4));
                        printf("lw $%d %s($%d)\n",rdreg,table[rstabid].name,addrreg);
                    }
                    else
                    {

                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        addrreg = 6;
                        printf("sll $%d $%d 2\n",addrreg,rtreg);
                        printf("lw $%d %s($%d)\n",rdreg,table[rstabid].name,addrreg);
                    }

                }

                //局部变量数组
                else
                {
                    //计算数组基地址
                    basicreg = 5;
                    printf("addi $%d $0 %d\n",basicreg,(table[rstabid].offset));

                    //偏移量为常数
                    if(rtint==TRUE)
                    {
                        addrreg = 6;
                        rdreg = calreg(3,rd);
                        printf("addi $%d $%d %d\n",addrreg,basicreg,(rtv*4));
                        printf("sub $%d $fp $%d\n",addrreg,addrreg);
                        printf("lw $%d 0($%d)\n",rdreg,addrreg);
                    }
                    else
                    {

                        rtreg = calreg(2,rt);
                        rdreg = calreg(3,rd);
                        addrreg = 6;

                        printf("sll $%d $%d 2\n",addrreg,rtreg);
                        printf("add $%d $%d $%d\n",addrreg,basicreg,addrreg);
                        printf("sub $%d $fp $%d\n",addrreg,addrreg);
                        printf("lw $%d 0($%d)\n",rdreg,addrreg);

                    }



                }


                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);


                break;


            case CALL:

                rstabid = medstack[rs].tbindex;


                printf("jal %s\n",table[rstabid].name);

                break;
            case FORM:


                //不需要写回内存
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
                        rsreg = 13;
                        printf("addi $%d $0 %d\n",rsreg,rsv);

                        addr = 0-(rt*4);
                        printf("sw $%d %d($sp)\n",rsreg,addr);

                    }
                    else
                    {
                        rsreg = calreg(1,rs);

                        addr = 0-(rt*4);
                        printf("sw $%d %d($sp)\n",rsreg,addr);
                    }



                    i++;
                }

                i--;


                break;
            case RETURN:

                if(rs==0)
                    printf("move $v0 $0\n");

                //不需要写回内存
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
                            rsreg = calreg(1,rs);

                            printf("move $v0 $%d\n",rsreg);
                        }

                    }

                    else
                    {
                         rsreg = calreg(1,rs);
                         printf("move $v0 $%d\n",rsreg);
                    }


                }

                break;

            case ADDI:
                rsint = FALSE;
                rtint = FALSE;


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
                    rdreg = calreg(3,rd);
                    j = rsv+rt;
                    printf("addi $%d $0 %d\n",rdreg,j);

                }

                else
                {
                    rdreg = calreg(3,rd);
                    rsreg = calreg(1,rs);
                    printf("addi $%d $%d %d\n",rdreg,rsreg,rt);

                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);


                break;


            case SUBI:
                rsint = FALSE;
                rtint = FALSE;


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

                    rdreg = calreg(3,rd);
                    j = rsv-rt;
                    printf("addi $%d $0 %d\n",rdreg,j);


                }

                else
                {

                    rdreg = calreg(3,rd);
                    rsreg = calreg(1,rs);
                    printf("subi $%d $%d %d\n",rdreg,rsreg,rt);
                }

                //写回内存
                if(rdreg<14)
                    swreg(rd,rdreg);

                break;
            case RD:

                rsreg = calreg(3,rs);

                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //读int
                    if(table[rstabid].type==1)
                    {
                        printf("li $v0 5\n");
                        printf("syscall\n");
                        printf("move $%d $v0\n",rsreg);
                    }
                    //读char
                    else if(table[rstabid].type==2)
                    {
                        printf("li $v0 12\n");
                        printf("syscall\n");
                        printf("move $%d $v0\n",rsreg);
                    }
                }

                else if(medstack[rs].tpindex!=-1)
                {
                    rstmpid = medstack[rs].tpindex;
                    //读int
                    if(table[rstmpid].type==1)
                    {
                        printf("li $v0 5\n");
                        printf("syscall\n");
                        printf("move $%d $v0\n",rsreg);
                    }
                    //读char
                    else if(table[rstmpid].type==2)
                    {
                        printf("li $v0 12\n");
                        printf("syscall\n");
                        printf("move $%d $v0\n",rsreg);
                    }
                }

                //写回内存
                if(rsreg<14)
                    swreg(rs,rsreg);



                break;
            case WRT:

                //write string
                if(medstack[rs].tbindex!=-1)
                {
                    rstabid = medstack[rs].tbindex;
                    //写表达式的值
                    if(table[rstabid].obj!=5)
                    {
                        rsreg = calreg(1,rs);

                        //写int
                        if(table[rstabid].type==1)
                        {
                            printf("li $v0 1\n");
                            printf("move $a0 $%d\n",rsreg);
                            printf("syscall\n");

                        }
                        //写char
                        else if(table[rstabid].type==2)
                        {
                            printf("li $v0 11\n");
                            printf("move $a0 $%d\n",rsreg);
                            printf("syscall\n");
                        }


                    }
                    //写字符串
                    else
                    {
                        j = table[rstabid].locate;
                        printf("li $v0 4\n");
                        printf("la $a0 $str%d\n",j);
                        printf("syscall\n");
                    }
                }
                else if(medstack[rs].tpindex!=-1)
                {
                    rstmpid = medstack[rs].tpindex;
                    rsreg = calreg(1,rs);

                    //写int
                    if(tempv[rstmpid].type==1)
                    {
                        printf("li $v0 1\n");
                        printf("move $a0 $%d\n",rsreg);
                        printf("syscall\n");

                    }
                    //写char
                    else if(tempv[rstmpid].type==2)
                    {
                        printf("li $v0 11\n");
                        printf("move $a0 $%d\n",rsreg);
                        printf("syscall\n");
                    }

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





    genblocks();
    dag();
    alloc();

    freopen("C:\\Users\\forever\\Documents\\GitHub\\Compiler\\SourceCodes\\out.txt","w",stdout);

    //openoutput();


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
   // free(file1);
    free(file);
    return 0;
}



