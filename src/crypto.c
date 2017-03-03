#include "crypto.h"
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

void aes_block_encrypt(const unsigned char* key, const unsigned char* plain, 
		int blen, unsigned char* cipher){
	AES_KEY key_ctx;
	AES_set_encrypt_key(key, blen*8, &key_ctx);
	AES_encrypt(plain, cipher, &key_ctx);
}

void aes_block_decrypt(const unsigned char* key, const unsigned char* cipher, 
		int blen, unsigned char* plain){
	AES_KEY key_ctx;
	AES_set_decrypt_key(key, blen*8, &key_ctx);
	AES_decrypt(cipher, plain, &key_ctx);
}

void aes_cbc_block_decrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* cipher,
		int blen, int tlen, unsigned char* plain){
	unsigned char iv_cpy[blen];
	memcpy(iv_cpy, iv, blen);

	AES_KEY key_ctx;
	AES_set_decrypt_key(key, blen*8, &key_ctx);
	AES_cbc_encrypt(cipher, plain, tlen, &key_ctx, iv_cpy, AES_DECRYPT); 	
}
void aes_cbc_block_encrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* text,
		int blen, int tlen, unsigned char* cipher){
	unsigned char iv_cpy[blen];
	memcpy(iv_cpy, iv, blen);

	AES_KEY key_ctx;
	AES_set_encrypt_key(key, blen*8, &key_ctx);
	AES_cbc_encrypt(text, cipher, tlen, &key_ctx, iv_cpy, AES_ENCRYPT); 
}

int pbkdf2_hmac_sha1(const char* secret, const char* salt, int iters, int keysize, unsigned char* derived){
	return PKCS5_PBKDF2_HMAC_SHA1(secret, strlen(secret), (const unsigned char*)salt, strlen(salt), iters, keysize, derived);	
}
