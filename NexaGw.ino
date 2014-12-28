// Sends HomeEasy/NEXA commands to devices

// One time slot is 250us
#define PULSE_HIGH 250
#define PULSE_LOW_0 250
#define PULSE_LOW_1 1250
#define PULSE_LOW_SYNC 9500
#define PULSE_LOW_PAUSE 2500
#define TX_PIN 13

// Our max message size is sync + message + (dim) + stop = 1 + 32 + 4 + 1
#define MAX_MESSAGE_SIZE 38

unsigned char message[MAX_MESSAGE_SIZE];
byte messagePtr = 0;
byte i, j;

void setup()
{
	pinMode(TX_PIN, OUTPUT);

	Serial.begin(9600);
	while (!Serial) { ; }
}

void loop()
{
	if (Serial.available()) {
		message[messagePtr] = Serial.read();

		if(message[0] != 'S')
		{
			messagePtr = 0;
		}
		else if(message[messagePtr] == 'P')
		{
			// Disable interrupts
			cli();

			for(j = 0;j < 4;j++)
			{
				Serial.print("Sending message: ");

				// Parse message and send
				for(i = 0;i <= messagePtr;i++)
				{
					switch(message[i])
					{
					case 'S':
						sendSync();
						sendPause();
						Serial.print("S");
						break;
					case 'P':
						sendPause();
						Serial.print("P");
						break;
					case '0':
						sendZero();
						sendOne();
						Serial.print("01");
						break;
					case '1':
						sendOne();
						sendZero();
						Serial.print("10");
						break;
					}
				}

				delayMicroseconds(10000);
				Serial.println();
			}

			sei();
			messagePtr = 0;
		}
		else
		{
			messagePtr++;
		}

		if(messagePtr > MAX_MESSAGE_SIZE) messagePtr = 0;
	}
}

void sendOne()
{
	digitalWrite(TX_PIN, HIGH);
	delayMicroseconds(PULSE_HIGH);
	digitalWrite(TX_PIN, LOW);
	delayMicroseconds(PULSE_LOW_1);
}

void sendZero()
{
	digitalWrite(TX_PIN, HIGH);
	delayMicroseconds(PULSE_HIGH);
	digitalWrite(TX_PIN, LOW);
	delayMicroseconds(PULSE_LOW_0);
}

void sendSync()
{
	digitalWrite(TX_PIN, HIGH);
	delayMicroseconds(PULSE_HIGH);
	digitalWrite(TX_PIN, LOW);
	delayMicroseconds(PULSE_LOW_SYNC);
}

void sendPause()
{
	digitalWrite(TX_PIN, HIGH);
	delayMicroseconds(PULSE_HIGH);
	digitalWrite(TX_PIN, LOW);
	delayMicroseconds(PULSE_LOW_PAUSE);
}
