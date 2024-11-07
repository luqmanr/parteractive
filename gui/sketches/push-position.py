import time
import paho.mqtt.client as paho

if __name__ == '__main__':
    client = paho.Client()

    client.username_pw_set("iotsbm", "belajariot")
    client.connect("host.docker.internal", 1883, 60)

    client.publish("particles", "fireball")

    max = 512
    i = 0
    while True:
        if i > 512:
            client.publish("particles", "fireball")
            i = 0
        client.publish("position", i)
        i += 1
        time.sleep(0.05)
