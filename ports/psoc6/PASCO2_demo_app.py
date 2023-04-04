"""
This demo enables a continuous measurement of the PAS CO2 sensor and publishes the value to the database Adafruit IO.
"""
from machine import I2C
from utime import sleep, sleep_ms
import network
from umqtt.simple import MQTTClient

ssid = b"<ssid>"
key = b"<pwd>"

sensor_add = 0x28
meas_interval = 10  # in seconds

i2c = I2C(0)  # default pins: scl = 5, sda = 4


def network_connect():
    wlan = network.WLAN(network.STA_IF)

    if wlan.isconnected():
        print("[Network] already connected")
        return

    # enable and connect wlan
    wlan.active(True)
    wlan.connect(ssid, key)

    # wait for connection to establish
    sleep(5)
    for i in range(0, 100):
        if not wlan.isconnected() and wlan.status() >= 0:
            print("[Network] Waiting to connect..")
            sleep(2)

    # check connection
    if not wlan.isconnected():
        print("[Network] Connection failed!")
    else:
        print(wlan.ifconfig())

    print("network connected successfully !\n")


def database_connect():
    client = MQTTClient(
        client_id=b"rpi-pico",
        server=b"io.adafruit.com",
        port=0,
        user=b"",  # Your Adafruit IO Key -> Username
        password=b"",  # Your Adafruit IO Key -> Active Key
        keepalive=7200,
        ssl=True,
        ssl_params={"server_hostname": "io.adafruit.com"},
    )
    client.connect()
    print("database connected successfully !\n")
    return client


def continuous_init(time=10):
    meas_interval = time  # in seconds

    try:
        # wait for sensor to be ready
        sensor_ready = i2c.readfrom_mem(sensor_add, 0x01, 1)[0] & 0x80
        while not sensor_ready:
            sleep(1)
            sensor_ready = i2c.readfrom_mem(sensor_add, 0x01, 1)[0] & 0x80
            print("[PAS CO2] waiting..")

        # soft reset sensor register
        i2c.writeto_mem(sensor_add, 0x10, b"\xa3")
        sleep_ms(800)

        # set measure rate
        buf = bytes([(meas_interval >> 8) & 0xFF, meas_interval & 0xFF])
        i2c.writeto_mem(sensor_add, 0x02, buf)

        # reset operation mode to idle mode by setting the bit field MEAS_CFG.OP_MODE to 0b00
        buf = bytes([(i2c.readfrom_mem(sensor_add, 0x04, 1)[0] & 0xFC) | 0x00])
        i2c.writeto_mem(sensor_add, 0x04, buf)
        # start continuous mode by setting the bit field MEAS_CFG.OP_MODE to 0b10
        buf = bytes([(i2c.readfrom_mem(sensor_add, 0x04, 1)[0] & 0xFC) | 0x02])
        i2c.writeto_mem(sensor_add, 0x04, buf)

        print("[PAS CO2] Setup done")
    except:
        print("[PAS CO2] Setup error")

    sleep(1)
    print("CO2 sensor connected successfully !\n")


def get_value():
    while True:
        try:
            # get meas status
            tmp = i2c.readfrom_mem(sensor_add, 0x07, 1)
            data_ready = tmp[0] & 0x10

            if data_ready:
                # get CO2 value
                tmp = i2c.readfrom_mem(sensor_add, 0x05, 2)
                co2_value = (tmp[0] << 8) | tmp[1]
                return co2_value
        except:
            return -1


network_connect()
database = database_connect()
# continuous_init(meas_interval)

while True:
    # wait for the value to be ready
    sleep(meas_interval)

    co2ppm = 22  # get_value()

    if co2ppm == -1:
        print("measurement not ready yet")
    else:
        print("co2 ppm value: ", co2ppm)
        # pattern for the publish topic -> '<your_adafruitIO_username>/feeds/<feed_name>'
        database.publish("bargfrede_jens/feeds/CO2-sensor", b'{"value":' + str(co2ppm) + "}")
