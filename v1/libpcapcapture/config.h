#ifndef __AFS_CONFIG_INCLUDED_HPP__
#define __AFS_CONFIG_INCLUDED_HPP__

#include <stdint.h>
#include <iniparser.h>
#include "util.h"

//wrap iniparser dictionary
typedef struct Config {
    dictionary* dict;
} conf_t;

//create and destroy
conf_t*  Config_Init(char* ininame);
void Config_Destroy(conf_t* conf);

//Section names

#define SEC_COMM "Common:"
#define MAX_FILE 1024
#define MAX_FILE_LEN 1024

//Common parameters

#define MAX_CONF_LEN 1024
#define DEF_INI_NAME "config.ini"

#define KEY_COMM_PIN_CPU "is_pin_cpu"
#define DEF_COMM_PIN_CPU 0

#define KEY_COMM_INTERVAL_LEN "interval_len"
#define DEF_COMM_INTERVAL_LEN 1000

#define KEY_COMM_PCAP_IF "pcap_if"
#define DEF_COMM_PCAP_IF "br0"

#define KEY_COMM_PCAP_DSTMAC "pcap_dstmac"
#define DEF_COMM_PCAP_DSTMAC "ether dst 00:1e:67:83:0c:0a"

#define KEY_COMM_PCAP_BUFSIZE "pcap_buf_size"
#define DEF_COMM_PCAP_BUFSIZE 2097152 //2*1024*1024, 2MB

#define KEY_COMM_PCAP_SNAPLEN "pcap_snap_len"
#define DEF_COMM_PCAP_SNAPLEN 65535

#define KEY_COMM_PCAP_TOMS "pcap_to_ms"
#define DEF_COMM_PCAP_TOMS 1

//functions
const char* strconcat(const char* a, const char* b);
void check_instance();

char* getstring(dictionary* dict, const char* sec, const char* key, char* def);
int getint(dictionary* dict,  const char* sec, const char* key, int def);
unsigned long long getull(dictionary* dict,  const char* sec, const char* key, unsigned long long def);
double getdouble(dictionary* dict,  const char* sec, const char* key, double def);
int getboolean(dictionary* dict,  const char* sec, const char* key, int def);

conf_t* Config_Init(char* ininame);
void Config_Destroy(conf_t* conf);

//get common parameters
int conf_common_pin_cpu(conf_t *conf);
unsigned long long conf_common_interval_len(conf_t *conf);
const char *conf_common_pcap_if(conf_t *conf);
const char *conf_common_pcap_dstmac(conf_t *conf);//add
int conf_common_pcap_bufsize(conf_t *conf);
int conf_common_pcap_snaplen(conf_t *conf);
int conf_common_pcap_toms(conf_t *conf);

#endif
