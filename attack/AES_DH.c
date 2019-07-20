#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<ctype.h>
#define swap(x,y) x^=y^=x^=y
#define MAX 500 //运算中的最高可能位数
#define NUMP 2 //已提供的素数个数
#define NUMG 32 //随机数g的位数
#define  MAXSIZE 500
#define SHAREDNUM 8765

extern char p_set[NUMP][110];
extern const int S_BOX[16][16];
extern const int INVERSE_S_BOX[16][16];
extern int RC[10];
int get_n(char*a)
{
    int i=MAX-1;
    while(i>=0&&!a[i])i--;
    return i+1;
}
int get_n1(unsigned char*a)
{
    int i=MAX-1;
    while(i>=0&&!a[i])i--;
    return i+1;
}
int get_mul(char*a,char*b)
{
    int na,nb,i,j,n;
    char c[MAX],d[MAX];
    for(i=0; i<MAX; i++)c[i]=a[i],d[i]=b[i],a[i]=0;
    na=get_n(c);
    nb=get_n(d);
    for(i=0; i<nb; i++)for(j=0; j<na; j++)
        {
            a[i+j]+=d[i]*c[j];
            if(a[i+j]>9)a[i+1+j]+=a[i+j]/10,a[i+j]%=10;
        }
    for(i=0; i<MAX&&i<(na+nb); i++)
    {
        if(a[i])n=i+1;
        if(a[i]>9)a[i+1]+=a[i]/10,a[i]%=10;
    }
    return n;
}
int get_mod(char*a,char*b)
{
    int na,nb,i,u,f=0,n;
    na=get_n(a);
    nb=get_n(b);
    u=na-nb;
    if(u<0)return 0;
    while(u+1)
    {
        for(i=na-1,f=0; i>=u; i--)
        {
            if(a[i]>b[i-u])
            {
                f=1;
                break;
            }
            if(a[i]<b[i-u])
            {
                f=-1;
                break;
            }
        }
        if(!f)
        {
            for(i=na-1; i>=u; i--)a[i]=0;
            u-=nb;
            if(u<0)break;
            continue;
        }
        if(f==-1)u--;
        if(f==1)
        {
            for(i=u; i<na; i++)
            {
                a[i]-=b[i-u];
                if(a[i]<0)a[i+1]--,a[i]+=10;
            }
        }
    }
    for(i=0; i<na; i++)if(a[i])n=i+1;

    return n;
}
void reverse(char*a)
{
    int i,n;
    n=get_n(a);
    for(i=0; i<n/2; i++)swap(a[i],a[n-1-i]);
}
void reverse1(unsigned char*a)
{
    int i,n;
    n=get_n1(a);
    for(i=0; i<n/2; i++)swap(a[i],a[n-1-i]);
}
void get_a(char*a)
{
    int i=0;
    while(a[i])a[i++]-='0';
}
void get_g(char*a)
{
    int i,r,j=0;;
    srand(time(0));
    while(1)
    {
        r=rand()%10000;
        for(i=0; i<4; i++)
        {
            a[j++]=r%10;
            r/=10;
            if(j==NUMG)return;
        }
    }
}
void get_p(char*a,int n)
{
    int i;
    for(i=0; i<100; i++)a[i]=p_set[n][i];
}
void display(char*a)
{
    int n,i;
    n=get_n(a);
    reverse(a);
    for(i=0; i<n; i++)printf("%d",a[i]);
    printf("\n");
    reverse(a);
}
void num_enc(char*a)
{
    int n,i;
    n=get_n(a);
    reverse(a);
    for(i=0; i<n; i++)
	a[i]=a[i]+SHAREDNUM;
    //printf("\n");
    reverse(a);
}
void display1(unsigned char*a)
{
    int n,i;
    n=get_n1(a);
    reverse1(a);
    for(i=0; i<n; i++)printf("%d",a[i]);
    printf("\n");
    reverse1(a);
}
void sent(int a,char* p,char* g,char *ans)
{
    char t[MAX]= {0};
    int p_n=0;
    int i;
    memset(p,0,MAX*sizeof(p[0]));
    memset(g,0,MAX*sizeof(g[0]));
    srand(time(0));
    p_n=rand()%NUMP;   //随机得到p_n
    get_g(g);
    get_p(p,p_n);
    get_a(p);
    reverse(p);
    for(i=0; i<MAX; i++)t[i]=g[i],ans[i]=0;
    ans[0]=1;
    for(i=0; i<32; i++)
    {
        if(a&1<<i)
        {
            get_mul(ans,t);
            get_mod(ans,p);
        }
        get_mul(t,t);
        get_mod(t,p);
    }
}
void copy(char *a,unsigned char *b)//a to b
{
    int n,i;
    n=get_n(a);
    reverse(a);
    for(i=0; i<n; i++){b[i]=a[i];}
    reverse(a);
    reverse1(b);

}

