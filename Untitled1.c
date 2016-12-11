
int a[30];

int read()
{
	int i,n,j;
	scanf("%d",&n);
	for(i=0;i<n;i=i+1)
	{
		scanf("%d",&j);
		a[i] = j;
	}
	return (n);
}


void quick(int s,int t)
{
    int i,j,temp;
    int ilab;
    int jlab;
    if(s<t){
        i = s;
        j = t-1;
        do{
            do{
                ilab = 0;
                if(a[s]>=a[i])
                    if(i<t){
                        i = i + 1;
                        ilab = 1;
                    }
            }while(ilab==1);
			do{
			    jlab = 0;
                if(a[s]<=a[j])
                    if(j>s){
                        j = j - 1;
                        jlab = 1;
                    }
			}while(jlab==1);
            if(i<j){
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }while(i<j);
        temp = a[s];
        a[s] = a[j];
        a[j] = temp;
        quick(s,j);
        quick(j+1,t);
    }
}

void print(int n)
{
	int i;
	for(i=0;i<n;i=i+1)
		printf(" %d",a[i]);
}

void main(){
	int n;
	n=read();

	quick(0,n);

	print(n);
}
