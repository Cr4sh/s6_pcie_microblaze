#include "rc4.h"
//--------------------------------------------------------------------------------------
void arc4_swap(unsigned char *a, unsigned char *b)
{
    unsigned char c = *a;

    *a = *b;
    *b = c;
}
//--------------------------------------------------------------------------------------
void arc4_set_key(RC4_CTX *ctx, unsigned char *key, int key_len)
{
    int i = 0;
    unsigned char x = 0, y = 0;
    unsigned char *S = ctx->S;

    ctx->x = x = 0;
    ctx->y = y = 0;

    for (i = 0; i < 256; i++)
    {
        S[i] = (unsigned char)i;
    }

    for (i = 0; i < 256; i++)
    {
        y = (y + S[i] + key[x]) % 256;

        arc4_swap(&S[i], &S[y]);

        x = (x + 1) % key_len;
    }
}
//--------------------------------------------------------------------------------------
void arc4_crypt(RC4_CTX *ctx, unsigned char *byte)
{
    unsigned char x = ctx->x;
    unsigned char y = ctx->y;
    unsigned char *S = ctx->S;

    x = (x + 1) % 256;
    y = (y + S[x]) % 256;

    arc4_swap(&S[x], &S[y]);

    *byte ^= S[(S[x] + S[y]) % 256];

    ctx->x = x;
    ctx->y = y;
}
//--------------------------------------------------------------------------------------
void arc4_crypt(RC4_CTX *ctx, unsigned char *data, int data_len)
{
    int i = 0;

    for (i = 0; i < data_len; i++)
    {
        arc4_crypt(ctx, &data[i]);
    }
}
//--------------------------------------------------------------------------------------
// EoF 
