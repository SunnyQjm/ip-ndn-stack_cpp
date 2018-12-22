#include <string.h>
#include "config.h"

conf_t* conf = NULL;

const char* strconcat(const char* a, const char* b) { //create a new characters, the first part is a, and the second part is b
    static char l[MAX_CONF_LEN+1];

    if (a == NULL) 
        return b;
    if (b == NULL)
        return NULL;
    
    if (strlen(a) + strlen(b) >= MAX_CONF_LEN) {
        LOG_ERR("error: concat names too long\n");
    }

    strcpy(l, a);
    strcat(l, b);

    return l;
}

//Wrap iniparser
void check_instance() {
    // if (conf = NULL) {
    //     conf = Config_Init(NULL);
    // }
}

const char* doGetSting(dictionary* dict, const char* sec, const char* key, char* def) {
    check_instance();
    return iniparser_getstring(dict, strconcat(sec, key), def);
}
int getint(const dictionary* dict,  const char* sec, const char* key, int def) { //we need section and key
    check_instance();
    return iniparser_getint(dict, strconcat(sec, key), def);
}
unsigned long long getull(const dictionary* dict,  const char* sec, const char* key, unsigned long long def) {
    check_instance();
    const char* str = iniparser_getstring(dict, strconcat(sec, key), NULL);
    if (str == NULL) return def;
    return strtoull(str, NULL, 10);
}
double getdouble(dictionary* dict,  const char* sec, const char* key, double def) {
    check_instance();
    return iniparser_getdouble(dict, strconcat(sec, key), def);
}
int getboolean(dictionary* dict,  const char* sec, const char* key, int def) {
    check_instance();
    return iniparser_getboolean(dict, strconcat(sec, key), def);
}

// create and destroy

conf_t* Config_Init(char* ininame) { //iniparser    get the dictionary object
    conf_t* ret = (conf_t*)calloc(1, sizeof(conf_t));
    if(ret == NULL)
        LOG_ERR("error: allocate config error\n");
    if(ininame) {
        ret->dict = iniparser_load(ininame); //load the iniparser file
    }
    else {
        ret->dict = iniparser_load(DEF_INI_NAME);
    }

    if (ret->dict == 0) {
        LOG_ERR("error: iniparser_load()\n");
    }

    return ret;
}
void Config_Destroy(conf_t* conf) {
    if (conf == NULL)
        return;
    if (conf->dict) {
        iniparser_freedict(conf->dict);
    }
    free(conf);
}

//rountes to get config parameters (common)

int conf_common_pin_cpu(conf_t *conf) {
    return getint(conf->dict, SEC_COMM, KEY_COMM_PIN_CPU, DEF_COMM_PIN_CPU);
}


unsigned long long conf_common_interval_len(conf_t *conf) {
    return getull(conf->dict, SEC_COMM, KEY_COMM_INTERVAL_LEN, DEF_COMM_INTERVAL_LEN);
}

const char *conf_common_pcap_if(conf_t *conf) {
    return doGetSting(conf->dict, SEC_COMM, KEY_COMM_PCAP_IF, DEF_COMM_PCAP_IF);
}

const char *conf_common_pcap_dstmac(conf_t *conf) {
	return doGetSting(conf->dict, SEC_COMM, KEY_COMM_PCAP_DSTMAC, DEF_COMM_PCAP_DSTMAC);
}

int conf_common_pcap_bufsize(conf_t *conf) {
    return getint(conf->dict, SEC_COMM, KEY_COMM_PCAP_BUFSIZE, DEF_COMM_PCAP_BUFSIZE);
}

int conf_common_pcap_snaplen(conf_t *conf) {
    return getint(conf->dict, SEC_COMM, KEY_COMM_PCAP_SNAPLEN, DEF_COMM_PCAP_SNAPLEN);
}

int conf_common_pcap_toms(conf_t *conf) {
    return getint(conf->dict, SEC_COMM, KEY_COMM_PCAP_TOMS, DEF_COMM_PCAP_TOMS);
}