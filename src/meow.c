#include "meow.h"

#define meowL(a)                (MeowToken){.type = LETTER, .token.letter.l = (a), .optional = false}
#define meowR(a, n)             (MeowToken){.type = REPEATABLE, .token.repeatable.l = (a), .token.repeatable.repeats = (n), .optional = false}
#define meowA(a, b, c, d, e)    (MeowToken){.type = ALTERNATIVE, .token.alternative.alternatives = {(a), (b), (c), (d), (e), 0}, .optional = false}

#define meowLO(a)               (MeowToken){.type = LETTER, .token.letter.l = (a), .optional = true}
#define meowRO(a, n)            (MeowToken){.type = REPEATABLE, .token.repeatable.l = (a), .token.repeatable.repeats = (n), .optional = true}
#define meowAO(a, b, c, d, e)   (MeowToken){.type = ALTERNATIVE, .token.alternative.alternatives = {(a), (b), (c), (d), (e), 0}, .optional = true}
#define meowENDL                (MeowToken){.type = ENDL}

typedef enum
{
    LETTER,
    REPEATABLE,
    ALTERNATIVE,
    ENDL,
} TokenType;

typedef struct
{
    TokenType type;
    union
    {
        struct
        {
            char l; 
        } letter;

        struct
        {
            char l;
            uint8_t repeats;
        } repeatable;

        struct 
        {
            const char alternatives[6];
        } alternative;
    } token;
    bool optional;
} MeowToken;

/*
meowL('m') gives you a letter m
meowR('e', 10) repeats letter e 1..10 times
meowA("!~") randomly selects one char from string

Versions with O (meowLO, meowRO, meowAO) make the token optional 
so it can be omitted with 50% chance

All small letters have 10% chance to became capital letter
*/

/*
! Hardcoded array size !
If you want to change meows array, don't forget to change tokenCount and maxLen
*/
#define maxLen 6
#define tokens 7
static const MeowToken meows[tokens][maxLen] = 
{
    {meowL('m'), meowR('e', 2), meowRO('o', 2), meowR('w', 6), meowAO('!', '~', 0, 0, 0), meowENDL},
    {meowL('m'), meowR('r', 10), meowA('a', 'o', 'e', 0, 0), meowR('w', 2), meowENDL},
    {meowL('m'), meowR('r', 10), meowENDL},
    {meowL('p'), meowLO('u'), meowR('r', 10), meowENDL},
    {meowL('m'), meowL('o'), meowR('w', 5), meowENDL},
    {meowLO('>'), meowL(':'), meowL('3'), meowENDL},
    {meowL('>'), meowR('/', 5), meowL('<'), meowENDL}
};

static char maybeCapital(char c, uint16_t (*randInt)(uint16_t, uint16_t))
{
    if(c > 96 && c < 123)
    {
        if(randInt(1, 10) == 1)
            return c - 32;
        else
            return c;
    }
    else
        return c;
}

void generateMeowString(char* buffer, uint16_t (*randInt)(uint16_t, uint16_t))
{
    uint8_t idx = randInt(0, tokens - 1);

    for(int8_t i = 0; meows[idx][i].type != ENDL; i++)
    {
        if(i > maxLen)
            break;

        if(meows[idx][i].optional)
        {
            if(randInt(1, 10) <= 5)
                continue;
        }

        switch(meows[idx][i].type)
        {
            case LETTER:
                if(meows[idx][i].token.letter.l != 0)
                    sprintf(buffer, "%s%c", buffer, maybeCapital(meows[idx][i].token.letter.l, randInt));
            break;
            case REPEATABLE:
                if(meows[idx][i].token.repeatable.l != 0 && meows[idx][i].token.repeatable.repeats != 0)
                    for(int8_t j = 0; j < randInt(1, meows[idx][i].token.repeatable.repeats); j++)
                        sprintf(buffer, "%s%c", buffer, maybeCapital(meows[idx][i].token.repeatable.l, randInt));
            break;
            case ALTERNATIVE:
                sprintf(buffer, "%s%c", buffer, maybeCapital(meows[idx][i].token.alternative.alternatives[randInt(0, strlen(meows[idx][i].token.alternative.alternatives) - 1)], randInt));
            break;
            default:
            break;
        }
    }

    if(randInt(1, 10) == 1)
        sprintf(buffer, "%s%c", buffer, '\n');
    else
        sprintf(buffer, "%s%c", buffer, ' ');
}
