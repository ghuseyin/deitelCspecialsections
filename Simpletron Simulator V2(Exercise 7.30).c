#include <stdio.h>
#include <limits.h>

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
    int read_inst(size_t* const pInstCounter, int memory[MEMSIZE]);
    int execute(size_t* const pInstCounter, int* const pOpCode, int* const pOperand, int* const pInstRegister,
        int* const pAccumulator, int memory[MEMSIZE]);
    int dump(const size_t instCounter,const int opCode,const int operand,
        const int instRegister, const int accumulator,const int memory[MEMSIZE]);

    //Starting
    size_t instCounter = 0;
    int opCode=0, operand=0, instRegister=0, accumulator=0, memory[MEMSIZE] = {0};
    welcome();

    //Reading instructions
    if(read_inst(&instCounter, memory)){
        puts("Error: Program loading error!!");
        dump(instCounter, opCode, operand, instRegister, accumulator, memory);
        return -1;
    }
    else
        puts(
            "***        Program loading completed            ***\n");

    
    //Executing instructions
    if(execute(&instCounter, &opCode, &operand, &instRegister, &accumulator, memory)){
        puts("Error: Program execution abnormally terminated!!");
        dump(instCounter, opCode, operand, instRegister, accumulator, memory);
        return -2;
    }
    else
        puts(
            "***        Program execution terminated         ***\n");


    //Computer Dump
    if(dump(instCounter, opCode, operand, instRegister, accumulator, memory)){
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
        "*** Type the sentinel -99999 to stop entering   ***\n"
        "*** your program.                               ***\n");
}

int read_inst(size_t* const pInstCounter, int memory[MEMSIZE]){
    puts(
        "***        Program loading started              ***");
    // Instruction reading cycle
    for(*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        do{ // Instruction range control cycle
            printf("%02zu ? ", *pInstCounter);
            scanf("%d", &memory[*pInstCounter]);
        }while((memory[*pInstCounter] < -9999 || memory[*pInstCounter] > 9999)
                && -99999!=memory[*pInstCounter]);
        
        if(-99999==memory[*pInstCounter]) break;
    }
    return 0;
}

int execute(size_t* const pInstCounter, int* const pOpCode, int* const pOperand, int* const pInstRegister,
    int* const pAccumulator, int memory[MEMSIZE]){

    puts(
        "***        Program execution begins             ***");
    
    // Instruction execution cycle
    for (*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        *pInstRegister = memory[*pInstCounter]; // Fetch data from memory
        if(*pInstRegister>9999 || *pInstRegister<-9999){
            printf("%04d There is no any operation code. Program will continue.\n",
                *pInstRegister);
            continue;
        }
        *pOpCode = *pInstRegister / 100;        // First 2-digit is operation code
        *pOperand = *pInstRegister % 100;       // Last 2-digit is operand

        // Selecting and executing instruction
        switch (*pOpCode){
        case READ:
            printf("%02d ? ", *pOperand);
            scanf("%d", &memory[*pOperand]);
            break;
        
        case WRITE:
            printf("%04d Printed: %d\n",
            *pInstRegister, memory[*pOperand]);
            break;
        
        case LOAD:
            *pAccumulator = memory[*pOperand];
            printf("%04d Loaded %d from mem[%02d] to accumulator.\n",
            *pInstRegister, *pAccumulator, *pOperand);
            break;

        case STORE:
            memory[*pOperand] = *pAccumulator;
            printf("%04d Stored %d from accumulator to mem[%d].\n",
            *pInstRegister, memory[*pOperand], *pOperand);
            break;

        case ADD:
            // Accumulator overflow/underflow control
            if((memory[*pOperand] > 0)&&(*pAccumulator > INT_MAX-memory[*pOperand]) ||
                (memory[*pOperand] < 0)&&(*pAccumulator < INT_MIN-memory[*pOperand]))
            {
                printf("%04d Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator += memory[*pOperand];
            printf("%04d Added %d to accumulator.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case SUBTRACT:
            if((memory[*pOperand] < 0)&&(*pAccumulator > INT_MAX+memory[*pOperand]) ||
                (memory[*pOperand] > 0)&&(*pAccumulator < INT_MIN+memory[*pOperand]))
            {
                printf("%04d Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator -= memory[*pOperand];
            printf("%04d Substracted %d from accumulator.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case DIVIDE:
            if(0==memory[*pOperand]){
                printf("%04d Error: Attempt to divide by zero!!\n", *pInstRegister);
                return 2;
            }
            if((INT_MIN==*pAccumulator) && (-1==memory[*pOperand])){
                printf("%04d Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator /= memory[*pOperand];
            printf("%04d Accumulator divided by %d.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case MULTIPLY:
            if( ((memory[*pOperand] > 0)&&(*pAccumulator > INT_MAX/memory[*pOperand])) ||
                ((memory[*pOperand] < -1)&&(*pAccumulator > INT_MIN/memory[*pOperand])) || 
                ((INT_MIN==memory[*pOperand])&&(-1==*pAccumulator)) ||
                ((INT_MIN==*pAccumulator)&&(-1==memory[*pOperand])) )
            {
                printf("%04d Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator *= memory[*pOperand];
            printf("%04d Accumulator multiplied by %d.\n",
            *pInstRegister, memory[*pOperand]);
            break;

        case BRANCH:
            printf("%04d Program branching.\n",
            *pInstRegister);
            *pInstCounter = *pOperand-1; // instCounter will increse after this loop
            break;

        case BRANCHNEG:
            printf("%04d Program ", *pInstRegister);
            printf((*pAccumulator<0) ? "branching.\n" : "not branched.\n");
            if(*pAccumulator<0) *pInstCounter = *pOperand-1;
            break;
        
        case BRANCHZERO:
            printf("%04d Program ", *pInstRegister);
            printf((0==*pAccumulator) ? "branching.\n" : "not branched.\n");
            if(0==*pAccumulator) *pInstCounter = *pOperand-1;
            break;
        
        case HALT:
            printf("%04d Done!\n", *pInstRegister);
            return 0;
            break;

        default:
            printf("%04d There is no any operation code. Program will continue.\n",
                *pInstRegister);
            break;
        }
    }
    return 1; // Program ended without "Halt" operation.
}

int dump(const size_t instCounter,const int opCode,const int operand,
    const int instRegister, const int accumulator,const int memory[MEMSIZE]){
    
    puts("*** Computer Dump Printing ***");
    printf("\nREGISTERS:\n"
    "accumulator:\t\t%+05d\n"
    "instructionCounter:\t   %02zu\n"
    "instructionRegister:\t%+05d\n"
    "operationCode:\t\t   %02d\n"
    "operand:\t\t   %02d\n\n",
    accumulator, instCounter, instRegister, opCode, operand);

    puts("MEMORY:");
    printf("%s", "  ");
    for (int i = 0; i < MEMSIZE/10; i++)
        printf("  %5d", i);
    puts("");
    for (size_t i = 0; i < MEMSIZE; i+=10){
        printf("%2zu  ", i);
        for (size_t j = 0; j < MEMSIZE/10; j++)
            printf("%+05d  ", memory[i+j]);
        puts("");
    }
    puts(
        "*** End of Computer Dump ***");
    return 0;
}
