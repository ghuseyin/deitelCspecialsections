#include <stdio.h>

//Defining SML Operation Codes
//I/O
#define READ 0x10
#define WRITE 0x11
//Load/Store
#define LOAD 0x20
#define STORE 0x21
//Arithmetic
#define ADD 0x30
#define SUBTRACT 0x31
#define DIVIDE 0x32
#define MULTIPLY 0x33
#define MOD 0x34  // 7.30.b
#define POW 0x35  // 7.30.c
//Transfer of Control
#define BRANCH 0x40
#define BRANCHNEG 0x41
#define BRANCHZERO 0x42
#define HALT 0x43

//Defining Computer Specifications
#define W_MAX 0xFFFF    // Max word size is 4 hex digit: FFFF
#define W_MIN -0xFFFF
#define MEMSIZE 0xFF    // Increamant for 7.30.a 
//7.30.a says 1000 (0x400) but more than FF not usable for a 4-digit instruction computer.

int main(void){
    void welcome(void);
    int read_inst(size_t* const pInstCounter, int memory[MEMSIZE]);
    int execute(size_t* const pInstCounter, size_t* const pOperand, int* const pOpCode,
        int* const pInstRegister, int* const pAccumulator, int memory[MEMSIZE]);
    int dump(const size_t instCounter, const size_t operand, const int opCode,
        const int instRegister, const int accumulator,const int memory[MEMSIZE]);

    //Starting
    size_t instCounter = 0, operand=0;
    int opCode=0, instRegister=0, accumulator=0, memory[MEMSIZE] = {0};
    welcome();

    //Reading instructions
    if(read_inst(&instCounter, memory)){
        puts("Error: Program loading error!!");
        dump(instCounter, operand, opCode, instRegister, accumulator, memory);
        return -1;
    }
    else
        puts(
            "***        Program loading completed            ***\n");

    
    //Executing instructions
    if(execute(&instCounter, &operand, &opCode, &instRegister, &accumulator, memory)){
        puts("Error: Program execution abnormally terminated!!");
        dump(instCounter, operand, opCode, instRegister, accumulator, memory);
        return -2;
    }
    else
        puts(
            "***        Program execution terminated         ***\n");

    //Computer Dump
    if(dump(instCounter, operand, opCode, instRegister, accumulator, memory)){
        puts("Error: Computer dump error!!");
        return -3;
    }

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
        "*** Type the sentinel -FFFFF to stop entering   ***\n"
        "*** your program.                               ***\n");
}

int read_inst(size_t* const pInstCounter, int memory[MEMSIZE]){
    puts(
        "***        Program loading started              ***");
    // Instruction reading cycle
    for(*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        do{ // Instruction range control cycle
            printf("%02lX ? ", *pInstCounter);
            scanf("%X", &memory[*pInstCounter]);
        }while((memory[*pInstCounter] < W_MIN || memory[*pInstCounter] > W_MAX)
            && -0xFFFFF!=memory[*pInstCounter]);
        if(-0xFFFFF==memory[*pInstCounter]) break;
    }
    return 0;
}

