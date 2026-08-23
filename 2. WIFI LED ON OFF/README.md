## WIFI LED TURN ON / OFF

When you connect to it using a browser on your phone or computer, it serves up a simple webpage with "Turn ON" and "Turn OFF" links. Clicking those links tells the ESP32 to change the state of the LED.


- Pin `23` i.e. `D23` is set HIGH / LOW using a web interface.

## Web Interface UI

![Web Interface](./images/web_interface.png)

## Expose to ngrok globally

- To forward port to ngrok: `ngrok http <ESP32_IP>:80`
