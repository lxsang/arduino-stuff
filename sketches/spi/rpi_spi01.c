/* test-spi-rpi-01.c - Programme pour Raspberry Pi */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <sys/ioctl.h>
#define CMD_SIZE 5 
#define RES_SIZE 10

int main(int argc, char *argv[])
{
        int fd_spi;
        char ligne[80];
        int value;
        unsigned char byte;

        unsigned int speed = 250000;
        char tx[CMD_SIZE];
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
       /* // request sensor data
        tx[0] = 0xF1;
        
       for(i=0; i < CMD_SIZE;i++)
            if (write(fd_spi, tx+i, 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        // read back respond
        //printf("Read back data\n");
        // ten milli section
        nanosleep((const struct timespec[]){{0, 10000000L}}, NULL);
        char b[RES_SIZE];
        for(i=0; i< RES_SIZE; i++)
        {
            read(fd_spi, b+i, 1);
             printf("%.2X ", b[i]);
        }
        printf("\n%.2X ", b[0]);
        printf("%d ", (int)b[1] | (int)(b[2] << 8));
        printf("%d ", (int)b[3] | (int)(b[4] << 8));
        printf("%d ", (int)b[5] | (int)(b[6] << 8));
        printf("%d ", b[7]);
        printf("%d \n", b[8]);*/
        // set actuator
        tx[0] = 0xF2;
        tx[1] = 1;
        tx[2] = 100;
        tx[3] = 1;
        tx[4] = 100;
        
        for(i=0; i < CMD_SIZE;i++)
            if (write(fd_spi, tx+i, 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
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





