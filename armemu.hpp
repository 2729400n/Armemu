/* Intellectual Proprty of Kevasd*/


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <cstring>
#include "Memulator.hpp"
namespace ARMISA{
struct ARMMachine;
typedef void (*arminst)(ARMMachine* ,int op);
arminst ops[256];
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
    GE=0b1010,
    LT=0b1011,
    GT=0b1100,
    LE=0b1101,
    AL=0b1110
};
enum SHIFTTYPE{
    shl,
    shr,
    sar,
    ror
};
struct DPINST{
    int op2:12,rd:4,Rn:4,S:1,OPCODE:4,I:1,unused:2,cond:4;   
};
struct MultInstruction{
    int rm:4,opCode:4,Rn:4,RdLo:4,RdHi:4,S:1,A:1,U:1;
}
struct OP2Reg{
    int Rm:4,shift:8,unused:20;
};
struct OP2Imm{
    int Rm:8,shift:4,unused:20;
};
struct BX{
    int rn:4,unused:24,cond:4;   
};
struct BL{
    int offet:24,Link:1,unused:3,cond:4;   
};
struct MLA{
    int Rm:4,unused1:4,Rs:4,Rn:4,Rd:4,SS:1,A:1,unused:6,cond:4;
};
struct MLAL{
    int Rm:4,unused1:4,Rs:4,Rn:4,Rd:4,SS:1,A:1,U:1,unused:5,cond:4;
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

typedef struct registerFile{
    union 
    {
        int regs[16] ;
        struct 
        {
            int r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,sp,lr,pc;
            struct 
            {
                int zf:1,of:1,cf:1,af:1,ov:1,sf:1,hlt:1;
            } Flags;
            

        };
        
    };
    
} registerFile_t;






struct ARMMachine:public SMALLMemULatorBase{
    private:
        int codeSize=0;
        int *base=NULL;
    public:
        registerFile_t registers={0};
        void ResetCLR(){
            memset(&registers,0,sizeof(registerFile_t));     
        }
        arminst intructionTable[255];

        
        ARMMachine();

        /**
         * ArmMachine contructor
         * <param name="builder">
         *  Does some of the hard and heavy lifting such as building a 
         *  fake arm intrutin set and a nice reduction to cisc. 
         *  This procedure should take a intance pointer to an ARMMachine
         * </param> 
         */
        ARMMachine(int* code,void (*builder)(ARMMachine*) );

        ~ARMMachine();

    void load(int* code,int size){
        this->ResetCLR();
        this->base=code;
        this->codeSize=size;
    } 


    virtual void execute(int)=0;

    void execute(){

        while (this->registers.Flags.hlt!=1 && (this->registers.pc<=codeSize)){
            this->execute(this->base[this->registers.pc++]);
        }    

    }


};
}

