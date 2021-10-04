#include "ZJUnet.h"

const int httpsPort = 443;
const char *HTTPS = "";
const char *INIT_URL = "net2.zju.edu.cn";
const char *GET_CHALLENGE_API = "/cgi-bin/get_challenge";
const char *SRUN_PORTAL_API = "/cgi-bin/srun_portal";
const char *N = "200";
const char *TYPE = "1";
const char *ENC = "srun_bx1";
const char *_PADCHAR = "=";
const char *_ALPHA = "LVoJPiCN2R8G90yg+hmFHuacZ1OWMnrsSTXkYpUq/3dlbfKwv6xztjI7DeBE45QA";
const char *ac_id = "3";
const char *randnum = "1234567890123456789012";
const int ELEMENT_COUNT_MAX = 50;

void ZJUnet_init(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = USERNAME,
            .password = PASSWORD,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());


}