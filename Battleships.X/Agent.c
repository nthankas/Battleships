#include <stdint.h>
#include "Message.h"
#include "BattleBoats.h"
#include "Agent.h"
#include "FieldOled.h"
#include "Negotiation.h"
#include "Oled.h"

#include "BOARD.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

BB_Event test1;
Message ans1;

/**BB_EVENT_NO_EVENT, //0
    BB_EVENT_START_BUTTON, //1
    BB_EVENT_RESET_BUTTON, //2
    BB_EVENT_CHA_RECEIVED, //3
    BB_EVENT_ACC_RECEIVED, //4
    BB_EVENT_REV_RECEIVED, //5
    BB_EVENT_SHO_RECEIVED, //6
    BB_EVENT_RES_RECEIVED, //7
    BB_EVENT_MESSAGE_SENT, //8
    BB_EVENT_ERROR, //9*/

int main () {
    AgentInit();
    test1.type = BB_EVENT_START_BUTTON;
    ans1 = AgentRun(test1);
    return 1;
}

typedef struct {
    AgentState currentState;
    int counter;
    FieldOledTurn currentPlayer;
} Agent;

typedef struct {
    Field fieldplayer;
    Field fieldOpp;
    NegotiationData secret;
    NegotiationData secretOpp;
    NegotiationData hash;
    NegotiationData hashOpp;
    GuessData me;
    GuessData opp;
} User;

Agent currentAgent;
User playerA;
User resetPlayer;


/**
* The Init() function for an Agent sets up everything necessary for an agent before the game
* starts.  At a minimum, this requires:
*   -setting the start state of the Agent SM.
*   -setting turn counter to 0
* If you are using any other persistent data in Agent SM, that should be reset as well.
* 
* It is not advised to call srand() inside of AgentInit.  
 * 
 * typedef enum {
    AGENT_STATE_START, //0
    AGENT_STATE_CHALLENGING, //1
    AGENT_STATE_ACCEPTING, //2
    AGENT_STATE_ATTACKING, //3
    AGENT_STATE_DEFENDING, //4
    AGENT_STATE_WAITING_TO_SEND, //5
    AGENT_STATE_END_SCREEN, //6

    //If implementing a human agent, you will need a state to setup boats:
    AGENT_STATE_SETUP_BOATS, //7
} AgentState;
*  */

