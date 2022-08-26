#include <Streaming.h>

#define RX1 18
#define TX1 19

byte voltage[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xCB};
byte current[] = {0x01, 0x04, 0x00, 0x06, 0x00, 0x02, 0x91, 0xCA};
byte activePower[] = {0x01, 0x04, 0x00, 0x0C, 0x00, 0x02, 0xB1, 0xC8};
byte activeEnergy[] = {0x01, 0x04, 0x01, 0x56, 0x00, 0x02, 0x90, 0x27};
double v, c, p, E;

struct readModbus
{
    int *buffer;
    int length;
} rVoltage, rCurrent, rPower, rEnergy;

readModbus recieveData(byte funcCode[], int len)
{
    static int arr[50];
    int lenCount = 0;
    Serial1.write(funcCode, len);
    delay(50);
    while (Serial1.available())
    {
        arr[lenCount] = Serial1.read();
        lenCount++;
    }
    return {arr, lenCount};
}

double hex2float(int hexArr[], int len)
{
    String strBIN = "";
    for (size_t i = len - 6; i <= len - 3; i++)
    {
        String arr = String(hexArr[i], BIN);
        int lenArr = arr.length();
        if (lenArr < 8)
        {
            for (size_t i = 0; i < 8 - lenArr; i++)
            {
                arr = "0" + arr;
            }
        }
        strBIN += arr;
    }
    if (strBIN.length() <= 32)
    {
        int signBit;
        if (strBIN[0] == '0')
            signBit = 1;
        else if (strBIN[0] == '1')
            signBit = -1;
        String compBIN = "";
        for (size_t i = 1; i < 9; i++)
        {
            compBIN += strBIN[i];
        }
        int compINT = strtol(compBIN.c_str(), NULL, 2);
        double actualComp = pow(2, compINT - 127);
        String mantissa = "";
        double actualMantis = 0;
        for (size_t i = 9; i < strBIN.length(); i++)
        {
            mantissa += strBIN[i];
        }
        for (int i = 0; i < mantissa.length(); i++)
        {
            actualMantis += String(mantissa[i]).toDouble() * pow(2, -(i + 1));
        }
        actualMantis = actualMantis + 1;
        return signBit * actualComp * actualMantis;
    }
}
void setup()
{
    Serial.begin(115200);
    Serial1.begin(19200, SERIAL_8N2, RX1, TX1);
    delay(500);
}
void loop()
{

    rVoltage = recieveData(voltage, 8);
    v = hex2float(rVoltage.buffer, rVoltage.length);
    rCurrent = recieveData(current, 8);
    c = hex2float(rCurrent.buffer, rCurrent.length);
    rPower = recieveData(activePower, 8);
    p = hex2float(rPower.buffer, rPower.length);
    rEnergy = recieveData(activeEnergy, 8);
    E = hex2float(rEnergy.buffer, rEnergy.length);

    Serial << "Voltage : " << v << " V."
           << " Current : " << c << " A."
           << " Active power : " << p << " W."
           << " Total active Energy : " << E << " kWh." << endl;
    delay(500);
}
