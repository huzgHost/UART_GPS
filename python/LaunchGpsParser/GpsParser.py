import serial
import time
import sys
import re

class GpsParser:
    def __init__(self, port='/dev/ttyUSB0', baudrate=9600, timeout=1):
        self.port = port
        self.baud = baudrate
        self.timeout = timeout
        self.GpsUart = serial.Serial(port, baudrate, timeout=timeout)
        self.GpsUart.flush()
        self.read()

    # end __init__

    def messageType(self, message):
        checksum = self.checkDataSum(message)
        if checksum is False:
            return None

        if re.match(r'\$GPGGA', message):
            return 'GPGGA'
        elif re.match(r'\$GPRMC', message):
            return 'GPRMC'

        return None

    def checkDataSum(self, message):
        payload = re.split(r'\*', message)
        assert len(payload) == 2
        payload_string = payload[0].strip()
        checksum_string = payload[1].strip()
        checksum = int(checksum_string, base=16)
        calc_checksum = 0x00
        for char in payload_string:
            if r'$' == char:
                continue
            elif r'*' == char:
                break
            else:
                calc_checksum ^= ord(char)

        if checksum == calc_checksum:
            valid_checksum = True
        else:
            valid_checksum = False

        '''
        calc_checksum_str = str(hex(calc_checksum)).lstrip("0x")
        if (calc_checksum_str != checksum_string):
            valid_checksum = False;
        else:
            valid_checksum = True;
        '''

        return valid_checksum

    def parseGPGGA(self, message):
        parsed_input = message.split(',')
        self.time = parsed_input[1]  # UTC time
        self.lat = parsed_input[2]
        # self.lat_ns = parsed_input[3]
        self.lon = parsed_input[4]
        # self.lon_ew = parsed_input[5]
        # self.fix = parsed_input[6]
        # self.sats = parsed_input[7]
        # self.altitude = parsed_input[9]
        return self.current_values()

    def parseGPRMC(self, message):
        parsed_input = message.split(',')
        self.time = parsed_input[1]  # UTC time
        self.lat = parsed_input[3]
        self.lon = parsed_input[5]
        return self.current_values()

    def read(self):
        '''
        text = "$GPGGA,184353.07,1929.045,S,02410.506,E,1,04,2.6,100.00,M,-33.9,M,,0000*6D"
        rawType = self.messageType(text)
        if rawType is None:
            pass
        elif rawType == 'GPGGA':
            return self.parseGPGGA(text)
        elif rawType == 'GPRMC':
            return self.parseGPRMC(text)
        '''

        while True:
            raw = self.GpsUart.readline()
            rawType = self.messageType(raw)

            if rawType is None:
                pass
            elif rawType == 'GPGGA':
                return self.parseGPGGA(raw)
            elif rawType == 'GPRMC':
                return self.parseGPRMC(raw)

    def current_values(self):
        data = {}
        data['time'] = self.time
        data['lat'] = self.lat
        # data['lat_ns'] = self.lat_ns
        data['lon'] = self.lon
        # data['lon_ew'] = self.lon_ew
        # data['fix'] = self.fix
        # data['sats'] = self.sats
        # data['altitude'] = self.altitude

        return data

if __name__ == '__main__':
    launchGps = GpsParser()
    while True:
        print(launchGps.read())
        time.sleep(1)