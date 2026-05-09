#include "hardware.h"
#include <stddef.h>

int __write(int handle, const unsigned char *buffer, size_t size)
{
    (void)handle;

    for (size_t i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            USART_Send_Byte(&usart2, '\r');
        }
        USART_Send_Byte(&usart2, buffer[i]);
    }

    return size;
}