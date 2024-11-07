# Parteractive
Parteractive is a personal project of mine. Combining sensor-reading arduino boards, and making them interact with a simple particle system.

This repository's aim is to show an idea and the possibility of combining IoT devices with other technologies. Hopefully this showcase can inspire and lead to projects and ideas with IoT based technologies.

The boards & sensor I used writing this repository are:
- ESP8266
- HC-SR04 (Ultrasonic Sensor)

# TODOs
1. Create a demo website
2. Record a demo video
3. setup NGINX with TLS for MQTTS
```
stream {

    upstream broker {

        server 10.1.0.3:1883 fail_timeout=1s max_fails=1;
        server 10.1.0.5:1883 fail_timeout=1s max_fails=1;


    }

    server {

#       access_log /var/log/nginx/access.log;
#        error_log /var/log/nginx/error.log;
ssl_certificate /etc/nginx/ssl/domain/server.crt;
    ssl_certificate_key /etc/nginx/ssl/domain/server.key;

    ssl_protocols TLSv1.2;


        listen 1883;
        listen domain.com:8883 ssl;

        proxy_pass broker;

        proxy_connect_timeout 1s;
    }

}
```


# Credits
- My father, [Budi Rahardjo](https://www.youtube.com/@rahard) for providing the codebase for the arduino/esp8266 codes [BRiot-stuff](https://github.com/rahard/BRiot-stuff)
- [Jay Newey](https://github.com/jaynewey) for the awesome particles template [bubbles](https://github.com/jaynewey/bubbles)
