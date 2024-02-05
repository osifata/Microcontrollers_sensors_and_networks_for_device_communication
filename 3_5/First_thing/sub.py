import time
import paho.mqtt.client as paho
import serial

response = {'d': 7,
             'u': 6}

port = "COM4"
connection = serial.Serial(port, timeout=1)


def on_message(client, userdata, message):
    global response
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    if data in list(response.keys()):
        resp = send_command(data, response[data])
        print(resp)
    print("received message =", data)


def send_command(cmd: str, response_len: int) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len != 0:
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp


broker = "broker.emqx.io"
client = paho.Client("client-isu-111")
client.on_message = on_message

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Subcribing")
client.subscribe("sonya")
time.sleep(100)
client.disconnect()
client.loop_stop()
