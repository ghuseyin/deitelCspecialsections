#include <stdio.h>

//Defining SML Operation Codes
//I/O
#define READ 10
#define WRITE 11
//Load/Store
#define LOAD 20
#define STORE 21
//Arithmetic
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
//Transfer of Control
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

//Defining Computer Specifications
#define MEMSIZE 100

int main(void){
    void welcome(void);
int read_inst(size_t* const pInstCounter, short memory[MEMSIZE]);
int execute(size_t* const pInstCounter, short* const pOpCode, short* const pOperand, short* const pInstRegister,
    short* const pAccumulator, short memory[MEMSIZE]);
int dump(const size_t* const pInstCounter,const short* const pOpCode,const short* const pOperand,
    const short* const pInstRegister, const short* const pAccumulator,const short memory[MEMSIZE]);

    //Starting
    short int memory[MEMSIZE] = {0};
    short int opCode=0, operand=0, instRegister=0, accumulator=0;
    size_t instCounter = 0;
    welcome();

    //Reading instructions
    if(read_inst(&instCounter, memory)){
        puts("Error: Program loading error!!");
        dump(&instCounter, &opCode, &operand, &instRegister, &accumulator, memory);
        return -1;
    }
    else
        puts(
            "***        Program loading completed           ***\n");

    
    //Executing instructions
    if(execute(&instCounter, &opCode, &operand, &instRegister, &accumulator, memory)){
        puts("Error: Program execution error!!");
        dump(&instCounter, &opCode, &operand, &instRegister, &accumulator, memory);
        return -2;
    }
    else
        puts(
            "***        Program execution terminated         ***\n");


    //Computer Dump
    if(dump(&instCounter, &opCode, &operand, &instRegister, &accumulator, memory)){
        puts("Error: Computer dump error!!");
        return -3;
    }
    else
        puts(
            "***        Computer dump completed              ***\n");
    
    return 0;
}

void welcome(void){
    puts(
        "***            Welcome to Simpletron            ***\n"
        "***                                             ***\n"
        "*** Please enter your program one instruction   ***\n"
        "*** (or data word) at a time. I will type the   ***\n"
        "*** location number and a question mark (?).    ***\n"
        "*** You then type the word for that location.   ***\n"
        "*** Type the sentinel -9999 to stop entering    ***\n"
        "*** your program.                               ***\n");
}

int read_inst(size_t* const pInstCounter, short memory[MEMSIZE]){
    puts(
        "***        Program loading started             ***");
    for(*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        do{
            printf("%02zu ? ", *pInstCounter);
            scanf("%hd", &memory[*pInstCounter]);
        }while(memory[*pInstCounter]>9999);
        if(-9999==memory[*pInstCounter]) break;
    }
    
    return 0;
}

int execute(size_t* const pInstCounter, short* const pOpCode, short* const pOperand, short* const pInstRegister,
    short* const pAccumulator, short memory[MEMSIZE]){

    puts(
        "***        Program execution begins            ***");
    
    for (*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        *pInstRegister = memory[*pInstCounter]; // Fetch data from memory
        // Parsing memory data to 2-digit operation code and 2-digit operand
        *pOpCode = *pInstRegister / 100;       // First 2-digit is operation code
        *pOperand = *pInstRegister % 100;      // Last 2-digit is operand

        // Selecting and executing instruction
        switch (*pOpCode){
        case READ:
            printf("%02hd ? ", *pOperand);
            scanf("%hd", &memory[*pOperand]);
            break;
        
        case WRITE:
            printf("%04hd Printed: %hd\n",
            *pInstRegister, memory[*pOperand]);
            break;
        
        case LOAD:
            *pAccumulator = memory[*pOperand];
            printf("%04hd Loaded %hd from mem[%02hd] to accumulator.\n",
            *pInstRegister, *pAccumulator, *pOperand);
            break;

        case STORE:
            memory[*pOperand] = *pAccumulator;
            printf("%04hd Stored %hd from accumulator to mem[%hd].\n",
            *pInstRegister, memory[*pOperand], *pOperand);
            break;

        case ADD:
            *pAccumulator += memory[*pOperand];
            printf("%04hd Added %hd to accumulator.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case SUBTRACT:
            *pAccumulator -= memory[*pOperand];
            printf("%04hd Substracted %hd from accumulator.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case DIVIDE:
            *pAccumulator /= memory[*pOperand];
            printf("%04hd Accumulator divided by %hd.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case MULTIPLY:
            *pAccumulator *= memory[*pOperand];
            printf("%04hd Accumulator multiplied by %hd.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case BRANCH:
            printf("%04hd Program branching.\n",
            *pInstRegister);
            *pInstCounter = *pOperand-1; // instCounter will increse after this loop
            break;

        case BRANCHNEG:
            printf("%04hd Program ", *pInstRegister);
            printf((*pAccumulator<0) ? "branching.\n" : "not branched.\n");
            if(*pAccumulator<0) *pInstCounter = *pOperand-1;
            break;
        
        case BRANCHZERO:
            printf("%04hd Program ", *pInstRegister);
            printf((0==*pAccumulator) ? "branching.\n" : "not branched.\n");
            if(0==*pAccumulator) *pInstCounter = *pOperand-1;
            break;
        
        case HALT:
            puts("Done!");
            return 0;
            break;

        default:
            printf("%04hd There is no any instruction. Program will continue.\n",
                *pInstRegister);
            break;
        }
    }
    return 1;
}

int dump(const size_t* const pInstCounter,const short* const pOpCode,const short* const pOperand,
    const short* const pInstRegister, const short* const pAccumulator,const short memory[MEMSIZE]){
    
    printf("\nREGISTERS:\n"
    "accumulator:\t\t%+05hd\n"
    "instructionCounter:\t   %02zu\n"
    "instructionRegister:\t%+05hd\n"
    "operationCode:\t\t   %02hd\n"
    "operand:\t\t   %02hd\n\n",
    *pAccumulator, *pInstCounter, *pInstRegister, *pOpCode, *pOperand);

    puts("MEMORY:");
    printf("%s", "  ");
    for (int i = 0; i < MEMSIZE/10; i++)
        printf("  %5d", i);
    puts("");
    for (size_t i = 0; i < MEMSIZE; i+=10){
        printf("%2zu  ", i);
        for (size_t j = 0; j < MEMSIZE/10; j++)
            printf("%+05hd  ", memory[i+j]);
        puts("");
    }
    return 0;
}