void AgentInit(void) {
    currentAgent.currentState = AGENT_STATE_START;
    currentAgent.counter = 0;
    currentAgent.currentPlayer = FIELD_OLED_TURN_NONE;
    
    playerA = resetPlayer;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 * 
 * typedef enum {
    BB_EVENT_NO_EVENT, //0
    BB_EVENT_START_BUTTON, //1
    BB_EVENT_RESET_BUTTON, //2
    BB_EVENT_CHA_RECEIVED, //3
    BB_EVENT_ACC_RECEIVED, //4
    BB_EVENT_REV_RECEIVED, //5
    BB_EVENT_SHO_RECEIVED, //6
    BB_EVENT_RES_RECEIVED, //7
    BB_EVENT_MESSAGE_SENT, //8
    BB_EVENT_ERROR, //9

    //the following events are only used for human agents:
    BB_EVENT_SOUTH_BUTTON, //10
    BB_EVENT_EAST_BUTTON, //11

} BB_EventType;
 */
Message AgentRun(BB_Event event) {
    Message returnMessage;
    returnMessage.type = MESSAGE_NONE;
    switch (event.type) {
        case BB_EVENT_START_BUTTON: {
            playerA.secret = rand();
            playerA.hash = NegotiationHash(playerA.secret);
            returnMessage.type = MESSAGE_CHA;
            returnMessage.param0 = playerA.hash;
            FieldInit(&(playerA.fieldplayer), &(playerA.fieldOpp));
            FieldAIPlaceAllBoats(&(playerA.fieldplayer));
            currentAgent.currentState = AGENT_STATE_CHALLENGING;
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            OledUpdate();
            return returnMessage;
        }
        case BB_EVENT_CHA_RECEIVED: {
            playerA.secret = rand();
            playerA.hashOpp = event.param0;
            FieldInit(&(playerA.fieldplayer), &(playerA.fieldOpp));
            FieldAIPlaceAllBoats(&(playerA.fieldplayer));
            returnMessage.type = MESSAGE_ACC;
            returnMessage.param0 = playerA.secret;
            currentAgent.currentState = AGENT_STATE_ACCEPTING;
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            OledUpdate();
            return returnMessage;
        }
        case BB_EVENT_ACC_RECEIVED: {
            playerA.secretOpp = event.param0;
            returnMessage.type = MESSAGE_REV;
            returnMessage.param0 = playerA.secret;
            if (NegotiateCoinFlip(playerA.secret, playerA.secretOpp) == HEADS) {
                currentAgent.currentState = AGENT_STATE_WAITING_TO_SEND;
                currentAgent.currentPlayer = FIELD_OLED_TURN_MINE;
            }
            else if (NegotiateCoinFlip(playerA.secret, playerA.secretOpp) == TAILS) {
                currentAgent.currentState = AGENT_STATE_DEFENDING;
                currentAgent.currentPlayer = FIELD_OLED_TURN_THEIRS;
            }
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            OledUpdate();
            return returnMessage;
        }
        case BB_EVENT_REV_RECEIVED: {
            playerA.secretOpp = event.param0;
            if (NegotiationVerify(playerA.secretOpp, playerA.hashOpp) == FALSE) {
                currentAgent.currentState = AGENT_STATE_END_SCREEN;
                FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
                OledDrawString("Cheating detected, game over.");
                OledUpdate();
                break;
            }
            else if (NegotiateCoinFlip(playerA.secretOpp, playerA.secret) == HEADS) {
                currentAgent.currentPlayer = FIELD_OLED_TURN_THEIRS;
                currentAgent.currentState = AGENT_STATE_DEFENDING;
                FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
                OledUpdate();
                break;
            }
            else if (NegotiateCoinFlip(playerA.secretOpp, playerA.secret) == TAILS) {
                currentAgent.currentPlayer = FIELD_OLED_TURN_MINE;
                playerA.me = FieldAIDecideGuess(&(playerA.fieldOpp));
                returnMessage.type = MESSAGE_SHO;
                returnMessage.param0 = playerA.me.row;
                returnMessage.param1 = playerA.me.col;
                currentAgent.currentState = AGENT_STATE_ATTACKING;
                FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
                OledUpdate();
                return returnMessage;
            }
        }
        case BB_EVENT_SHO_RECEIVED: {
            playerA.opp.col = event.param0;
            playerA.opp.row = event.param1;
            FieldRegisterEnemyAttack(&(playerA.fieldplayer), &(playerA.opp));
            returnMessage.type = MESSAGE_RES;
            returnMessage.param0 = playerA.opp.col;
            returnMessage.param1 = playerA.opp.row;
            returnMessage.param2 = playerA.opp.result;
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            OledUpdate();
            if (FieldGetBoatStates(&(playerA.fieldplayer)) == 0) {
                OledDrawString("You lose.");
                OledUpdate();
                currentAgent.currentState = AGENT_STATE_END_SCREEN;
            }
            else {
                currentAgent.currentState = AGENT_STATE_WAITING_TO_SEND;
                currentAgent.currentPlayer = FIELD_OLED_TURN_MINE;
                FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
                OledUpdate();
            }
            return returnMessage;
        }
        case BB_EVENT_MESSAGE_SENT: {
            currentAgent.counter++;
            playerA.me = FieldAIDecideGuess(&(playerA.fieldOpp));
            returnMessage.type = MESSAGE_SHO;
            returnMessage.param0 = playerA.me.row;
            returnMessage.param1 = playerA.me.col;
            currentAgent.currentState = AGENT_STATE_ATTACKING;
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            OledUpdate();
            return returnMessage;
        }
        
        case BB_EVENT_RES_RECEIVED: {
            playerA.opp.row = event.param0;
            playerA.opp.col = event.param1;
            playerA.opp.result = event.param2;
            FieldUpdateKnowledge(&(playerA.fieldOpp), &(playerA.me));
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            OledUpdate();
            if (FieldGetBoatStates(&(playerA.fieldOpp)) == 0) {
                OledDrawString("You win.");
                OledUpdate();
                currentAgent.currentState = AGENT_STATE_END_SCREEN;
                break;
            }
            else {
                currentAgent.currentState = AGENT_STATE_DEFENDING;
                currentAgent.currentPlayer = FIELD_OLED_TURN_THEIRS;
                FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
                OledUpdate();
                break;
            }
        }
        case BB_EVENT_RESET_BUTTON: {
            AgentInit();
            OledDrawString("This is BattleBoats!\nPress BTN4 to\nchallenge, or wait\nfor opponent.");
            OledUpdate();
            break;
        }
        
        case BB_EVENT_ERROR: {
            OledDrawString("Try again.");
            OledUpdate();
            FieldOledDrawScreen(&(playerA.fieldplayer), &(playerA.fieldOpp), currentAgent.currentPlayer, currentAgent.currentPlayer);
            break;
        }
        
        case BB_EVENT_NO_EVENT:
        default: {
            break;
        }
    }
    return returnMessage;
}

AgentState AgentGetState(void) {
    return currentAgent.currentState;
}

void AgentSetState(AgentState newState) {
    currentAgent.currentState = newState;
}