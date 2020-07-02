#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHBUFFER_SIZE 10

int randRange(int n)
{
    int limit;
    int r;

    limit = RAND_MAX - (RAND_MAX % n);

    while((r = rand()) >= limit);

    return r % n;
}

int main(int argc, char** argv)
{
    time_t t;
    srand(time(NULL));
    int j, chBufferCursor, options = 0, dice = -1, sides = -1, modVal = 0, modOp = -1, top = -1;
    char chBuffer[CHBUFFER_SIZE] = "";
    for(int i = 1; i < argc; ++i){
        j = 0;
        if(argv[i][0] == '-'){
            while(argv[i][j] != '\0'){
                switch(argv[i][j]){
                    case 'v': options |= 1; break;
                }
                ++j;
            }
        }else{
            chBufferCursor = 0;

            // Read numeric characters
            while(argv[i][j] >= '0' && argv[i][j] <= '9'){
                chBuffer[chBufferCursor] = argv[i][j];
                ++chBufferCursor;
                ++j;
            }
            dice = atoi(chBuffer);
            if(dice > 1000){
                printf("Limiting dice count to 1000...");
                dice = 1000;
            }

            if(argv[i][j] == 'd'){
                ++j;
            }

            // Reset character buffer
            chBufferCursor = 0;
            for(int k = 0; k < CHBUFFER_SIZE; ++k) chBuffer[k] = 0;

            // Read numeric characters
            while(argv[i][j] >= '0' && argv[i][j] <= '9'){
                chBuffer[chBufferCursor] = argv[i][j];
                ++chBufferCursor;
                ++j;
            }
            sides = atoi(chBuffer);
            if(sides < 1){
                printf("Setting die sides to 1...");
                sides = 1;
            }

            switch(argv[i][j]){
                case '+':
                    modOp = 1;
                    break;
                case '-':
                    modOp = 2;
                    break;
                case '*':
                    modOp = 3;
                    break;
                case '/':
                    modOp = 4;
                    break;
                case 't':
                    break;
            }
            if(modOp != -1){
                ++j;
                chBufferCursor = 0;
                for(int k = 0; k < CHBUFFER_SIZE; ++k) chBuffer[k] = 0;
                while(argv[i][j] >= '0' && argv[i][j] <= '9'){
                    chBuffer[chBufferCursor] = argv[i][j];
                    ++chBufferCursor;
                    ++j;
                }
                modVal = atoi(chBuffer);
            }
            if(argv[i][j] == 't'){
                ++j;
                options |= 2;
                chBufferCursor = 0;
                for(int k = 0; k < CHBUFFER_SIZE; ++k) chBuffer[k] = 0;
                while(argv[i][j] >= '0' && argv[i][j] <= '9'){
                    chBuffer[chBufferCursor] = argv[i][j];
                    ++chBufferCursor;
                    ++j;
                }
                top = atoi(chBuffer);
            }
        }
    }

    int result = 0;
    // even though I check the value of dice earlier, this eliminates a warning due to GCC stupidity under O2
    if(sizeof(int) * dice > __PTRDIFF_MAX__) return 1;
    int* rolls = malloc(sizeof(int) * dice);
    int grandTotal = 0;
    if(sides > 0){
        for(int i = 0; i < dice; ++i){
            rolls[i] = randRange(sides) + 1;
            if((options & 1) == 1){ // v
                printf("Roll %d: %d\n", i + 1, rolls[i]);
            }
            grandTotal += rolls[i];
        }
    }

    if((options & 2) == 2 && top > 0){ // t
        int i, j, tmp;
        for(int i = 1; i < dice; ++i){
            tmp = rolls[i];
            j = i - 1;
            while(j >= 0 && rolls[j] > tmp){
                rolls[j + 1] = rolls[j];
                --j;
            }
            rolls[j + 1] = tmp;
        }
        for(int i = dice - 1; i >= dice - top; --i){
            result += rolls[i];
        }
    }else{
        result = grandTotal;
    }

    switch(modOp){
        case 1:
            result += modVal;
            break;
        case 2:
            result -= modVal;
            break;
        case 3:
            result *= modVal;
            break;
        case 4:
            result /= modVal;
            break;
    }

    if((options & 1) == 1){ // v
        printf("Modifier: %d", modVal);
        switch(modOp){
            case 1:
                printf(" (Add)");
                break;
            case 2:
                printf(" (Subtract)");
                break;
            case 3:
                printf(" (Multiply)");
                break;
            case 4:
                printf(" (Divide)");
                break;
        }
        printf("\nTotal: ");
    }

    if(dice == 1 && modOp != -1 && rolls[0] == 1){
        printf("%d (natural 1)\n", result);
    }else if(dice == 1 && modOp != -1 && rolls[0] == sides){
        printf("%d (natural %d)\n", result, sides);
    }else{
        printf("%d\n", result);
    }
}
