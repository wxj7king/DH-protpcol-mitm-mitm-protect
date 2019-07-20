#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include<errno.h> 
#include"aes_gcm.h"
#include"AES_DH.h"
#include <ctype.h>
#include <openssl/aes.h>
#define MAX 500 //运算中的最高可能位数
#define NUMP 2 //已提供的素数个数
#define NUMG 32 //随机数g的位数
#define  MAXSIZE 500
#define SER_PORT 8080
/*
void aes_enc(char* source,unsigned char* des);
void aes_dec(unsigned char* source,char* des);
*/
unsigned char shared_key[]="60078158909043594823482134059039";
unsigned char aes_iv[]={0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84,0x69,0x12,0x35,0x92};

int connectToSer(char *ip){
	struct sockaddr_in servaddr;
    char buf[MAX];
    int sockfd,n;
    char *str;
	const char* IP=ip;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
	
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    //inet_pton(AF_INET,"192.168.146.163",&servaddr.sin_addr);
	inet_pton(AF_INET,IP,&servaddr.sin_addr);
    servaddr.sin_port = htons(SER_PORT);

    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){            	
		printf("connet error:%s\n",strerror(errno));
    }   
    char sa[MAX],sb[MAX],p[MAX],g[MAX],ak[MAX];
	char sa2[MAX]={'0'};

    int a;
    srand((int)time(0));
	a=rand()%10000+1;
    memset(p,0,MAXSIZE);
    memset(g,0,MAXSIZE);
    memset(sa,0,MAXSIZE);
	memset(aes_sa,0,MAXSIZE);
    sent(a,p,g,sa);
    int i;
    printf("生成密钥中.........\nrand:%d\n",a); 
    for(int i=0;i<3;i++)
    {    
        if(i==0)
        {write(sockfd,p,MAX);}   //把我们的输入，写到服务器
        if(i==1)
       	{write(sockfd,g,MAX);}
        if(i==2)
       	{write(sockfd,sa,MAX);}
    }
    memset(sb,0,MAXSIZE);
    n = read(sockfd,sb,MAX);    //从服务器读取数据
    get_key(a,sb,p,g,ak);
    printf("密钥为:"); 
    display(ak);
    memset(gcm_key,0,MAX);
    copy(ak,gcm_key);
    //display1(gcm_key);
	unsigned char temp[MAX]={'0'};
	int flag=0,flag2=0;
    while(1){
		memset(temp,0,MAX);
		memset(gcm_pt,0,MAX);
		memset(gcm_tag,0,MAX);
		memset(gcm_ct,0,MAX);		
		
		printf("input message(input q quit):\n");
    	//getchar();
    	fgets(gcm_pt,MAX,stdin);
		gcm_pt[(int)strlen(gcm_pt)-1]='\0';
		printf("pt:%s\n",gcm_pt);
		if(strcmp(gcm_pt,"q")==0){
			write(sockfd,"quit",(int)strlen("quit"));
			printf("quit!!\n");
			break;			
		}
   		
    	aes_gcm_encrypt();
		if(flag==0){
			write(sockfd,gcm_aad,MAX);//aad
			write(sockfd,gcm_iv,MAX);//iv   		
			flag=1;
		}
    	
		write(sockfd,gcm_tag,16);//tag
    	write(sockfd,gcm_ct,MAX);//ct
    	read(sockfd,temp,MAX);
    	printf("server:%s\n",temp);

    }

    close(sockfd);
}

/*
void aes_enc(char* source,unsigned char* des){
	EVP_CIPHER_CTX *ctx;
	int outlen;
	unsigned char content[MAX]={'0'};
	unsigned char outbuf[1024]={'0'};
	copy(source,content);	
	printf("Plaintext:\n");
    BIO_dump_fp(stdout, content, 32);

	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, shared_key, aes_iv);
	EVP_EncryptUpdate(ctx, des, &outlen, content, 32);
	
	printf("Ciphertext:\n");
    BIO_dump_fp(stdout, des, 32);

	EVP_EncryptFinal_ex(ctx, outbuf, &outlen);
	EVP_CIPHER_CTX_free(ctx);


}

void aes_dec(unsigned char* source,char* des){
	EVP_CIPHER_CTX *ctx;
	int outlen;
	unsigned char output[MAX]={'0'};
	unsigned char outbuf[1024]={'0'};
	
	printf("Ciphertext:\n");
    BIO_dump_fp(stdout, source, 32);
	
	ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, shared_key, aes_iv);
	EVP_DecryptUpdate(ctx, output, &outlen, source, 32);
	
	//strncpy(des,(char*)output,32);
	copy2(output,des);	
	printf("Plaintext:\n");
    BIO_dump_fp(stdout, des, 32);
	
	EVP_DecryptFinal_ex(ctx, outbuf, &outlen);
	EVP_CIPHER_CTX_free(ctx);

}
*/
int main(int argc,char *argv[]){  
    connectToSer(argv[1]);
    return 0;

}

