#ifndef _CRYPTO_H_
#define _CRYPTO_H_

void aes_block_encrypt(const unsigned char* key, const unsigned char* plain, 
		int blen, unsigned char* cipher);

void aes_block_decrypt(const unsigned char* key, const unsigned char* cipher, 
		int blen, unsigned char* plain);

void aes_cbc_block_encrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* text,
		int blen, int tlen, unsigned char* cipher);

void aes_cbc_block_decrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* cipher,
		int blen, int tlen, unsigned char* plain);

int pbkdf2_hmac_sha1(const char* secret, const char* salt, int iters, int keysize, unsigned char* derived);

#endif
