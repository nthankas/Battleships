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




int main (void) {
    BOARD_Init();
    
    
    printf("Beginning hworrede's Message Testing Harness, compiled on %s %s\n",__DATE__,__TIME__);
    static char testingpay[MESSAGE_MAX_PAYLOAD_LEN];
    for (int i = 0; i<5;i++){ 
        switch (i){
            case 1:
                sprintf(testingpay, PAYLOAD_TEMPLATE_ACC, 46);
                if (Message_CalculateChecksum(testingpay)==0x6F){
                    printf("Passed Test\n");
                    
                }else{
                    printf("Failed Test\n");
                }
                strcpy(testingpay,"");
                break;
            case 2:
                sprintf(testingpay, PAYLOAD_TEMPLATE_ACC, 12);
                if (Message_CalculateChecksum(testingpay)==0x6E){
                    printf("Passed Test\n");
                    
                }else{
                    printf("Failed Test\n");
                }
                strcpy(testingpay,"");
                break;
            case 3:
                sprintf(testingpay, PAYLOAD_TEMPLATE_ACC, 78);
                if (Message_CalculateChecksum(testingpay)==0x69){
                    printf("Passed Test\n");
                    
                }else{
                    printf("Failed Test\n");
                }
                strcpy(testingpay,"");
                break;
        }
    }
    
    BOARD_End();
    while(1);
}