# basic_esp8266_STA_server_onoff

![LED](https://github.com/esp32peacock/basic_esp8266_STA_server_onoff/blob/master/LED.png?raw=true)

This code about Web Browser that connect to STA webserver
can turn on-off GPIO2 on STA client

Emulate that STA web server is a Web Service
and STA client is your project on-off esp8266

* Client can automatic scan Server ip
* if Client has an OLED you can use that ip to serf STA webserver
* Browser that connect to STA server can on-off GPIO on STA client

Browser ---> STA Server ----> STA Client ----> on-off GPIO


             STA Server <---- STA Client ----> show ip of server on OLED
