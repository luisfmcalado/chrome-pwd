
#include "../src/reader.h"
#include "../src/crypto.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef __APPLE__

void test_aes_block_enc_dec(const int blen){

	assert(blen > 0);

	int i;
	char key[blen];
	unsigned char text[blen];
	unsigned char cipher[blen];
	unsigned char plain[blen];

	for(i = 0; i < blen; i++){
		key[i] = i;
		text[i] = 'A' + i;
	}

	aes_block_encrypt((const unsigned char*) key, text, blen, cipher);
	aes_block_decrypt((const unsigned char*) key, cipher, blen, plain);

#ifdef DEBUG
	printf("aes_%d_block_enc_dec decrypted: ", blen*8);
	for (i=0; i<blen; i++) 
	    printf("%c", plain[i]);
	printf("\n");
#endif

	for (i=0; i<blen; i++) 
		assert(plain[i] == text[i]);

	key[2] += 1;
	aes_block_decrypt((const unsigned char*) key, cipher, blen, plain);

	int errors = 0;
	for (i=0; i<blen; i++)
		if(plain[i] != text[i]) errors++;
	assert(errors != 0);
}

void test_aes_cbc_enc_dec(const int blen, const unsigned char* text, const int textlen){

	assert(blen > 0);
	assert(textlen > 0);

	int i;
	char key[blen];
	unsigned char cipher[textlen];
	unsigned char plain[textlen];
	unsigned char iv[textlen];

	for(i = 0; i < blen; i++){
		key[i] = i;
		iv[i] = i+1;
	}

	aes_cbc_block_encrypt((const unsigned char*)key, iv, text, blen, textlen, cipher);
	aes_cbc_block_decrypt((const unsigned char*)key, iv, cipher, blen, textlen, plain);
	for(i = 0; i < blen; i++){
		assert(plain[i] == text[i]);
	}

#ifdef DEBUG
	printf("aes_%d_cbc_enc_dec decrypted: ", blen*8);
	for (i=0; i<textlen; i++) 
	    printf("%c", plain[i]);
	printf("\n");
#endif

	key[2] += 1;
	aes_cbc_block_decrypt((const unsigned char*)key, iv, cipher, blen, textlen, plain);
	int errors = 0;
	for (i=0; i<blen; i++)
		if(plain[i] != text[i]) errors++;
	assert(errors != 0);
	key[2] -= 1;

	iv[3] += 1;
	aes_cbc_block_decrypt((const unsigned char*)key, iv, cipher, blen, textlen, plain);
	errors = 0;
	for (i=0; i<blen; i++)
		if(plain[i] != text[i]) errors++;
	assert(errors != 0);
	
}

void test_kdf2_hmac(){

	int res;	
	const char secret[] = "NHo/iVz/xBgJv2XZ7UrABw==";
	const char salt[] = "saltysalt";
	unsigned char derived[16] = { 0 };
	res = pbkdf2_hmac_sha1(secret, salt, 1003, 16, derived);
	assert(res == 1);

#ifdef DEBUG
	int i;
	for(i = 0; i < 16; i++)
		printf("%c", derived[i]);
	printf("\n");	
#endif
}

#endif

int main(int argc, char** argv) {

#ifdef __APPLE__
	int i;
	const int blen = 16; //128 bit
	const int textlen = 3 * blen; //3 * 128 bit

	unsigned char text[textlen];
	for(i = 0; i < textlen; i++){
		text[i] = 'A' + i;
	}

	test_aes_block_enc_dec(16);
	test_aes_cbc_enc_dec(16, (const unsigned char*)text, textlen); 
	test_kdf2_hmac();	
#endif

	return 0;
}
