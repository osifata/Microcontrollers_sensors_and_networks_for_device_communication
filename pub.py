import time
import paho.mqtt.client as mqtt_client
import random

broker = "broker.emqx.io"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)


client= mqtt_client.Client('client-isu-122')
client.on_connect = on_connect
client.connect(broker)
print("Publishing")

for _ in range(10):
    state = "u" if random.randint(0,1) else "d"
    client.publish("sonya", state)
    print(f"publish state is {state}")
    time.sleep(3)

client.disconnect()