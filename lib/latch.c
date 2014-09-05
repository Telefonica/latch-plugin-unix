/*
 * This library implements all functions of Latch API.
 * Copyright (C) 2013 Eleven Paths

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "latch.h"

#define ACCOUNT_ID_MAX_LENGTH 64
#define OPERATION_ID_MAX_LENGTH 20
#define TOKEN_MAX_LENGTH 6

typedef struct curl_response_buffer {
  char *buffer;
  size_t size;
} curl_response_buffer;

/*
 * Function to handle stuff from HTTP response.
 * 
 * @param buf- Raw buffer from libcurl.
 * @param len- number of indexes
 * @param size- size of each index
 * @param userdata- any extra user data needed
 * @return Number of bytes actually handled. If different from len * size, curl will throw an error
 */
static int writeFn(void* buf, size_t len, size_t size, void* userdata) {

    size_t realsize = len * size;
    curl_response_buffer *response = (curl_response_buffer*)userdata;

    response->buffer = realloc(response->buffer, response->size + realsize + 1);

    memcpy(&(response->buffer[response->size]), buf, realsize);
    response->size += realsize;
    response->buffer[response->size] = '\0';

    return realsize;

}

/*
 * Function to encode a string in Base64 format
 * 
 * @param input- string to encode
 * @param length- string length
 * @return encoded string in Base64 format
 */

char* base64encode(const unsigned char *input, int length) {
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char *buff = (char *)malloc(bptr->length);
	memcpy(buff, bptr->data, bptr->length - 1);
	buff[bptr->length - 1] = 0;

	BIO_free_all(b64);

	return buff;
}

/*
 * Function to calculate the HMAC hash (SHA1) of a string. Returns a Base64 value of the hash
 * 
 * @param pSecretKey- secret key
 * @param pData- original data to calculate the HMAC
 * @return HMAC in Base64 format
 */
char* sign_data(const char* pSecretKey, const char* pData) {
	unsigned char* digest;
	
	digest = HMAC(EVP_sha1(), pSecretKey, strlen(pSecretKey), (unsigned char*)pData, strlen(pData), NULL, NULL);
	return base64encode(digest, 20);
}

int nosignal = 0;
int timeout = 2;
const char* AppId;
const char* SecretKey;
const char* Host = "https://latch.elevenpaths.com";
const char* Proxy;
const char* tlsCAFile = NULL;
const char* tlsCAPath = NULL;
const char* tlsCRLFile = NULL;

void init(const char* pAppId, const char* pSecretKey) {
	AppId = pAppId;
	SecretKey = pSecretKey;
}

void setHost(const char* pHost){
	Host = pHost;
}

/*
 * Enable using a Proxy to connect to Latch Server
 */
void setProxy(const char* pProxy){
	Proxy = pProxy;
}

void setTimeout(const int iTimeout)
{
    timeout = iTimeout;
}

/*
 * If called with iNoSignal 1, CURLOPT_NOSIGNAL will be set to 1
 */
void setNoSignal(const int iNoSignal)
{
    nosignal = iNoSignal;
}

void setTLSCAFile(const char* pTLSCAFile)
{
    tlsCAFile = pTLSCAFile;
}

void setTLSCAPath(const char* pTLSCAPath)
{
    tlsCAPath = pTLSCAPath;
}

void setTLSCRLFile(const char* pTLSCRLFile)
{
    tlsCRLFile = pTLSCRLFile;
}

void authenticationHeaders(const char* pHTTPMethod, const char* pQueryString, char* pHeaders[]) {

	char* authHeader, *dateHeader, *stringToSign, *b64hash;
	char utc[20];
	time_t timer;
	struct tm tm_info;
	int len = 0;

	time(&timer);
	gmtime_r(&timer, &tm_info);
	strftime(utc, 20, UTC_STRING_FORMAT, &tm_info);

	len = strlen(pHTTPMethod) + strlen(utc) + strlen(pQueryString) + 4;
	stringToSign = malloc(len);
	snprintf(stringToSign, len, "%s\n%s\n\n%s", pHTTPMethod, utc, pQueryString);

	b64hash = sign_data(SecretKey, stringToSign);

	len = strlen(AUTHORIZATION_HEADER_NAME) + strlen(AUTHORIZATION_METHOD) + strlen(AppId) + strlen(b64hash) + 5;
	authHeader = malloc(len);
	snprintf(authHeader, len, "%s: %s %s %s", AUTHORIZATION_HEADER_NAME, AUTHORIZATION_METHOD, AppId, b64hash);

	len = strlen(DATE_HEADER_NAME) + strlen(utc) + 3;
	dateHeader = malloc(len);
	snprintf(dateHeader, len, "%s: %s", DATE_HEADER_NAME, utc);

	pHeaders[0] = authHeader;
	pHeaders[1] = dateHeader;

    free(stringToSign);
    free(b64hash);

}

/*
 * Perform a GET request to the specified URL of the Latch API
 * @param pUrl- requested URL including host
 */
