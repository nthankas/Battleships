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
#include "BattleBoats.h"
#include "Agent.h"
#include "Negotiation.h"
#include "Message.h"
#include "Field.h"

#define two 2
// Define functions




void FieldInit(Field *own_field, Field * opp_field) {
    int player1row, player1col;
    for (player1row = 0; player1row < FIELD_ROWS - 1; player1row++) 
    {
        for (player1col = 0; player1col < FIELD_COLS - 1; player1col++) 
        {
            own_field->grid[player1row][player1col] = FIELD_SQUARE_EMPTY;
        }
    }

    int player2row, player2col;
    for (player2row = 0; player2row < FIELD_ROWS - 1; player2row++)
    {
        for (player2col = 0; player2col < FIELD_COLS - 1; player2col++) 
        {
            opp_field->grid[player2row][player2col] = FIELD_SQUARE_UNKNOWN;
        }
    }
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE; 
    own_field->hugeBoatLives = 0;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    own_field->largeBoatLives = 0;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    own_field->mediumBoatLives = 0;
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    own_field->smallBoatLives = 0;
}

// Starting FieldGetSquareStatus
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    if ((row > FIELD_ROWS - 1)||(row < 0)) 
    {
        return FIELD_SQUARE_INVALID;
    }
    if ((col > FIELD_ROWS - 1) || (col < 0))
    {
        return FIELD_SQUARE_INVALID;
    }

    return f->grid[row][col];
}

// Starting FieldSetSquareStatus
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p) {
    SquareStatus lastlist = f->grid[row][col];
    f->grid[row][col] = p;
    return lastlist;
}



