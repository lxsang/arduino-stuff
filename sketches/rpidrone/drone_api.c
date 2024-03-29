#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdint.h>
int main(int arc, const char** argv)
{
	int file_i2c;
	int length;
	uint8_t buffer[60] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x04;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
int i=0;
while(i < 100)
{
	
	//----- WRITE BYTES -----
	buffer[0] = 'r';

	length = 1;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	
	//----- READ BYTES -----
	// handshake, wait for the data is avaiable
	length = 3;			//<<< Number of bytes to read
	while (read(file_i2c, buffer, length) != length || buffer[0] != 1)		
	//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		//printf("No data the i2c bus, retrying...\n");
	}
	//else
	//{
	printf("Data IR: %d\n", buffer[1]);
	printf("SONAR: %d\n",buffer[2]);
	//}

    i++;
}
}



