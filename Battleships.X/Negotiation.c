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
#include "Oled.h"
#include "Buttons.h"
#include "Uart1.h"


// Battleboats Libraries:
#include "Negotiation.h"



/*** Define Negotation.h functions here ***/

NegotiationData NegotiationHash(NegotiationData secret) {
    uint16_t finallist; 
    finallist = (secret*secret) % PUBLIC_KEY;
    return finallist;
}

int NegotiationVerify(NegotiationData secret, NegotiationData commitment) {
    uint16_t finalkey;
    finalkey = NegotiationHash(secret);
    if (finalkey == commitment) {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

NegotiationOutcome NegotiateCoinFlip(NegotiationData A, NegotiationData B) {
    if (A^B) {
        return HEADS;
    }
    else 
    {
        return TAILS;
    }
}