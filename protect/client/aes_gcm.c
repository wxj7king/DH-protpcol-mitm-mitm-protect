#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#define MAX 500 
extern unsigned char gcm_key[MAX];

extern const unsigned char gcm_iv[];

extern unsigned char gcm_pt[MAX];

extern const unsigned char gcm_aad[];

extern unsigned char gcm_ct[MAX];

extern unsigned char gcm_tag[MAX];

void aes_gcm_encrypt(void)
{
    EVP_CIPHER_CTX *ctx;
    int outlen, tmplen,ptlen;
    unsigned char outbuf[1024]={'0'};
    printf("AES GCM Encrypt:\n");
    printf("Plaintext:\n");
    BIO_dump_fp(stdout, gcm_pt, (int)strlen(gcm_pt));
    ctx = EVP_CIPHER_CTX_new();
    /* Set cipher type and mode */
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    /* Set IV length if default 96 bits is not appropriate */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, 12, NULL);
    /* Initialise key and IV */
    EVP_EncryptInit_ex(ctx, NULL, NULL, gcm_key, gcm_iv);
    /* Zero or more calls to specify any AAD */
    EVP_EncryptUpdate(ctx, NULL, &outlen, gcm_aad, 16);
    /* Encrypt plaintext */

    ptlen=(int)strlen(gcm_pt);
    tmplen=0;
    while(tmplen<=ptlen-16){
        EVP_EncryptUpdate(ctx, gcm_ct+tmplen, &outlen, gcm_pt+tmplen, 16);	
        tmplen+=16;
    }
    EVP_EncryptUpdate(ctx, gcm_ct+tmplen, &outlen, gcm_pt+tmplen, ptlen-tmplen);	
    //strncpy(gcm_ct,outbuf,(int)strlen(outbuf));
    /* Output encrypted block */
    //printf("Ciphertext(out):\n");
    //BIO_dump_fp(stdout, outbuf, (int)strlen(outbuf));
    printf("Ciphertext(gcm_ct):\n");
    BIO_dump_fp(stdout, gcm_ct, (int)strlen(gcm_ct));
    /* Finalise: note get no output for GCM */
    EVP_EncryptFinal_ex(ctx, outbuf, &outlen);
    /* Get tag */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_GET_TAG, 16, gcm_tag);
    //strncpy(gcm_tag,outbuf,16);
    /* Output tag */   
    //printf("Tag(out):\n");
    //BIO_dump_fp(stdout, outbuf, 16);
    printf("Tag(gcm_tag):\n");
    BIO_dump_fp(stdout, gcm_tag, (int)strlen(gcm_tag));
    EVP_CIPHER_CTX_free(ctx);
}


int aes_gcm_decrypt(void)
{
    EVP_CIPHER_CTX *ctx;
    int outlen, tmplen, rv,ctlen;
    unsigned char outbuf[1024];
    printf("AES GCM Derypt:\n");
    printf("Ciphertext:\n");
    BIO_dump_fp(stdout, gcm_ct, (int)strlen(gcm_ct));
    ctx = EVP_CIPHER_CTX_new();
    /* Select cipher */
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    /* Set IV length, omit for 96 bits */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, (int)strlen(gcm_iv), NULL);
    /* Specify key and IV */
    EVP_DecryptInit_ex(ctx, NULL, NULL, gcm_key, gcm_iv);
    /* Zero or more calls to specify any AAD */
    EVP_DecryptUpdate(ctx, NULL, &outlen, gcm_aad, (int)strlen(gcm_aad));
    /* Decrypt plaintext */

    ctlen=(int)strlen(gcm_ct);
    tmplen=0;	
    while(tmplen<=ctlen-16){
        EVP_DecryptUpdate(ctx, outbuf+tmplen, &outlen, gcm_ct+tmplen, 16);
        tmplen+=16;	
    }
    EVP_DecryptUpdate(ctx, outbuf+tmplen, &outlen, gcm_ct+tmplen, ctlen-tmplen);
    /* Output decrypted block */
    printf("Plaintext:\n");
    BIO_dump_fp(stdout, outbuf, (int)strlen(outbuf));
    /* Set expected tag value. */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG, (int)strlen(gcm_tag), 
                        (void *)gcm_tag);
    /* Finalise: note get no output for GCM */
    rv = EVP_DecryptFinal_ex(ctx, outbuf, &outlen);
    /*
     * Print out return value. If this is not successful authentication
     * failed and plaintext is not trustworthy.
     */
    printf("Tag Verify %s\n", rv > 0 ? "Successful!" : "Failed!");
    EVP_CIPHER_CTX_free(ctx);
	if(rv>0) return 1;
	else return 0;
}
