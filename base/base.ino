#include <Servo.h>
#include <Arduino.h>

#define HAL_SERVO_PIN_YAW 2
#define RX_BUFFER_SIZE 96

typedef struct
{
    int RX;
    int RY;
    int LX;
    int LY;
    int LB;
    int RB;
    int REC;
} ControlPacket;

static char rxBuffer[RX_BUFFER_SIZE];
static uint8_t rxIndex = 0;

static ControlPacket packet;
static bool packetReady = false;


Servo servo_yaw;
int servo_yaw_angle = 0;



void setup() {
    Serial.begin(115200);
  servo_yaw.attach(HAL_SERVO_PIN_YAW);
  
}

bool readIntField(const char* src, const char* key, int* value)
{
    const char* p = strstr(src, key);
    if (p == NULL) return false;

    p += strlen(key);
    *value = atoi(p);

    return true;
}

bool parsePacket(const char* line, ControlPacket* out)
{
    bool ok = true;

    ok &= readIntField(line, "RX=", &out->RX);
    ok &= readIntField(line, "RY=", &out->RY);
    ok &= readIntField(line, "LX=", &out->LX);
    ok &= readIntField(line, "LY=", &out->LY);
    ok &= readIntField(line, "LB=", &out->LB);
    ok &= readIntField(line, "RB=", &out->RB);
    ok &= readIntField(line, "REC=", &out->REC);

    return ok;
}

void serialParserUpdate(Stream* serial)
{
    while (serial->available() > 0)
    {
        char c = serial->read();

        if (c == '\r')
            continue;

        if (c == '\n')
        {
            rxBuffer[rxIndex] = '\0';

            if (rxIndex > 0)
            {
                ControlPacket temp;

                if (parsePacket(rxBuffer, &temp))
                {
                    packet = temp;
                    packetReady = true;
                }
            }

            rxIndex = 0;
            continue;
        }

        if (rxIndex < RX_BUFFER_SIZE - 1)
        {
            rxBuffer[rxIndex++] = c;
        }
        else
        {
            // переполнение — сброс
            rxIndex = 0;
        }
    }
}

bool serialPacketAvailable()
{
    return packetReady;
}

ControlPacket* serialGetPacket()
{
    packetReady = false;
    return &packet;
}

void servo_inc(int& current, int delta)
{
    current += delta / 25;
    if (current < 0) 
    {
        current = 0;
    }
    if (current > 180)
    {
        current = 180;
    }
}

void loop() {

  
  serialParserUpdate(&Serial);
  if (serialPacketAvailable())
  {
    ControlPacket* p = serialGetPacket();
    if (p->REC == 2)
    {
        servo_yaw_angle = 180;    
    }
    else if (p->REC == 4)
    {
        servo_yaw_angle = 0;
    }
    else if (p->REC == 5)
    {
        servo_yaw_angle = 90;
    }
    else
    {
        servo_inc(servo_yaw_angle, p->RX);
    }
  }

  servo_yaw.write(servo_yaw_angle);    
  
  
  
  
  
  
}
