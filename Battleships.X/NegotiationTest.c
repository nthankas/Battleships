// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

//CE13 standard libraries:
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"

// Battleboats Libraries:
#include "Message.h"
#include "Field.h"
#include "BattleBoats.h"
#include "Agent.h"
#include "Negotiation.h"


//Negotiation Test 
static BB_Event battleboat;




int main (void) {
    BOARD_Init();
    
    
    printf("Beginning of hworrede's Negotiation Testing Harness, compiled on %s %s\n",__DATE__,__TIME__);
    
    if(NegotiationHash(30) == 900) {
        printf("First Test is True\n");
    }
    else {
        printf("First Test is False\n");
    }
    
    if(NegotiationHash(5) == 25) {
        printf("Second Test is True\n");
    }
    else {
        printf("Second Test is False\n");
    }
    

    if(NegotiationVerify(30, 900)) {
        printf("Verify Success\n");
    }
    else {
        printf("Verify Failed\n");
    }
    
    if(NegotiationVerify(5, 25)) {
        printf("Verify Success\n");
    }
    else {
        printf("Verify Failed\n");
    }
    
    NegotiationData A = 0b01010011011101001;
    NegotiationData B = 0b01010011011001001;
    printf("The Test output is %d\n", NegotiateCoinFlip(A, B));
    
    A = 0b01010011011101001;
    B = 0b01010011011101001;
    printf("The Test output is %d\n", NegotiateCoinFlip(A, B));
    
    BOARD_End();
    while(1);
}