char* http_get_proxy(const char* pUrl) {

	char* headers[2];
	curl_response_buffer response;
	char error_message[CURL_ERROR_SIZE];
	CURL* pCurl = curl_easy_init();
	int res = -1;
	int i = 0;
	struct curl_slist* chunk = NULL;
	char* hostAndUrl;
	
	if (!pCurl) {
		return NULL;
	}

    response.buffer = malloc(1*sizeof(char));
    response.size = 0;
    response.buffer[response.size] = '\0';

	authenticationHeaders("GET", pUrl, headers);
	for (i=0; i<(sizeof(headers)/sizeof(char*)); i++) {
		chunk = curl_slist_append(chunk, headers[i]);
	}

    free(headers[0]);
    free(headers[1]);

	hostAndUrl = malloc(strlen(Host) + strlen(pUrl) + 1);
	strcpy(hostAndUrl, Host);
	strcat(hostAndUrl, pUrl);

	curl_easy_setopt(pCurl, CURLOPT_URL, hostAndUrl);
	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, chunk);
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeFn);
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1); // we don't care about progress
	curl_easy_setopt(pCurl, CURLOPT_FAILONERROR, 1);

	if(Proxy != NULL){
		curl_easy_setopt(pCurl, CURLOPT_PROXY, Proxy);
	}

	// we don't want to leave our user waiting at the login prompt forever
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, timeout);

	// SSL needs 16k of random stuff. We'll give it some space in RAM.
	curl_easy_setopt(pCurl, CURLOPT_RANDOM_FILE, "/dev/urandom");
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 1);
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 2);
	curl_easy_setopt(pCurl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

	// error message when curl_easy_perform return non-zero
	curl_easy_setopt(pCurl, CURLOPT_ERRORBUFFER, error_message);

	// Optional if setNoSignal(1)
	// Avoid crashing if multithread, DNS timeout and libcurl < 7.32.0
	// Blocks with standard resolver (doesn't apply the timeout)

	if (nosignal == 1) {
	    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);
	}

	if (tlsCAFile != NULL) {
	    curl_easy_setopt(pCurl, CURLOPT_CAINFO, tlsCAFile);
	    curl_easy_setopt(pCurl, CURLOPT_CAPATH, NULL);
	}
	else {
	    if (tlsCAPath != NULL) {
	        curl_easy_setopt(pCurl, CURLOPT_CAINFO, NULL);
	        curl_easy_setopt(pCurl, CURLOPT_CAPATH, tlsCAPath);
	    }
	}

	if (tlsCRLFile != NULL) {
	    curl_easy_setopt(pCurl, CURLOPT_CRLFILE, tlsCRLFile);
	}

	// synchronous, but we don't really care
	res = curl_easy_perform(pCurl);

	curl_easy_cleanup(pCurl);
    curl_slist_free_all(chunk);
    free(hostAndUrl);

    if (res != CURLE_OK) {
        free(response.buffer);
        return NULL;
    }
    else {
        return response.buffer;
    }

}


char* pairWithId(const char* pAccountId) {

    char *response = NULL;
    char *url = NULL;

    if ((url = malloc((strlen(API_PAIR_WITH_ID_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1)*sizeof(char))) == NULL) {
        return NULL;
    }

    snprintf(url, strlen(API_PAIR_WITH_ID_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1, "%s/%s", API_PAIR_WITH_ID_URL, pAccountId);

    response = http_get_proxy(url);

    free(url);

    return response;

}

char* pair(const char* pToken) {

    char *response = NULL;
    char *url = NULL;

    if ((url = malloc((strlen(API_PAIR_URL) + 1 + strnlen(pToken, TOKEN_MAX_LENGTH) + 1)*sizeof(char))) == NULL) {
        return NULL;
    }

    snprintf(url, strlen(API_PAIR_URL) + 1 + strnlen(pToken, TOKEN_MAX_LENGTH) + 1, "%s/%s", API_PAIR_URL, pToken);

    response = http_get_proxy(url);

    free(url);

    return response;

}

char* status(const char* pAccountId) {

    char *response = NULL;
    char *url = NULL;

    if ((url = malloc((strlen(API_CHECK_STATUS_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1)*sizeof(char))) == NULL) {
        return NULL;
    }

    snprintf(url, strlen(API_CHECK_STATUS_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1, "%s/%s", API_CHECK_STATUS_URL, pAccountId);

    response = http_get_proxy(url);

    free(url);

    return response;

}

char* operationStatus(const char* pAccountId, const char* pOperationId) {

    char *response = NULL;
    char *urlA = NULL;
    char *urlB = NULL;

    if ((urlA = malloc((strlen(API_CHECK_STATUS_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1)*sizeof(char))) == NULL) {
        return NULL;
    }

    if ((urlB = malloc((strlen(API_CHECK_STATUS_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 4 + strnlen(pOperationId, OPERATION_ID_MAX_LENGTH) + 1)*sizeof(char))) == NULL) {
        free(urlA);
        return NULL;
    }

    snprintf(urlA, strlen(API_CHECK_STATUS_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1, "%s/%s", API_CHECK_STATUS_URL, pAccountId);
    snprintf(urlB, strlen(urlA) + 4 + strnlen(pOperationId, OPERATION_ID_MAX_LENGTH) + 1, "%s/op/%s", urlA, pOperationId);

    response = http_get_proxy(urlB);

    free(urlA);
    free(urlB);

    return response;

}

char* unpair(const char* pAccountId) {

    char *response = NULL;
    char *url = NULL;

    if ((url = malloc((strlen(API_UNPAIR_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1)*sizeof(char))) == NULL) {
        return NULL;
    }

    snprintf(url, strlen(API_UNPAIR_URL) + 1 + strnlen(pAccountId, ACCOUNT_ID_MAX_LENGTH) + 1, "%s/%s", API_UNPAIR_URL, pAccountId);

    response = http_get_proxy(url);

    free(url);

    return response;

}
