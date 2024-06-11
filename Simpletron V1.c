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

void welcome(void);
int read_opCode(short int memory[MEMSIZE]);
int execute(short int memory[MEMSIZE]);

int main(void){
    //Starting
    short int memory[MEMSIZE] = {0};
    welcome();

    //Reading opCodes
    if(read_opCode(memory)){
        puts("Error: Program loading error!!");
        return -1;
    }
    else
        puts(
        "***        Program loading completed           ***\n");
    
    //Executing instructions
    if(execute(memory)){
        puts("Error: Program execution error!!");
        return -2;
    }
    else
        puts(
            "***        Program execution completed         ***");
    
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

int read_opCode(short int memory[MEMSIZE]){
    puts(
        "***        Program loading started             ***");
    for(size_t instCounter = 0; instCounter < MEMSIZE; instCounter++){
        do{
            printf("%02zu ? ", instCounter);
            scanf("%hd", &memory[instCounter]);
        }while(memory[instCounter]>9999);
        if(-9999==memory[instCounter]) break;
    }
    
    return 0;
}

int execute(short int memory[MEMSIZE]){
    puts(
        "***        Program execution begins            ***");
    //operation code, operand and accumulator variables
    short int opCode, operand, accumulator=0; 
    for (size_t instCounter = 0; instCounter < MEMSIZE; instCounter++){
        // Parsing memory data to 2-digit opCode. code and 2-digit operand
        opCode = memory[instCounter] / 100;       // First 2-digit is operation code
        operand = memory[instCounter] % 100;      // Last 2-digit is operand

        // Selecting and executing opCode
        switch (opCode){
        case READ:
            printf("%02hd ? ", operand);
            scanf("%hd", &memory[operand]);
            break;
        
        case WRITE:
            printf("%04hd Printed: %hd\n",
            memory[instCounter], memory[operand]);
            break;
        
        case LOAD:
            accumulator = memory[operand];
            printf("%04hd Loaded %hd from mem[%02hd] to accumulator.\n",
            memory[instCounter], accumulator, operand);
            break;

        case STORE:
            memory[operand] = accumulator;
            printf("%04hd Stored %hd from accumulator to mem[%hd].\n",
            memory[instCounter], memory[operand], operand);
            break;

        case ADD:
            accumulator += memory[operand];
            printf("%04hd Added %hd to accumulator.\n",
            memory[instCounter], memory[operand]);
            break;

        case SUBTRACT:
            accumulator -= memory[operand];
            printf("%04hd Substracted %hd from accumulator.\n",
            memory[instCounter], memory[operand]);
            break;

        case DIVIDE:
            accumulator /= memory[operand];
            printf("%04hd Accumulator divided by %hd.\n",
            memory[instCounter], memory[operand]);
            break;

        case MULTIPLY:
            accumulator *= memory[operand];
            printf("%04hd Accumulator multiplied by %hd.\n",
            memory[instCounter], memory[operand]);
            break;

        case BRANCH:
            printf("%04hd Program branching.\n",
            memory[instCounter]);
            instCounter = operand-1; // instCounter will increment after this loop
            break;

        case BRANCHNEG:
            printf("%04hd Program ", memory[instCounter]);
            printf((accumulator<0) ? "branching.\n" : "not branched.\n");
            if(accumulator<0) instCounter = operand-1;
            break;
        
        case BRANCHZERO:
            printf("%04hd Program ", memory[instCounter]);
            printf((0==accumulator) ? "branching.\n" : "not branched.\n");
            if(0==accumulator) instCounter = operand-1;
            break;
        
        case HALT:
            printf("%04hd DONE!\n", memory[instCounter]);
            instCounter = MEMSIZE;
            break;

        default:
            printf("%04hd There is not any instruction. Program will continue.\n", memory[instCounter]);
            break;
        }
    }
    return 0;
}
