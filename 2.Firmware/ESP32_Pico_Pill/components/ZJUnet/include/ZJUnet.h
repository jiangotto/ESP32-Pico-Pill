#ifndef __ZJUNET_H__
#define __ZJUNET_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_wifi.h"

#define USERNAME "yourUsername"
#define PASSWORD "yourPassword"

// String ip = "";
// String token = "";
// String request = "";
// String res = "";
// String i = "";
// String hmd5 = "";
// String chksum = "";
// String t;

void ZJUnet_init(void);

#endif