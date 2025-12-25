#include "meow.h"

#define meowL(a)                (MeowToken){.type = LETTER, .token.letter.l = (a), .optional = false}
#define meowR(a, n)             (MeowToken){.type = REPEATABLE, .token.repeatable.l = (a), .token.repeatable.repeats = (n), .optional = false}
#define meowA(a, b, c, d, e)    (MeowToken){.type = ALTERNATIVE, .token.alternative.alternatives = {(a), (b), (c), (d), (e), 0}, .optional = false}

#define meowLO(a)               (MeowToken){.type = LETTER, .token.letter.l = (a), .optional = true}
#define meowRO(a, n)            (MeowToken){.type = REPEATABLE, .token.repeatable.l = (a), .token.repeatable.repeats = (n), .optional = true}
#define meowAO(a, b, c, d, e)   (MeowToken){.type = ALTERNATIVE, .token.alternative.alternatives = {(a), (b), (c), (d), (e), 0}, .optional = true}
#define meowENDL                (MeowToken){.type = ENDL}
#define meowEND                 (MeowToken){.type = END}

typedef enum
{
    LETTER,
    REPEATABLE,
    ALTERNATIVE,
    ENDL,
    END
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
static const MeowToken meows[] = 
{
    meowL('m'), meowR('e', 2), meowRO('o', 2), meowR('w', 6), meowAO('!', '~', 0, 0, 0), meowENDL,
    meowL('m'), meowR('r', 10), meowA('a', 'o', 'e', 0, 0), meowR('w', 2), meowENDL,
    meowL('m'), meowR('r', 10), meowENDL,
    meowL('p'), meowLO('u'), meowR('r', 10), meowENDL,
    meowL('m'), meowL('o'), meowR('w', 5), meowENDL,
    meowLO('>'), meowL(':'), meowL('3'), meowENDL,
    meowL('>'), meowR('/', 5), meowL('<'), meowENDL,
    meowEND
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

static void insertChar(char* str, char c)
{
    size_t len = strlen(str);
    str[len] = c;
    str[len + 1] = 0;
}

void generateMeowString(char* buffer, uint16_t (*randInt)(uint16_t, uint16_t))
{
    uint8_t tokens = 0;
    int8_t i = 0;

    while(meows[i].type != END)
    {
        if(meows[i].type == ENDL) ++tokens;
        ++i;
    }

    uint8_t row = randInt(0, tokens - 1);
    uint8_t startIdx = 0;
    if(row != 0)
    {
        /*reuse tokens as counter*/
        tokens = 0;
        while(tokens != row)
        {
            if(meows[startIdx].type == ENDL)
                ++tokens;

            ++startIdx;
        }
    }

    for(i = startIdx; meows[i].type != ENDL; i++)
    {
        if(meows[i].optional)
        {
            if(randInt(1, 10) <= 5)
                continue;
        }

        switch(meows[i].type)
        {
            case LETTER:
                if(meows[i].token.letter.l != 0)
                    insertChar(buffer, maybeCapital(meows[i].token.letter.l, randInt));
            break;
            case REPEATABLE:
                if(meows[i].token.repeatable.l != 0 && meows[i].token.repeatable.repeats != 0)
                    for(int8_t j = 0; j < randInt(1, meows[i].token.repeatable.repeats); j++)
                        insertChar(buffer, maybeCapital(meows[i].token.repeatable.l, randInt));
            break;
            case ALTERNATIVE:
                /*this line generates stringop-overread, ignore*/
                insertChar(buffer, maybeCapital(meows[i].token.alternative.alternatives[randInt(0, strlen(meows[i].token.alternative.alternatives) - 1)], randInt));
            break;
            default:
            break;
        }
    }

    if(randInt(1, 10) == 1)
        insertChar(buffer, '\n');
    else
        insertChar(buffer, ' ');
}
