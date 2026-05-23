#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/HTMLparser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

struct Memory {
	char *data;
	size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t real_size = size * nmemb;

	struct Memory *mem = (struct Memory *)userp;

	char *ptr = realloc(mem->data, mem->size + real_size + 1);
	if(ptr == NULL) {
		return 0;
	}	

	mem->data = ptr;
	memcpy(&(mem->data[mem->size]), contents, real_size);
	mem->size += real_size;
	mem->data[mem->size] = 0;

	return real_size;
}

struct Cabecalho {
	char servidor[256];
	char linguagem[256];
};

static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata){
	size_t total_size = size * nitems;
	struct Cabecalho *dados = (struct Cabecalho *)userdata;

	if(strncasecmp(buffer, "Server:", 7) == 0) {
		sscanf(buffer + 7, "%255s", dados->servidor);
	}
	else if(strncasecmp(buffer, "X-Powered-By:", 13) == 0){
		sscanf(buffer + 13, "%255s", dados->linguagem);
	}
	return total_size;
}	

char *extrair_dominio(const char *url) {
	CURLU *h;
	char *host = NULL;

	h = curl_url();
	if (!h) return NULL;
      	if(curl_url_set(h, CURLUPART_URL, url, 0) == CURLUE_OK) {
		curl_url_get(h, CURLUPART_HOST, &host, 0);
	}	

	curl_url_cleanup(h);
	return host;
}

void extrair_links_externos(const char *html, const char *dominio_base) {
	htmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	doc = htmlReadMemory(html, strlen(html), NULL, NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	if(!doc) {
		printf("Erro ao parsear HTML\n");
		return;
	}

	xpathCtx = xmlXPathNewContext(doc);
	xpathObj = xmlXPathEvalExpression((xmlChar *)"//a[@href]", xpathCtx);

	if(!xpathObj || !xpathObj->nodesetval) {
		printf("Nenhum link encontrado!");
		goto cleanup;
	}

	printf("Links externos encontrados:\n");

	for(int i = 0; i < xpathObj->nodesetval->nodeNr; i++) {
		xmlNodePtr node = xpathObj->nodesetval->nodeTab[i];
		xmlChar *href = xmlGetProp(node, (xmlChar *)"href");

		if(href) {
			if((strncmp((char *)href, "http://", 7) == 0 ||
	    		strncmp((char *)href, "https://", 8) == 0) &&
	    		!strstr((char *)href, dominio_base)) {
				printf("%s\n", href);
			}
		}	
	}
	cleanup:
		if (xpathObj) xmlXPathFreeObject(xpathObj);
	        if (xpathCtx) xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);	
}

int main(void) {
	CURL *curl;
	CURLcode resposta;
	char url[1024];
	char *ip = NULL;
	struct Cabecalho cb = {0};
	struct curl_certinfo *certinfo;
	struct Memory html;
	html.data = malloc(1);
	html.size = 0;	
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if(!curl){
		fprintf(stderr, "Erro ao inicializar a CURL\n");
		return 1;	
	}
	printf("Digite uma URL valida:");
	scanf("%1023s", &url);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback); 	
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &cb);	
	curl_easy_setopt(curl, CURLOPT_ACCEPTTIMEOUT_MS, 5000L);
	curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
	resposta = curl_easy_perform(curl);
	if(resposta != CURLE_OK) {
		fprintf(stderr, "Erro ao acessar o site: %s\n", curl_easy_strerror(resposta));
		return 1;	
	}
	curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
	if(certinfo) {
		if(certinfo->num_of_certs <= 0){
			printf("Nenhuma informacao de certificado SSL disponivel\n");
			return 0;
		}
		printf("Informacoes do certificado SSL\n");

		for(int i = 0; i < certinfo->num_of_certs;i++) {
			struct curl_slist *slist = certinfo->certinfo[i];
			int limite = 0;

			while(slist && limite < 50) {
				if(slist->data){
					if(strncmp(slist->data, "Subject:", 8) == 0 ||
					   strncmp(slist->data, "Start date:", 11) == 0 ||
					   strncmp(slist->data, "Expire date:", 12) == 0) {
						printf("%s\n", slist->data);
					}
				}
				slist = slist->next;
				limite++;
			}
		}
	}
	printf("\n");
	curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip);
	if(ip) {
		printf("IP: %s\n", ip);
	}
	printf("\n");
	printf("Tecnologias web utilizadas\n");
	printf("Servidor: %s\n", cb.servidor);
	printf("X-Powered-By: %s\n", cb.linguagem);
	printf("\n");
	char *dominio_base = extrair_dominio(url);

	if(!dominio_base) {
		printf("Nao foi possivel extrair o dominio da URL\n");
	}
	else {
		extrair_links_externos(html.data, dominio_base);
		curl_free(dominio_base);
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}
