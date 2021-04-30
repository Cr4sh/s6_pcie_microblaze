
typedef struct _RC4_CTX 
{
    unsigned char S[256];
    unsigned char x, y;

} RC4_CTX;


void arc4_set_key(RC4_CTX *ctx, unsigned char *in_key, int key_len);

void arc4_crypt(RC4_CTX *ctx, unsigned char *byte);
void arc4_crypt(RC4_CTX *ctx, unsigned char *data, int data_len);
