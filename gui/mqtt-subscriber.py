import paho.mqtt.client as mqtt
import time
import config

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
	print("Connected with result code " + str(rc))
	client.subscribe("particles")
	client.subscribe("color")
	client.subscribe("position")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if msg.topic == 'particles':
        f = open('particles.txt', 'w')
        f.write(msg.payload.decode())
        f.close()
    elif msg.topic == 'position':
        f = open('position.txt', 'w')
        f.write(msg.payload.decode())
        f.close()

mqttc = mqtt.Client("test_local")
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.username_pw_set(config.MQTT_USER, config.MQTT_PASSWORD)
mqttc.connect_async(config.MQTT_SERVER, config.MQTT_PORT, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
mqttc.loop_start()
while True:
    time.sleep(1)
    # print("sleeping")