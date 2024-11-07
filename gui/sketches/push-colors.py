import time
import paho.mqtt.client as paho

if __name__ == '__main__':
    client = paho.Client()

    client.username_pw_set("iotsbm", "belajariot")
    client.connect("host.docker.internal", 1883, 60)

    colors = ['#4287f5', '#42f590', '#f5429b', '#f57842', '#4842f5']

    while True:
        for c in colors:
            client.publish("color", c)
            time.sleep(1)
