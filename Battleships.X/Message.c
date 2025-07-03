#include "Message.h"
#include "BOARD.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

typedef enum {
    WAITING,
    RECORDING_PAYLOAD,
    RECORDING_CHECKSUM,
} MessageState;

MessageState live = WAITING;
int livePayloadLen = 0;
int liveChecksumLen = 0;
char newPayloadString[256] = "";
char newChecksumString[256] = "";

//char test1[256] = "CAT";
//uint8_t answer = 0;
//
//char *test2 = "RES,1,0,3";
//char *checksum2 = "5A";
//BB_Event event1;
//
//char *test3 = "SHO,7,3";
//char *checksum3 = "50";
//BB_Event event2;
//BB_Event event3;
//
//Message message1;
//char *exitString;


//int main() {
//    
//    message1.type = MESSAGE_CHA;
//    message1.param0 = 2;
//    
//    uint8_t answer = Message_CalculateChecksum(test1);
//    int answer2 = Message_ParseMessage(test2, checksum2, &event1);
//    int answer3 = Message_ParseMessage(test3, checksum3, &event2);
//    exitString = (char*)malloc(MESSAGE_MAX_LEN);
//    int answer4 = Message_Encode(exitString, message1);
//    int answer5 = strlen(exitString);
//    int answer6 = Message_Decode('$', &event3);
//    int answer7 = Message_Decode('R', &event3);
//    int answer8 = Message_Decode('E', &event3);
//    int answer9 = Message_Decode('S', &event3);
//    int answer10 = Message_Decode(',', &event3);
//    int answer11 = Message_Decode('1', &event3);
//    int answer12 = Message_Decode(',', &event3);
//    int answer13 = Message_Decode('0', &event3);
//    int answer14 = Message_Decode(',', &event3);
//    int answer15 = Message_Decode('3', &event3);
//    int answer16 = Message_Decode('*', &event3);
//    int answer17 = Message_Decode('5', &event3);
//    int answer18 = Message_Decode('A', &event3);
//    int answer19 = Message_Decode('\n', &event3);
//    
//  
//    
//    return 0;
//    
//}

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */
uint8_t Message_CalculateChecksum(const char* payload) {
    uint8_t sum = payload[0];
    for (int j = 1; j < strlen(payload); j++) {
        sum = (sum ^ (payload[j]));
    }
    return sum;
}

/**
 * ParseMessage() converts a message string into a BB_Event.  The payload and
 * checksum of a message are passed into ParseMessage(), and it modifies a
 * BB_Event struct in place to reflect the contents of the message.
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //A BB_Event which will be modified by this function.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */

