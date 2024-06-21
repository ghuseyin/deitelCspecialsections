#include <stdio.h>
#include <stdlib.h>

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
#define MOD 0x34        // 7.30.b
#define POW 0x35        // 7.30.c
//Transfer of Control
#define BRANCH 0x40
#define BRANCHNEG 0x41
#define BRANCHZERO 0x42
#define HALT 0x43
//Character Operations
#define NEWLINE 0x50    // 7.30.e
#define GETSTR 0x51     // 7.30.i
#define PRINTSTR 0x52   // 7.30.j

//Defining Computer Specifications
#define W_MAX 0xFFFF    // Max word size is 4 hex digit: FFFF
#define W_MIN -0xFFFF
#define MEMSIZE 0x100   // Increamant for 7.30.a 
// 7.30.a says 1000 (0x400) but more than FF not usable for a 4-digit instruction computer.

char* lf2hex(double lfvalue);       // For printing floats as hex 
int read_hexf(const size_t index, double *writeAdrr, int readInstBit);   // Read float as hex

int main(void){
    void welcome(void);
    int read_inst(size_t* const pInstCounter, double memory[MEMSIZE]);
    int execute(size_t* const pInstCounter, size_t* const pOperand, int* const pOpCode,
        int* const pInstRegister, double* const pAccumulator, double memory[MEMSIZE]);
    int dump(const size_t instCounter, const size_t operand, const int opCode,
        const int instRegister, const double accumulator,const double memory[MEMSIZE]);

    //Starting
    size_t instCounter=0, operand=0;
    int opCode=0, instRegister=0;
    double accumulator=0, memory[MEMSIZE] = {0};
    welcome();

    //Reading instructions
    if(read_inst(&instCounter, memory)){
        puts("Error: Program loading error!!");
        if(dump(instCounter, operand, opCode, instRegister, accumulator, memory))
            puts("Error: Computer dump error!!");
        return -1;
    }
    else
        puts(
            "***        Program loading completed            ***\n");

    //Executing instructions
    if(execute(&instCounter, &operand, &opCode, &instRegister, &accumulator, memory)){
        puts("Error: Program execution abnormally terminated!!");
        if(dump(instCounter, operand, opCode, instRegister, accumulator, memory))
            puts("Error: Computer dump error!!");
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
        "***             Welcome to Simpletron           ***\n"
        "***                                             ***\n"
        "***  Please enter your program one instruction  ***\n"
        "***  (or data word) at a time. I will type the  ***\n"
        "***  location number and a question mark (?).   ***\n"
        "***  You then type the word for that location.  ***\n"
        "***  Type the sentinel FFFFF to stop entering   ***\n"
        "***  your program.                              ***\n");
}

int read_inst(size_t* const pInstCounter, double memory[MEMSIZE]){
    puts(
        "***        Program loading started              ***");
    // Instruction reading cycle
    for(*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){
        if(read_hexf(*pInstCounter, &memory[*pInstCounter], 1)){
            puts("Error: Instructions can not be read!!");
            return 5;
        }
        if(0xFFFFF==(int)memory[*pInstCounter]) break;
    }
    return 0;
}

