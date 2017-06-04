/* test-spi-rpi-01.c - Programme pour Raspberry Pi */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
        int fd_spi;
        char ligne[80];
        int value;
        unsigned char byte;

        unsigned int speed = 250000;
        char tx[] = {
            0x48, 0x45, 0x4C, 0x4C, 0x4F,
            0x20, 
            0x57, 0x4F, 0x52, 0x4C, 0x44,
            0x0A
	    };
        if (argc != 3) {
                fprintf(stderr, "usage: %s <spi-port> <spi-speed>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        fd_spi = open(argv[1], O_RDWR);
        if (fd_spi < 0) {
                perror(argv[1]);
                exit(EXIT_FAILURE);
        }

        if (sscanf(argv[2], "%d", & speed) != 1) {
                fprintf(stderr, "Wrong value for speed: %s\n", argv[2]);
                exit(EXIT_FAILURE);
        }
        if (ioctl(fd_spi, SPI_IOC_WR_MAX_SPEED_HZ, & speed) != 0) {
                perror("ioctl");
                exit(EXIT_FAILURE);
        }
        int i=0;
       if (write(fd_spi, tx, 12) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        // read back respond
        printf("Read back data\n");
        char b;
        for(i=0; i< 11; i++)
        {
            read(fd_spi, & b, 1);
            fprintf(stdout, "%c", b);
        }
        printf("\n");
        
        /*char rx[12];
        if(read(fd_spi, rx, 11) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        rx[11] = '\0';
        fprintf(stdout, "%s \n", rx);*/
        /*for(i=0; i< 12; i++)
        {
            if (write(fd_spi, tx+i, 1) != 1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }*/
        /*while (fgets(ligne, 80, stdin) != NULL) {
                if (sscanf(ligne, "%d", & value) != 1) {
                        fprintf(stderr, "integer value expected\n");
                        continue;
                }
                byte = (unsigned char) (value & 0xFF);
                if (write(fd_spi, & byte, 1) != 1) {
                        perror("write");
                        exit(EXIT_FAILURE);
                }
        }*/
        close(fd_spi);
        return EXIT_SUCCESS;
}


