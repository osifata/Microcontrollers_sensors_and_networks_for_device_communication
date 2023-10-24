import paho.mqtt.client as paho
from datetime import datetime

broker = "broker.emqx.io"

client = paho.Client("client-isu-111")

print("Connection to broker", broker)
client.connect(broker)
print("Publishing")

topic = "sofa/command"

last_min = datetime.now().minute

start_second = 20
final_second = 40

pause_message = False

while True:
    current_min = datetime.now().minute
    current_sec = datetime.now().second

    if current_min != last_min:
        if final_second != 35:
            final_second -= 1
        elif start_second != 25:
            start_second += 1
        else:
            start_second = 20
            final_second = 40
    elif not pause_message and start_second <= current_sec <= final_second:  # лампа включена
        state = 'o'
        client.publish(topic, state)
        print(f"state is {state}")
        pause_message = True
    elif pause_message and (start_second > current_sec or current_sec > final_second):  # лампа выключена
        state = 'f'
        client.publish(topic, state)
        print(f"state is {state}")
        pause_message = False

    last_min = current_min