// Starting FieldAddBoat
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {

    if ((dir != FIELD_DIR_EAST) && (dir != FIELD_DIR_SOUTH) ) {
        return STANDARD_ERROR;
    }
    if ((boat_type != FIELD_BOAT_TYPE_HUGE) && (boat_type != FIELD_BOAT_TYPE_LARGE)) {
        return STANDARD_ERROR;
    }
    if ((boat_type != FIELD_BOAT_TYPE_SMALL) && (boat_type != FIELD_BOAT_TYPE_MEDIUM)) {
        return STANDARD_ERROR;
    }

    int lastcol;
    if (boat_type == FIELD_BOAT_TYPE_SMALL) {
        if (dir == FIELD_DIR_SOUTH) {
            for (lastcol = 0; lastcol < 3; lastcol++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row + lastcol][col] = FIELD_SQUARE_SMALL_BOAT;
                    own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
                }
            }
        }
    }
    
    int firstcol;
    if (boat_type == FIELD_BOAT_TYPE_LARGE) {
        if (dir == FIELD_DIR_SOUTH) {
            for (firstcol = 0; firstcol < 5; firstcol++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row + lastcol][col] = FIELD_SQUARE_LARGE_BOAT;
                    own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
                }
            }
        }
    }
    
    int secondcol;
    if (boat_type == FIELD_BOAT_TYPE_MEDIUM) {
        if (dir == FIELD_DIR_SOUTH) {
            for (secondcol = 0; secondcol < 4; secondcol++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row + lastcol][col] = FIELD_SQUARE_MEDIUM_BOAT;
                    own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
                }
            }
        }
    }
    
    
    
    int hugecol;
    if (boat_type == FIELD_BOAT_TYPE_HUGE) {
        if (dir == FIELD_DIR_SOUTH) {
            for (hugecol = 0; hugecol < 6; hugecol++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row + lastcol][col] = FIELD_SQUARE_HUGE_BOAT;
                    own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
                }
            }
        }
    }
    
    int lastdigit;
    if (boat_type == FIELD_BOAT_TYPE_SMALL) {
        if (dir == FIELD_DIR_EAST) {
            for (lastdigit = 0; lastdigit < 3; lastdigit++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                 else 
                {
                    own_field->grid[row][col + lastdigit] = FIELD_SQUARE_SMALL_BOAT;
                    own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
                }
            }
        }
    }
    int firstdigit;
    if (boat_type == FIELD_BOAT_TYPE_LARGE) {
        if (dir == FIELD_DIR_EAST) {
            for (firstdigit = 0; firstdigit < 5; firstdigit++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row][col + lastdigit] = FIELD_SQUARE_LARGE_BOAT;
                    own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
                }
            }
        }
    }
    
    int seconddigit;
    if (boat_type == FIELD_BOAT_TYPE_MEDIUM) {
        if (dir == FIELD_DIR_EAST) {
            for (seconddigit = 0; seconddigit < 4; seconddigit++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row][col + lastdigit] = FIELD_SQUARE_MEDIUM_BOAT;
                    own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
                }
            }
        }
    }
      
    
   
    int hugedigit;
    if (boat_type == FIELD_BOAT_TYPE_HUGE) {
        if (dir == FIELD_DIR_EAST) {
            for (hugedigit = 0; hugedigit < 6; hugedigit++) {
                if (own_field->grid[row][col] != FIELD_SQUARE_EMPTY) {
                    return STANDARD_ERROR;
                }
                if (FieldGetSquareStatus(own_field, row, col) == FIELD_SQUARE_INVALID) {
                    return STANDARD_ERROR;
                }
                else 
                {
                    own_field->grid[row][col + lastdigit] = FIELD_SQUARE_HUGE_BOAT;
                    own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
                }
            }
        }
    }
    

    return SUCCESS;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess) {

    SquareStatus statusofenemy = FieldGetSquareStatus(own_field, opp_guess->row, opp_guess->col);

    if ((statusofenemy == FIELD_SQUARE_LARGE_BOAT)) {
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_HIT) {
            if (own_field->largeBoatLives == 0) {
                return RESULT_LARGE_BOAT_SUNK;
        } 
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_MISS) {
            return RESULT_MISS;
        }
        else {
                own_field->largeBoatLives--;
                return RESULT_HIT;
            }
        }
    }
    if ((statusofenemy == FIELD_SQUARE_HUGE_BOAT)) {
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_HIT) {
            if (own_field->hugeBoatLives == 0) {
                return RESULT_HUGE_BOAT_SUNK;
        }
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_MISS) {
            return RESULT_MISS;
        }
         else {
                own_field->hugeBoatLives--;
                return RESULT_HIT;
            }
        }
    }
    
    if ((statusofenemy == FIELD_SQUARE_MEDIUM_BOAT)) {
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_HIT) {
            if (own_field->mediumBoatLives == 0) {
                return RESULT_MEDIUM_BOAT_SUNK;
        }
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_MISS) {
            return RESULT_MISS;
        }
         else {
                own_field->mediumBoatLives--;
                return RESULT_HIT;
            }
        }
    }
    
    if ((statusofenemy == FIELD_SQUARE_SMALL_BOAT)) {
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_HIT) {
            if (own_field->smallBoatLives == 0) {
                return RESULT_SMALL_BOAT_SUNK;
            }
        if (own_field->grid[opp_guess->row][opp_guess->col] == RESULT_MISS) {
            return RESULT_MISS;
        }
         else {
                own_field->smallBoatLives--;
                return RESULT_HIT;
            }
        }
    }

    

    
    
    return own_field->grid[opp_guess->row][opp_guess->col];
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess){
    
    
    SquareStatus statusoffield = FieldGetSquareStatus(opp_field, own_guess->row, own_guess->col);


    if ((statusoffield == FIELD_SQUARE_LARGE_BOAT)) {
        if (opp_field->largeBoatLives == 0) {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_LARGE_BOAT_SUNK;
        } else {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_HIT;
        }
    } else {
        opp_field->grid[own_guess->row][own_guess->col] = RESULT_MISS;
    }
    
    if ((statusoffield == FIELD_SQUARE_HUGE_BOAT)) {
        if (opp_field->hugeBoatLives == 0) {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_HUGE_BOAT_SUNK;
        } else {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_HIT;
        }
    } else {
        opp_field->grid[own_guess->row][own_guess->col] = RESULT_MISS;
    }
    
    if ((statusoffield == FIELD_SQUARE_MEDIUM_BOAT)) {
        if (opp_field->mediumBoatLives == 0) {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_MEDIUM_BOAT_SUNK;
        } else {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_HIT;
        }
    } else {
        opp_field->grid[own_guess->row][own_guess->col] = RESULT_MISS;
    }
    
    if ((statusoffield == FIELD_SQUARE_SMALL_BOAT)) {
        if (opp_field->smallBoatLives == 0) {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_SMALL_BOAT_SUNK;
        } else {
            opp_field->grid[own_guess->row][own_guess->col] = RESULT_HIT;
        }
    } else {
        opp_field->grid[own_guess->row][own_guess->col] = RESULT_MISS;
    }
    
 
    return statusoffield;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f){
    
    if((f->largeBoatLives) && (f->hugeBoatLives) && (f->smallBoatLives > 0) && (f->mediumBoatLives > 0) )
    {
        return FIELD_BOAT_STATUS_LARGE |  FIELD_BOAT_STATUS_HUGE | FIELD_BOAT_STATUS_SMALL | FIELD_BOAT_STATUS_MEDIUM;
        
    } 
    else if((f->hugeBoatLives) && (f->largeBoatLives > 0)  && (f->smallBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_HUGE | FIELD_BOAT_STATUS_LARGE | FIELD_BOAT_STATUS_SMALL;
        
    }
    else if((f->largeBoatLives) && (f->mediumBoatLives > 0) && (f->smallBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_LARGE | FIELD_BOAT_STATUS_MEDIUM | FIELD_BOAT_STATUS_SMALL;
        
    }
    else if((f->hugeBoatLives) && (f->mediumBoatLives > 0) && (f->smallBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_HUGE | FIELD_BOAT_STATUS_MEDIUM | FIELD_BOAT_STATUS_SMALL;
        
    }
    else if(f->largeBoatLives > 0){
        return FIELD_BOAT_STATUS_LARGE;
      
    }
    else if(f->mediumBoatLives > 0){
        return FIELD_BOAT_STATUS_MEDIUM;
        
    }
    else if(f->smallBoatLives > 0){
        return FIELD_BOAT_STATUS_SMALL;
        
    }
    else if(f->hugeBoatLives > 0){
        return FIELD_BOAT_STATUS_HUGE;
    }
    else if((f->largeBoatLives > 0) && (f->mediumBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_LARGE | FIELD_BOAT_STATUS_MEDIUM;
        
    }
    else if((f->hugeBoatLives > 0) && (f->mediumBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_HUGE | FIELD_BOAT_STATUS_MEDIUM;
        
    }
    else if((f->hugeBoatLives > 0) && (f->largeBoatLives > 0)){
        return FIELD_BOAT_STATUS_HUGE | FIELD_BOAT_STATUS_LARGE;
        
    }
    else if((f->mediumBoatLives > 0) && (f->smallBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_MEDIUM | FIELD_BOAT_STATUS_SMALL ;
        
    }
    else if((f->largeBoatLives > 0) && (f->smallBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_LARGE | FIELD_BOAT_STATUS_SMALL;
        
    }
    else if((f->hugeBoatLives > 0) && (f->smallBoatLives > 0))
    {
        return FIELD_BOAT_STATUS_HUGE | FIELD_BOAT_STATUS_SMALL;
        
    }
    
    
    return 0x00;
}



uint8_t FieldAIPlaceAllBoats(Field *own_field){
   
    BoatType ct = FIELD_BOAT_TYPE_SMALL;
    BoatDirection Boatdir;
    
    uint8_t col;
    uint8_t row;
    
    uint8_t placement;
    uint8_t list;
    while (1){
        Boatdir=rand() % two;
        col=rand()%FIELD_COLS;
        row=rand()%FIELD_ROWS;
        
        switch(ct){
            case FIELD_BOAT_TYPE_SMALL:
                list=FieldAddBoat(own_field, col, row, Boatdir, ct);
                if (list==SUCCESS){
                    ct=FIELD_BOAT_TYPE_MEDIUM;
                    
                }
                break;
            case FIELD_BOAT_TYPE_LARGE:
                list=FieldAddBoat(own_field, col, row, Boatdir, ct);
                if (list==SUCCESS){
                    ct=FIELD_BOAT_TYPE_HUGE;
                }
                break;
            case FIELD_BOAT_TYPE_MEDIUM:
                list=FieldAddBoat(own_field, col, row, Boatdir, ct);
                if (list==SUCCESS){
                    ct=FIELD_BOAT_TYPE_LARGE;
                }
                break;
            case FIELD_BOAT_TYPE_HUGE:
                list=FieldAddBoat(own_field, col, row, Boatdir, ct);
                if (list==SUCCESS){
                    return SUCCESS;
                }
                break;
                
        }
        
    }
    
}
   


//FieldAIDecideGuess Function

GuessData FieldAIDecideGuess(const Field *opp_field){
    
    GuessData finalguess; 
    finalguess.col = rand() % FIELD_COLS;
    finalguess.row = rand() % FIELD_ROWS;
    

    SquareStatus statusofenemy = FieldGetSquareStatus(opp_field, finalguess.row, finalguess.col);
    
    if((statusofenemy == FIELD_SQUARE_HIT) || (statusofenemy == FIELD_SQUARE_MISS)){
        FieldAIDecideGuess(opp_field);
    }
    else
    {
        return finalguess;
    }
}