void copy2(unsigned char *a,char *b){
	int n,i;	
	n=32;
	for(i=0; i<n; i++){b[i]=(char)a[i];}
    reverse(a);
    //reverse1(b);
}
void get_key(int b,char *rec,char*p,char*g,char *ans)
{
    char t[MAX]= {0};
    int i;

    for(i=0; i<MAX; i++)t[i]=rec[i],ans[i]=0;
    ans[0]=1;
    for(i=0; i<32; i++)
    {
        if(b&1<<i)
        {
            get_mul(ans,t);
            get_mod(ans,p);
        }
        get_mul(t,t);
        get_mod(t,p);
    }
} 

//aes

int c2i(char ch) {
    // Èç¹ûÊÇÊý×Ö£¬ÔòÓÃÊý×ÖµÄASCIIÂë¼õÈ¥48, Èç¹ûch = '2' ,Ôò '2' - 48 = 2
    if(isdigit(ch))
        return ch - 48;

    // Èç¹ûÊÇ×ÖÄ¸£¬µ«²»ÊÇA~F,a~fÔò·µ»Ø
    if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )
        return -1;

    // Èç¹ûÊÇ´óÐ´×ÖÄ¸£¬ÔòÓÃÊý×ÖµÄASCIIÂë¼õÈ¥55, Èç¹ûch = 'A' ,Ôò 'A' - 55 = 10
    // Èç¹ûÊÇÐ¡Ð´×ÖÄ¸£¬ÔòÓÃÊý×ÖµÄASCIIÂë¼õÈ¥87, Èç¹ûch = 'a' ,Ôò 'a' - 87 = 10
    if(isalpha(ch))
        return isupper(ch) ? ch - 55 : ch - 87;

    return -1;
}

void subBytes(int a[4][4], int encode){
    // encode Îª1 ´ú±í×Ö½ÚÌæ´ú£¬Îª0´ú±íÄæÏò×Ö½ÚÌæ´ú
	int i,j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            int temp = a[i][j];
            int row = temp / 16;
            int column = temp % 16;
            if (encode)
                a[i][j] = S_BOX[row][column];
            else
                a[i][j] = INVERSE_S_BOX[row][column];
        }
    }
}

void shiftRows(int a[4][4], int encode){
    //encode Îª1´ú±íÐÐÒÆÎ»£¬Îª0´ú±íÄæÏòÐÐÒÆÎ»
	int i,j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < i; ++j) {
            if (encode) {
                int temp = a[i][0];
                a[i][0] = a[i][1];
                a[i][1] = a[i][2];
                a[i][2] = a[i][3];
                a[i][3] = temp;
            } else{
                int temp = a[i][3];
                a[i][3] = a[i][2];
                a[i][2] = a[i][1];
                a[i][1] = a[i][0];
                a[i][0] = temp;
            }
        }
    }
}
//AES³Ë·¨¼ÆËã
int aes_multiple(int a, int le){
    int thr = le & 0x8;
    int sec = le & 0x4;
    int fir = le & 0x2;
    int fir_mod = le % 2;
    int result = 0;
	int i;
    if (thr){
        int b = a;
        for (i = 1; i <=3 ; ++i) {
            b = b<<1;
            if (b >= 256)
                b = b ^ 0x11b;
        }
        b = b % 256;
        result = result ^ b;
    }
    if (sec){
        int b = a;
        for (i = 1; i <=2 ; ++i) {
            b = b<<1;
            if (b >= 256)
                b = b ^ 0x11b;
        }
        b = b % 256;
        result = result ^ b;
    }
    if (fir){
        int b = a << 1;
        if (b >= 256)
            b = b ^ 0x11b;
        b = b % 256;
        result = result ^ b;
    }
    if (fir_mod)
        result = result ^ a;
    return  result;
}

