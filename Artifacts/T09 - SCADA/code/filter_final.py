#
# ENISA
# ARTIFACT 09 - SCADA
# Author: Oscar Delgado, oscar.delgado@hackrocks.com
#

# This challange inserts arbitrary strings into a RAM dump file.
# The strings are read from a text file, whose name is passed on the command line.

from scapy.all import *

def process_pcap(input_file, output_file):
    packets = rdpcap(input_file)

    modbus_function_4_packets = []
    modbus_function_15_packets = []

    # Filtrar paquetes según el número de función
    for packet in packets:
        if packet.haslayer(TCP) and packet.haslayer(Raw):
            data = packet[Raw].load
            function_code = data[7] if len(data) >= 8 else None
            if function_code == 4:
                modbus_function_4_packets.append(packet)
            elif function_code == 15:
                modbus_function_15_packets.append(packet)

    # Tomar solo los primeros 50 paquetes de función 4
    modbus_function_4_packets = modbus_function_4_packets[:50]

    # Tomar solo los primeros 30 paquetes de función 15
    modbus_function_15_packets = modbus_function_15_packets[:30]

    # Cambiar número de función a 5 para los paquetes de función 15
    for packet in modbus_function_15_packets:
        packet[Raw].load = packet[Raw].load[:7] + b'\x05' + packet[Raw].load[8:]

    # Guardar los paquetes filtrados en un nuevo archivo
    filtered_packets = modbus_function_4_packets + modbus_function_15_packets
    wrpcap(output_file, filtered_packets)

# Nombre del archivo de entrada (captura original)
input_file = 'old_captures/ModbusTCP.pcap'

# Nombre del archivo de salida (captura filtrada)
output_file = 'captura_filtrada.pcap'

# Procesar el archivo PCAP
process_pcap(input_file, output_file)

print('Proceso completado. Paquetes filtrados y guardados en', output_file)