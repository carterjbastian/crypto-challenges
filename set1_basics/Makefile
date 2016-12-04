SRC = base64.c message_util.c message_ops.c
all:
	gcc -o test base64.c message_util.c message_util_test.c -I/usr/local/opt/openssl/include/ -lcrypto

c1: $(SRC) challenge1.c
	gcc -o c1 base64.c message_util.c challenge1.c -I/usr/local/opt/openssl/include/ -lcrypto

c2: $(SRC) challenge2.c
	gcc -o c2 base64.c message_util.c message_ops.c challenge2.c \
	  -I/usr/local/opt/openssl/include/ -lcrypto

c3: $(SRC) challenge3.c
	gcc -ggdb -o c3 base64.c message_util.c message_ops.c challenge3.c \
	  -I/usr/local/opt/openssl/include/ -lcrypto

c4: $(SRC) challenge4.c
	gcc -ggdb -o c4 base64.c message_util.c message_ops.c challenge4.c \
	  -I/usr/local/opt/openssl/include/ -lcrypto

c5: $(SRC) challenge5.c
	gcc -ggdb -o c5 base64.c message_util.c message_ops.c challenge5.c \
	  -I/usr/local/opt/openssl/include/ -lcrypto

c6: $(SRC) challenge6.c
	gcc -ggdb -o c6 base64.c message_util.c message_ops.c challenge6.c \
	  -I/usr/local/opt/openssl/include/ -lcrypto


# Remove files for the form cX where X is a challenge number
clean:
	ls | grep 'c.\{1\}$$' | xargs rm
	rm -rf *.dSYM