int execute(size_t* const pInstCounter, size_t* const pOperand, int* const pOpCode,
    int* const pInstRegister, int* const pAccumulator, int memory[MEMSIZE]){

    puts(
        "***        Program execution begins             ***");
    
    // Instruction execution cycle
    for (*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        *pInstRegister = memory[*pInstCounter]; // Fetch data from memory
        if(*pInstRegister<0x1000 || *pInstRegister>0x4400){
            printf("%04X There is no any operation code. Program will continue.\n",
                *pInstRegister);
            continue;
        }
        *pOpCode = *pInstRegister / 0x100;              // First 2-digit is operation code
        *pOperand = (size_t)(*pInstRegister % 0x100);   // Last 2-digit is operand

        // Selecting and executing instruction
        switch (*pOpCode){
        case READ:
            printf("%02lX ? ", *pOperand);
            scanf("%X", &memory[*pOperand]);
            break;
        
        case WRITE:
            printf("%04X Printed: %X\n",
                *pInstRegister, memory[*pOperand]);
            break;
        
        case LOAD:
            *pAccumulator = memory[*pOperand];
            printf("%04X Loaded %X from mem[%02lX] to accumulator.\n",
                *pInstRegister, *pAccumulator, *pOperand);
            break;

        case STORE:
            memory[*pOperand] = *pAccumulator;
            printf("%04X Stored %X from accumulator to mem[%lX].\n",
                *pInstRegister, memory[*pOperand], *pOperand);
            break;

        case ADD:
            // Accumulator overflow/underflow control
            if((memory[*pOperand] > 0)&&(*pAccumulator > W_MAX-memory[*pOperand]) ||
                (memory[*pOperand] < 0)&&(*pAccumulator < W_MIN-memory[*pOperand]))
            {
                printf("%04X Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator += memory[*pOperand];
            printf("%04X Added %X to accumulator.\n",
                *pInstRegister, memory[*pOperand]);
            break;

        case SUBTRACT:
            if((memory[*pOperand] < 0)&&(*pAccumulator > W_MAX+memory[*pOperand]) ||
                (memory[*pOperand] > 0)&&(*pAccumulator < W_MIN+memory[*pOperand]))
            {
                printf("%04X Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator -= memory[*pOperand];
            printf("%04X Substracted %X from accumulator.\n",
                *pInstRegister, memory[*pOperand]);
            break;

        case DIVIDE:
            if(0==memory[*pOperand]){
                printf("%04X Error: Attempt to divide by zero!!\n", *pInstRegister);
                return 2;
            }
            *pAccumulator /= memory[*pOperand];
            printf("%04X Accumulator divided by %X.\n",
                *pInstRegister, memory[*pOperand]);
            break;

        case MULTIPLY:
            if( ((*pAccumulator > W_MAX/memory[*pOperand])&&(memory[*pOperand] > 0)) ||
                ((*pAccumulator > W_MIN/memory[*pOperand])&&(memory[*pOperand] < 0)) )
            {
                printf("%04X Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator *= memory[*pOperand];
            printf("%04X Accumulator multiplied by %X.\n",
                *pInstRegister, memory[*pOperand]);
            break;

        case MOD:
            if(0==memory[*pOperand]){
                printf("%04X Error: Attempt to divide by zero!!\n", *pInstRegister);
                return 2;
            }
            *pAccumulator %= memory[*pOperand];
            printf("%04X Accumulator mod %X is %X.\n",
                *pInstRegister, memory[*pOperand], *pAccumulator);
            break;
        
        case POW:
            if(memory[*pOperand]>0)
                for(int i = 1, temp=*pAccumulator; i < memory[*pOperand]; i++){
                    *pAccumulator *= temp;
                }
            else if(0==memory[*pOperand]){
                *pAccumulator = 1;
            }
            else if(memory[*pOperand]<0){     //     TODO after 7.30.f
                printf("Power with negative numbers not supported yet.\n"
                    "Program canceling.\n");
                return 4;
            }
            printf("Accumulator ^ %X is %X.\n",
                memory[*pOperand], *pAccumulator);
            break;

        case BRANCH:
            printf("%04X Program branching.\n", *pInstRegister);
            *pInstCounter = *pOperand-1; // instCounter will increse after this loop
            break;

        case BRANCHNEG:
            printf("%04X Program ", *pInstRegister);
            printf((*pAccumulator<0) ? "branching.\n" : "not branched.\n");
            if(*pAccumulator<0) *pInstCounter = *pOperand-1;
            break;
        
        case BRANCHZERO:
            printf("%04X Program ", *pInstRegister);
            printf((0==*pAccumulator) ? "branching.\n" : "not branched.\n");
            if(0==*pAccumulator) *pInstCounter = *pOperand-1;
            break;
        
        case HALT:
            printf("%04X Done!\n", *pInstRegister);
            return 0;
            break;

        default:
            printf("%04X There is no any operation code. Program will continue.\n",
                *pInstRegister);
            break;
        }
    }
    return 1; // Program ended without "Halt" operation.
}

int dump(const size_t instCounter, const size_t operand, const int opCode,
    const int instRegister, const int accumulator,const int memory[MEMSIZE]){
    
    puts("*** Computer Dump Printing ***");
    printf("\nREGISTERS:\n"
        "accumulator:\t\t%04X\n"
        "instructionCounter:\t%02lX\n"
        "instructionRegister:\t%04X\n"
        "operationCode:\t\t%02X\n"
        "operand:\t\t%02lX\n\n",
        accumulator, instCounter, instRegister, opCode, operand);

    puts("MEMORY:");
    printf("%s", "  ");
    for (int i = 0; i < 0x10; i++)
        printf("  %4X", i);
    puts("");
    for (size_t i = 0; i < MEMSIZE; i+=0x10){
        printf("%2lX  ", i);
        for (size_t j = 0; j < 0x10; j++)
            printf("%04X  ", memory[i+j]);
        puts("");
    }
    puts(
        "*** End of Computer Dump ***");
    return 0;
}
