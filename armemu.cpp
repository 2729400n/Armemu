#include "armemu.hpp"
#include <iostream>
using namespace std;
using namespace ARMISA;



int rotateOP(int op,int rot){
    return (op>>rot)|(op<<(32-rot))
}

int shiftOP(int op,int shift){
    return op>>
}

void UD2(ARMMachine* inst, int instruction){
    printf("Instuction Not Implemented #UD2(%#x)\n",instruction);
}

void movw(ARMMachine* inst, int instruction){
    int register_=(instruction>>12)&0xf,value = (instruction&0xff)|(((instruction>>8)&0xf)|((instruction>>12)&0xf0));
    inst->registers.regs[register_] = (inst->registers.regs[register_]&0xffff0000)|(value&0xffff);
}
void movt(ARMMachine* inst, int instruction){
    int register_=(instruction>>12)&0xf,value = (instruction&0xff)|(((instruction>>8)&0xf)|((instruction>>12)&0xf0));
    inst->registers.regs[register_] = (inst->registers.regs[register_]&0xffff)|(value<<16);
}

void and_(ARMMachine* inst, int instruction){
    DPINST* instruct= (DPINST*)&instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
    int result= instruct->Rn&op2;
}

void orr(ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*) &instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
    int result= instruct->Rn|op2;
}

void eor(ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*)&instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
    int result= instruct->Rn^op2;
}

void add( ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*) &instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
    int result= instruct->Rn+op2;
}

void sub(ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*) &instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
    int result= instruct->Rn-op2;
}

void adc(ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*) &instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
}

void mul(ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*) &instruction;
    int op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8),shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
}

void MathHUB(ARMMachine* inst, int instruction){
    DPINST* instruct =(DPINST*)&instruction;
    if(instruct->OPCODE<CMN){
    switch(instruct->OPCODE &7){
        case AND:
            and_(inst,instruct);
            break;
        case EOR:
            eor(inst,instruct);
            break;
        case SUB:
            sub(inst,instruct);
            break;
        case ADD:
            add(inst,instruct);
            break;
    }
    }else{
    switch (instruct->OPCODE)
    {

    case CMN:
        add(inst,instruct);
        break;
    case ORR:
        orr(inst,instruct);
        break;
    case MOV:
        mov(inst,instruct);
        break;
    case BIC:
        bic(inst,instruct);
        break;
    case MVN: 
        mov(inst,instruct);
        break;
    }
    }
}

void branch(ARMMachine* inst, int instruction){

}
void blockTransfer(ARMMachine* inst, int instruction){

}

class ARMEmulatorHTB:ARMMachine{
    public:
    ARMEmulatorHTB(){

    }
    void execute(const int n){
        logicalBreakdown_t* logict = (logicalBreakdown_t*) &n;
        int result=0;
        if((logict->logicalBranch&0x6)==6){
            return;
        }else{
            switch (logict->logicalBranch)
            {
            case 5:
                branch(this,n);
                break;
            case 4:
                blockTransfer(this,n);

            default:
                break;
            }
            if((logict->logicalBranch&2)!=0){
                
            }else if(logict->branch==1){

            }else if((((MultInstruction*) logict)->opCode==9)&&((logict->branch+logict->logicalBranch)==0)){
                mul(this,n);
            }else{
                MathHUB(this,n);
            }
        }

    }
};

int main(){
    ARMEmulatorHTB();
    return 0;
}