#include <message.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <pio.h>
#include "accelerometer.h"

static void handle_pio_INT1(Task t, MessagePioChanged *pio);
static void AccHandler(Task t, MessageId id, Message payload);
static AccReadTask ReadAcc = {{ AccHandler }}; 


int main(void)
{   
    /* int l;*/
    LIS2DH_init(); 

    MessagePioTask(&ReadAcc.task);

    /* Setup PIO interrupt messages */
    PioDebounce32(ACC_INT_1,  /* PIO pins we are interested in */
                1, 0);                 /* 2 reads and 20ms between them */     
    
    
    MessageSend(&ReadAcc.task,READ_ACC, 0);
    MessageLoop(); /* never returns */
    
    return 0;
}

static void handle_pio_INT1(Task t, MessagePioChanged *pio)
{
    if (pio->state & ACC_INT_1) 
        {
            printf("Interrupt High Detected\n");
            readTap();
        }
       
}

static void AccHandler(Task t, MessageId id, Message payload)
{

    switch(id)
    {
        case READ_ACC:
           if(AccAvailable())
            {
                AccRead();
            }
          /* MessageSendLater(t, READ_ACC, 0, 1); */
           break;
           
        case MESSAGE_PIO_CHANGED:
           handle_pio_INT1(t, (MessagePioChanged*)payload);
           break;

        default:
            printf("Unhandled message 0x%x\n", id);
          break;
    }
    
    
 
}