int execute(size_t* const pInstCounter, size_t* const pOperand, int* const pOpCode,
    int* const pInstRegister, double* const pAccumulator, double memory[MEMSIZE]){
    
    int get_string(double* writeAdrr);          // for GETSTR operation
    int print_string(const double* readAdrr);   // for PRINTSTR operation

    puts(
        "***        Program execution begins             ***");
    
    // Instruction execution cycle
    for (*pInstCounter = 0; *pInstCounter < MEMSIZE; (*pInstCounter)++){

        *pInstRegister = (int)memory[*pInstCounter];    // Fetch data from memory
        if(*pInstRegister<0x1000 || *pInstRegister>0x6000){ 
            printf("%04X There is no any operation code. Program will continue.\n",
                *pInstRegister);
            continue;
        }
        
        *pOpCode = *pInstRegister / 0x100;              // First 2-digit is operation code
        *pOperand = (size_t)(*pInstRegister % 0x100);   // Last 2-digit is operand

        // Selecting and executing instruction
        switch (*pOpCode){
        case READ:
            if(read_hexf(*pOperand, &memory[*pOperand], 0)){
                puts("Error: Data can not be read!!");
                return 5;
            }
            break;

        case WRITE:
            printf("%04X Printed: %s\n",
                *pInstRegister, lf2hex(memory[*pOperand]));
            break;
        
        case LOAD:
            *pAccumulator = memory[*pOperand];
            printf("%04X Loaded %s from mem[%02lX] to accumulator.\n",
                *pInstRegister, lf2hex(*pAccumulator), *pOperand);
            break;

        case STORE:
            memory[*pOperand] = *pAccumulator;
            printf("%04X Stored %s from accumulator to mem[%lX].\n",
                *pInstRegister, lf2hex(memory[*pOperand]), *pOperand);
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
            printf("%04X Added %s to accumulator.\n",
                *pInstRegister, lf2hex(memory[*pOperand]));
            break;

        case SUBTRACT:
            if((memory[*pOperand] < 0)&&(*pAccumulator > W_MAX+memory[*pOperand]) ||
                (memory[*pOperand] > 0)&&(*pAccumulator < W_MIN+memory[*pOperand]))
            {
                printf("%04X Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator -= memory[*pOperand];
            printf("%04X Substracted %s from accumulator.\n",
                *pInstRegister, lf2hex(memory[*pOperand]));
            break;

        case DIVIDE:
            if(0==(int)memory[*pOperand]){
                printf("%04X Error: Attempt to divide by zero!!\n", *pInstRegister);
                return 2;
            }
            *pAccumulator /= memory[*pOperand];
            printf("%04X Accumulator divided by %s.\n",
                *pInstRegister, lf2hex(memory[*pOperand]));
            break;

        case MULTIPLY:
            if( ((*pAccumulator > W_MAX/memory[*pOperand])&&(memory[*pOperand] > 0)) ||
                ((*pAccumulator > W_MIN/memory[*pOperand])&&(memory[*pOperand] < 0)) )
            {
                printf("%04X Error: Accumulator overflow!!\n", *pInstRegister);
                return 3;
            }
            *pAccumulator *= memory[*pOperand];
            printf("%04X Accumulator multiplied by %s.\n",
                *pInstRegister, lf2hex(memory[*pOperand]));
            break;

        case MOD:
            if(0==(int)memory[*pOperand]){
                printf("%04X Error: Attempt to divide by zero!!\n", *pInstRegister);
                return 2;
            }
            *pAccumulator = (int)(*pAccumulator) % (int)memory[*pOperand];
            printf("%04X Accumulator mod %s is %s.\n",
                *pInstRegister, lf2hex(memory[*pOperand]), lf2hex(*pAccumulator));
            break;
        
        case POW:
            if(memory[*pOperand]>0)
                for(int i = 1, base=*pAccumulator; i < memory[*pOperand]; i++)
                    *pAccumulator *= base;
            
            else if(0==(int)memory[*pOperand])
                *pAccumulator = 1;
            
            else if(memory[*pOperand]<0){
                if(0==(int)*pAccumulator){
                    printf("%04X Error: Attempt to divide by zero!!\n", *pInstRegister);
                    return 2;
                }
                for(int i = 1, base=*pAccumulator; i < -(memory[*pOperand]); i++)
                    *pAccumulator *= base;
                *pAccumulator = 1 / *pAccumulator;
            }
            printf("Accumulator ^ %s is %s.\n",
                lf2hex(memory[*pOperand]), lf2hex(*pAccumulator));
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
            printf((0==(int)*pAccumulator) ? "branching.\n" : "not branched.\n");
            if(0==(int)*pAccumulator) *pInstCounter = *pOperand-1;
            break;
        
        case HALT:
            printf("%04X Done!\n", *pInstRegister);
            return 0;
            break;

        case NEWLINE:
            printf("%04X Printed new line.\n\n", *pInstRegister);
            break;

        case GETSTR:
            printf("%04X Reading string: ", *pInstRegister);
            if(get_string(&memory[*pOperand])){
                puts("Error: String can not be read!!");
                return 6;
            }
            break;

        case PRINTSTR:
            printf("%04X Printing string: ", *pInstRegister);
            if(print_string(&memory[*pOperand])){
                puts("Error: String printing error!!");
                return 7;
            }
            break;

        default:
            printf("%04X There is no any operation code. Program will continue.\n",
                *pInstRegister);
            break;
        }
    }
    return 1;   // Program ended without "Halt" operation.
}

int dump(const size_t instCounter, const size_t operand, const int opCode,
    const int instRegister, const double accumulator,const double memory[MEMSIZE]){
    
    puts("*** Computer Dump Printing ***");
    printf("\nREGISTERS(hex):\n"    // Printing registers
        "accumulator:\t\t%s\n"
        "instructionCounter:\t     %02lX\n"
        "instructionRegister:\t   %04X\n"
        "operationCode:\t\t     %02X\n"
        "operand:\t\t     %02lX\n\n",
        lf2hex(accumulator), instCounter, instRegister, opCode, operand);

    puts("MEMORY(hex):");           // Printing memory dump
    printf("%s", "  ");
    for (int i = 0; i < 0x10; i++)
        printf("  %7X", i);
    puts("");
    for (size_t i = 0; i < MEMSIZE; i+=0x10){
        printf("%2lX  ", i);
        for (size_t j = 0; j < 0x10; j++)
            printf("%s  ", lf2hex(memory[i+j]));
        puts("");
    }
    puts(
        "*** End of Computer Dump ***");
    return 0;
}

char* lf2hex(double lfvalue){
    static char hexf[15];
    sprintf(hexf, "%04X.%02x", (int)lfvalue, (int)(lfvalue*0x100)%0x100);
    return hexf;
}

int read_hexf(const size_t index, double *writeAdrr, int readInstBit){
    char input[7]= {0}, output[10]= {0};
    do{
        printf("%02lX ? ", index);
        fgets(input, 7, stdin);     // Max input 6 character
        if('-'==input[0]){
            input[0] = '0';
            sprintf(output, "-0x%s", input);
        }
        else
            sprintf(output, "0x%s", input);
        sscanf(output, "%lf", writeAdrr);
    }while((*writeAdrr < W_MIN || *writeAdrr > W_MAX) && 
        (0xFFFFF!=(int)(*writeAdrr) || 0==readInstBit) );
    return 0;
}

int get_string(double* writeAdrr){
    int strSize = ((int)(*writeAdrr) / 0x100)+1;
    char buffer[strSize];
    fgets(buffer, strSize, stdin);
    for(size_t i=0; buffer[i]!='\0'; i++){
        if(0==i%2){  // for writing to last 2 digit of memory word
            *writeAdrr += buffer[i];
            writeAdrr++;
        }
        else        // for writing to first 2 digit of memory word
            *writeAdrr = buffer[i]*0x100;
    }
    printf("String saved.\n");
    return 0;
}

int print_string(const double* readAdrr){
    int strSize = ((int)(*readAdrr) / 0x100)+1;
    char buffer[strSize];
    for(size_t i=0; i<strSize && (*readAdrr)!='\0'; i++){
        if(0==i%2){
            buffer[i] = (int)(*readAdrr) % 0x100;
            readAdrr++;
        }
        else
            buffer[i] = (int)(*readAdrr) / 0x100;
    }
    printf("%s\n", buffer);
    return 0;
}