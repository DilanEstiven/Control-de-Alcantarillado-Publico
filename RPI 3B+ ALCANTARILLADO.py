import pandas as pd
import redis
import time
from gpiozero import LED

led = LED(16)
buzzer=LED(2)
    
def mostrar_ultimos_datos(csv_file, num_datos):
    global valor_primera_clave
    # Cargar el archivo CSV en un DataFrame de Pandas
    df = pd.read_csv(csv_file)

    # Obtener los últimos 'num_datos' registros
    ultimos_datos = df.tail(num_datos)
    
    # Convertir los últimos datos a un diccionario
    diccionario = ultimos_datos.to_dict(orient='records')[0]
    
    # Mostrar los últimos datos
    print(ultimos_datos)
    print(diccionario)
    
    primera_clave = next(iter(diccionario))
    valor_primera_clave = diccionario[primera_clave]
    print(valor_primera_clave)
    
r = redis.Redis(
    host ='redis-10162.c282.east-us-mz.azure.cloud.redislabs.com',
    port=10162,
    password='pm2ZRgR6TkIcr1bOR6VnOvn1Xnspt5bU')

ts = r.ts()
#ts.create("ultra1")
    
while True:    
    csv_file = '/home/pi/Documents/ultrasonido.csv'
    num_datos = 1  # Número de últimos datos a mostrar

    mostrar_ultimos_datos(csv_file, num_datos)
    ts.add("ultra1", "*",valor_primera_clave)
    time.sleep(0.3)
    if (int(valor_primera_clave) <= 3):
        led.on()
        buzzer.on()
        time.sleep(0.2)
        led.off()
        buzzer.off()
        time.sleep(0.2)