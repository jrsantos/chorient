#include <SPI.h>

void write_register(char, int, char);
void read_register(char, int, int, char *);

#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

const int cs_lines[3] = {2, 3, 4};
const int num_chips = 3;

int status = 0;

void adxl345_init(int chip_select)
{
	pinMode(chip_select, OUTPUT);
	digitalWrite(chip_select, HIGH);
	write_register(DATA_FORMAT, chip_select, 0x03);
	write_register(POWER_CTL, chip_select, 0x08);  /* Measurement mode */
}

void setup()
{ 
	int i;

	SPI.begin();
	SPI.setDataMode(SPI_MODE3);
	Serial.begin(9600);

	for (i = 0; i < num_chips; i++)
		adxl345_init(cs_lines[i]); 
}

void print_monitor_orientation(int chip_select)
{
	int x;
	char values[10];

	read_register(DATAX0, chip_select, 6, values);
	x = abs(((int)values[1] << 8) | (int)values[0]);

	if (x < 10)
		Serial.println("Portrait");
	else if (x > 22)
		Serial.println("Landscape");
	else
		Serial.println("---");
}

bool read_orientation(int chip_select)
{
	int x;
	char values[10];

	read_register(DATAX0, chip_select, 6, values);
	x = abs(((int)values[1] << 8) | (int)values[0]);

	if (x < 10)
		return 1;
	else if (x > 22)
		return 0;

	return -1;
}

void loop()
{
	int i;
	int new_status = 0;
	int orientation;

	for (i = 0; i < num_chips; i++) {
		orientation = read_orientation(cs_lines[i]);
		switch(orientation) {
		case 0:
			new_status |= 1 << i;
			break;
		case 1:
			new_status &= ~(1 << i);
			break;
		}
	}

	if (status != new_status) {
		status = new_status;
		Serial.print(num_chips);
		Serial.print(" ");
		Serial.println(status);
	}

	delay(500);
}

void write_register(char register_address, int chip_select, char value)
{
	digitalWrite(chip_select, LOW);

	SPI.transfer(register_address);
	SPI.transfer(value);

	digitalWrite(chip_select, HIGH);
}

void read_register(char register_address, int chip_select, int num_bytes,
		   char *values)
{
	char address = 0x80 | register_address;

	if (num_bytes > 1)
		address = address | 0x40;
  
	digitalWrite(chip_select, LOW);

	SPI.transfer(address);
	for (int i = 0; i < num_bytes; i++)
		values[i] = SPI.transfer(0x00);

	digitalWrite(chip_select, HIGH);
}