int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event) {

    
    const char *delim = ",";
    uint8_t actualChecksum = Message_CalculateChecksum(payload);
    char payloadptr2[256];
    strcpy(payloadptr2, payload);
    char *payloadptr = strtok(payloadptr2, delim);
    
    uint8_t reportedChecksum = (int)strtol(checksum_string, NULL, 16);
    
    if (actualChecksum != reportedChecksum) {
        return STANDARD_ERROR;
    }
    
    if (strlen(checksum_string) != 2) {
        return STANDARD_ERROR;
    }
    
    if (!(strcmp(payloadptr, "CHA"))) {
        message_event->type = BB_EVENT_CHA_RECEIVED;
        int index = 0;
        payloadptr = strtok(NULL, delim);
        while (payloadptr != NULL) {
            if (index == 0) {
                message_event->param0 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index > 0) {
                return STANDARD_ERROR;
            }
        }
        return SUCCESS;
    }

    if (!(strcmp(payloadptr, "ACC"))) {
        message_event->type = BB_EVENT_ACC_RECEIVED;
        int index = 0;
        payloadptr = strtok(NULL, delim);
        while (payloadptr != NULL) {
            if (index == 0) {
                message_event->param0 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index > 0) {
                return STANDARD_ERROR;
            }
        }
        return SUCCESS;
    }

    if (!(strcmp(payloadptr, "REV"))) {
        message_event->type = BB_EVENT_REV_RECEIVED;
        int index = 0;
        payloadptr = strtok(NULL, delim);
        while (payloadptr != NULL) {
            if (index == 0) {
                message_event->param0 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index > 0) {
                return STANDARD_ERROR;
            }
        }
        return SUCCESS;
    }

    if (!(strcmp(payloadptr, "SHO"))) {
        message_event->type = BB_EVENT_SHO_RECEIVED;
        int index = 0;
        payloadptr = strtok(NULL, delim);
        while (payloadptr != NULL) {
            if (index == 0) {
                message_event->param0 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index == 1) {
                message_event->param1 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index > 1) {
                return STANDARD_ERROR;
            }
        }
        return SUCCESS;
    }

    if (!(strcmp(payloadptr, "RES"))) {
        message_event->type = BB_EVENT_RES_RECEIVED;
        int index = 0;
        payloadptr = strtok(NULL, delim);
        while (payloadptr != NULL) {
            if (index == 0) {
                message_event->param0 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index == 1) {
                message_event->param1 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index == 2) {
                message_event->param2 = (uint16_t)(atoi(payloadptr));
                payloadptr = strtok(NULL, delim);
                index++;
            }
            else if (index > 2) {
                return STANDARD_ERROR;
            }
        }
        return SUCCESS;
    }
    else {
        return STANDARD_ERROR;
    }
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 * 
 * typedef enum {
    MESSAGE_NONE, //used if no message is to be sent
    MESSAGE_CHA,
    MESSAGE_ACC,
    MESSAGE_REV,
    MESSAGE_SHO,
    MESSAGE_RES,
            
    //while not required, an error message can be a useful debugging tool:
    MESSAGE_ERROR = -1, 
} MessageType;
 */
int Message_Encode(char *message_string, Message message_to_encode) {
    char returnstring[MESSAGE_MAX_LEN] = "";
    char payload_string[MESSAGE_MAX_PAYLOAD_LEN] = "";
    uint8_t payloadnum = 0;
    switch (message_to_encode.type) {
        case MESSAGE_NONE: {
            return 0;
        }
        case MESSAGE_CHA: {
            sprintf(payload_string, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            payloadnum = Message_CalculateChecksum(payload_string);
            sprintf(returnstring, MESSAGE_TEMPLATE, payload_string, payloadnum);
            break;
        }
        case MESSAGE_ACC: {
            sprintf(payload_string, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            payloadnum = Message_CalculateChecksum(payload_string);
            sprintf(returnstring, MESSAGE_TEMPLATE, payload_string, payloadnum);
            break;
        }
        case MESSAGE_REV: {
            sprintf(payload_string, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            payloadnum = Message_CalculateChecksum(payload_string);
            sprintf(returnstring, MESSAGE_TEMPLATE, payload_string, payloadnum);
            break;
        }
        case MESSAGE_SHO: {
            sprintf(payload_string, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            payloadnum = Message_CalculateChecksum(payload_string);
            sprintf(returnstring, MESSAGE_TEMPLATE, payload_string, payloadnum);
            break;
        }
        case MESSAGE_RES: {
            sprintf(payload_string, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            payloadnum = Message_CalculateChecksum(payload_string);
            sprintf(returnstring, MESSAGE_TEMPLATE, payload_string, payloadnum);
            break;
        }
        case MESSAGE_ERROR: {
            break;
        }
    }
    strcpy(message_string, returnstring);
    return strlen(returnstring);
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    if (char_in == '$' && live == WAITING) {
        live = RECORDING_PAYLOAD;   
        return SUCCESS;
    }
    if (live == RECORDING_PAYLOAD) {
        if (char_in == '$' || char_in == '\n' || livePayloadLen > MESSAGE_MAX_PAYLOAD_LEN) {
            live = WAITING;
            newPayloadString[0] = '\0';
            newChecksumString[0] = '\0';
            livePayloadLen = 0;
            liveChecksumLen = 0;
            return STANDARD_ERROR;
        }
        else if (char_in == '*') {
            live = RECORDING_CHECKSUM;
            return SUCCESS;
        }
        else {
            newPayloadString[livePayloadLen] = char_in;
            livePayloadLen++;
            return SUCCESS;
        }
    }
    else if (live == RECORDING_CHECKSUM) {
        if (isxdigit(char_in) && liveChecksumLen <= MESSAGE_CHECKSUM_LEN) {
            newChecksumString[liveChecksumLen] = char_in;
            liveChecksumLen++;
            return SUCCESS;
        }
        else if (char_in == '\n' && liveChecksumLen <= MESSAGE_CHECKSUM_LEN) {
            if (Message_ParseMessage(newPayloadString, newChecksumString, decoded_message_event) == STANDARD_ERROR) {
                live = WAITING;
                newPayloadString[0] = '\0';
                newChecksumString[0] = '\0';
                livePayloadLen = 0;
                liveChecksumLen = 0;
                return STANDARD_ERROR;
                
            }
            else {
                live = WAITING;
                newPayloadString[0] = '\0';
                newChecksumString[0] = '\0';
                livePayloadLen = 0;
                liveChecksumLen = 0;
                return SUCCESS;
            }
        }
        else {
            live = WAITING;
            newPayloadString[0] = '\0';
            newChecksumString[0] = '\0';
            livePayloadLen = 0;
            liveChecksumLen = 0;
            return STANDARD_ERROR;
        }
    }
    return STANDARD_ERROR;
    
    
}