void mixColumns(int a[4][4], int encode){
    //encode Îª1´ú±íÁÐ»ìÏý£¬Îª0´ú±íÄæÏòÁÐ»ìÏý
	int i;
    for (i = 0; i < 4; ++i) {
        int temp0 = a[0][i];
        int temp1 = a[1][i];
        int temp2 = a[2][i];
        int temp3 = a[3][i];
        if (encode) {
            a[0][i] = aes_multiple(temp0, 2) ^ aes_multiple(temp1, 3) ^ temp2 ^ temp3;
            a[1][i] = temp0 ^ (aes_multiple(temp1, 2)) ^ (temp2 ^ aes_multiple(temp2, 2)) ^ temp3;
            a[2][i] = temp0 ^ temp1 ^ (aes_multiple(temp2, 2)) ^ (temp3 ^ aes_multiple(temp3, 2));
            a[3][i] = temp0 ^ (aes_multiple(temp0, 2)) ^ temp1 ^ temp2 ^ aes_multiple(temp3, 2);
        }else{
            a[0][i] = aes_multiple(temp0, 14) ^ aes_multiple(temp1, 11) ^ aes_multiple(temp2, 13) ^ aes_multiple(temp3, 9);
            a[1][i] = aes_multiple(temp0, 9) ^ aes_multiple(temp1, 14) ^ aes_multiple(temp2, 11) ^ aes_multiple(temp3, 13);
            a[2][i] = aes_multiple(temp0, 13) ^ aes_multiple(temp1, 9) ^ aes_multiple(temp2, 14) ^ aes_multiple(temp3, 11);
            a[3][i] = aes_multiple(temp0, 11) ^ aes_multiple(temp1, 13) ^ aes_multiple(temp2, 9) ^ aes_multiple(temp3, 14);
        }
    }
}

void addRoundKey(int a[4][4], int k[4][4]){
    // ÓÉÓÚÓÃw[11][4][4]±íÊ¾W[44]µ¼ÖÂÐÐÁÐ×ªÖÃ£¬ËùÒÔÔÚ½øÐÐÒì»ò²Ù×÷µÄÊ±ºòÓ¦¸ÃÊÇa[i£¬j] Òì»ò k[j,i]
	int i,j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            a[i][j] = a[i][j] ^ k[j][i];
        }
    }
}

void keyExpansion(int key[4][4], int w[11][4][4]){
	int i,j,k,l;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            w[0][i][j] = key[j][i];
        }
    }
    for (i = 1; i < 11; ++i){
        for (j = 0; j < 4; ++j) {
            int temp[4];
            if (j == 0){
                temp[0] = w[i-1][3][1];
                temp[1] = w[i-1][3][2];
                temp[2] = w[i-1][3][3];
                temp[3] = w[i-1][3][0];
                for (k = 0; k < 4; ++k) {
                    int m = temp[k];
                    int row = m / 16;
                    int column = m % 16;
                    temp[k] = S_BOX[row][column];
                    if (k == 0){
                        temp[k] = temp[k] ^ RC[i-1];
                    }
                }
            } else{
                temp[0] = w[i][j-1][0];
                temp[1] = w[i][j-1][1];
                temp[2] = w[i][j-1][2];
                temp[3] = w[i][j-1][3];
            }
            for (l = 0; l < 4; ++l) {

                w[i][j][l] = w[i-1][j][l] ^ temp[l];
            }

        }
    }

}

void aes_detail(int content[4][4],  int password[4][4], int encode){
    int p[11][4][4];
    keyExpansion(password, p);
	int i;
    if (encode) {
        addRoundKey(content, p[0]);
        for (i = 1; i <= 10; ++i) {
            subBytes(content, encode);
            shiftRows(content, encode);
            if (i != 10) {
                mixColumns(content, encode);
            }

            addRoundKey(content, p[i]);
        }
    }else {
        addRoundKey(content, p[10]);
        for (i = 9; i >= 0; --i) {
            shiftRows(content, encode);
            subBytes(content, encode);
            addRoundKey(content, p[i]);
            if (i != 0) {
                mixColumns(content, encode);
            }
        }
    }
}
void aes_enc(char* source,unsigned char* des, char* password, int method){
    int p[4][4];
	int m,i,j,k;
    for (m = 0; m < 4; ++m) {
        for (i = 0; i < 4; ++i) {
            int indx = 4 * i + m;
            p[i][m] = 16 * c2i(password[indx]) + c2i(password[indx + 1]);
        }
    }


    unsigned char content[MAX]={'0'};
    copy(source,content);
	int size=32;
    //½«ÎÄ¼þ×ª»»³É16×Ö½ÚµÄintÐÍÊý×é¼ÓÃÜ¡¢½âÃÜ
    for (i = 0; i < size/16; ++i) {
        int content_to_int[4][4];
        for (j = 0; j < 4; ++j) {
            for (k = 0; k < 4; ++k) {
                content_to_int[j][k] = content[j * 4 + k + 16 * i];
                
            }
        }
        aes_detail(content_to_int, p, method);
        for (j = 0; j < 4; ++j) {
            for (k = 0; k < 4; ++k) {
                des[j * 4 + k + 16 * i] = content_to_int[j][k];
            	//printf("%c-%d\n",des[j * 4 + k + 16 * i],content_to_int[j][k]);
			}
        }
    }
}

