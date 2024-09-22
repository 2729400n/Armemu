/* Intellectual Proprty of Kevasd*/


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <cstring>
#include "Memulator.hpp"
namespace ARMISA {
struct ARMMachine;

typedef struct logicalBreakdown{
    int unused:24,branch:1,logicalBranch:3,condition:4;
}logicalBreakdown_t;

enum Conditions{
    EQ=0b0000, // Equal
    NE=0b0001, // Not Equal
    CS=0b0010, // carry set
    CC=0b0011, // carry clear
    MI=0b0100, // Signed value
    PL=0b0101, // Signed clear
    VS=0b0110, // Overflow set
    VC=0b0111, // Overflow clear
    HI=0b1000, // US higher
    LS=0b1001, // US LOWER or SAME
    GE=0b1010, // Greater than or Equal
    LT=0b1011,
    GT=0b1100,
    LE=0b1101,
    AL=0b1110 // no checks
};
enum SHIFTTYPE{
    shl,
    shr,
    sar,
    ror
};
struct DPINST{
    unsigned int op2:12,rd:4,Rn:4,S:1,OPCODE:4,I:1,unused:2,cond:4;   
};
struct MultInstruction{
    unsigned int rm:4,opCode:4,Rn:4,RdLo:4,RdHi:4,S:1,A:1,U:1;
};
struct OP2Reg{
    unsigned int Rm:4,shift:8,unused:20;
};
struct OP2Imm{
    unsigned int Rm:8,shift:4,unused:20;
};
struct BX{
    unsigned int rn:4,unused:24,cond:4;   
};
struct BL{
    unsigned int offet:24,Link:1,unused:3,cond:4;   
};
struct MLA{
    unsigned int Rm:4,unused1:4,Rs:4,Rn:4,Rd:4,SS:1,A:1,unused:6,cond:4;
};
struct MLAL{
    unsigned int Rm:4,unused1:4,Rs:4,Rn:4,Rd:4,SS:1,A:1,U:1,unused:5,cond:4;
};


enum DATACSPR{
    AND,
    EOR,
    SUB,
    RSB,
    ADD,
    ADC,
    SBC,
    RSC,
    TST,
    TEQ,
    CMP,
    CMN,
    ORR,
    MOV,
    BIC,
    MVN
};

struct FlagReg {
    int zf:1,of:1,cf:1,af:1,ov:1,sf:1,hlt:1,unused:25;
} ;

typedef struct registerFile{
    union 
    {
        int regs[16] ;
        struct 
        {
            int r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,sp,lr,pc;
            FlagReg Flags;
            

        };
        
    };
    
} registerFile_t;



enum SHIFTER{
    SHL,
    SHR,
    SAR,
    ROR
};


struct ARMMachine:public SMALLMemULatorBase{
    protected:
        int codeSize=0;
        int *base=NULL;
    public:
        registerFile_t registers={0};
        void ResetCLR();
        bool shouldExec(char condition);
        void execute(const int n);
        void load(int* code,int size);
        void load(FILE* codeFile);
        void exec();
        void setFlags(FlagReg flags){
        this->registers.Flags=flags;
    }
};

}
/*class ARMEmulatorHTB:public ARMMachine{
    public:
    ARMEmulatorHTB(){

    }
};
}*/