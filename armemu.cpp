#include "armemu.hpp"
#include <iostream>
using namespace std;
namespace ARMISA
{
    



FlagReg getFlags(long long result ){
   FlagReg reg;
   reg.zf= result==0;
   reg.sf= ((result>>31)&1)&&((result>>32)!=0);
   reg.of=(((result>>31)&1)&&((result>>32)==0));
   reg.cf= (result>>32)!=0;
   return reg;
}


int rotateOP(int op,int rot){
    return (op>>rot)|(op<<(32-rot));
}

int shiftOP(int op,int shift){
    switch (shift)
    {
    default:
        break;
    }
    return op>>shift;
}

void UD2(ARMMachine* inst, int instruction){
    printf("Instuction Not Implemented #UD2(%#x)\n",instruction);
}

int mov(int op1,int op2){
    return op2;
}

int bic(int op1,int op2){
    return op1&(~op2);
}

long long and_(int op1,int op2){
    return op1&op2;
}

long long orr(int op1,int op2){
    return op1|op2;
}

long long eor(int op1,int op2){
    return op1^op2;
}

long long add(int op1,int op2){
    return op1+op2;
}

long long sub(long long op1,int op2){
    return op1-op2;
}

long long mul(ARMMachine* inst, int op1,int op2){
    return op1*op2;

}

void MathHUB(ARMMachine* inst, int instruction){
    DPINST* instruct=(DPINST*) &instruction;
    int *dest = &(inst->registers.regs[instruct->rd]),op1=inst->registers.regs[instruct->Rn],
    op2=instruct->I==1?rotateOP(inst->registers.regs[instruct->op2&0xff],(instruct->op2&0xf00)>>8):shiftOP(inst->registers.regs[instruct->op2&0xf],(instruct->op2&0xff0)>>4);
    long long result =0;
    if(instruct->OPCODE<CMN){
    switch(instruct->OPCODE &7){
        case AND:
            
            result =and_(op1,op2);
            break;

        case EOR:
            result =eor(op1,op2);
            break;

        case SBC:
        case RSC:
            op2+=inst->registers.Flags.cf-1;
        case RSB: 
            if((instruct->OPCODE&1)!=0){
                int temp=op1;
                op1=op2;
                op2=temp;
            }            
        case SUB:        
            result =sub(op1,op2);
            break;

        case ADC:  
            op2+=inst->registers.Flags.cf;  
        case ADD:                
            result =add(op1,op2);
            break;
    }
    if(instruct->S){
        inst->setFlags(getFlags(result));
    }
    if(instruct->OPCODE&8!=0){
        *dest=result;
    }
    return;
    }else{
    switch (instruct->OPCODE)
    {

    case CMN:
        add(op1,op2);
        break;
    case ORR:
        orr(op1,op2);
        break;
    case MOV:
        mov(op1,op2);
        break;
    case BIC:
        bic(op1,op2);
        break;
    case MVN: 
        mov(op1,~op2);
        break;
    }
    }
}

void branch(ARMMachine* inst, int instruction){

}
void blockTransfer(ARMMachine* inst, int instruction){

}

class ARMEmulatorHTB:public ARMMachine{
    public:
    ARMEmulatorHTB(){

    }

    void ResetCLR(){
            memset(&registers,0,sizeof(registerFile_t));     
        }
    bool shouldExec(char condition){
        switch (condition)
        {
        case EQ:
            return this->registers.Flags.zf==1;                     
        case NE:    
            return this->registers.Flags.zf==0;
        case CS:
            return this->registers.Flags.cf==1;
        case CC:    
            return this->registers.Flags.cf==0;
        case MI:
            return this->registers.Flags.sf==1;
        case PL:    
            return this->registers.Flags.sf==0;
        case VS:
            return this->registers.Flags.of==1;
        case VC:    
            return this->registers.Flags.of==0; 
        case HI:
            return (this->registers.Flags.cf&!this->registers.Flags.zf)==1;
        case LS:    
            return !((this->registers.Flags.cf&!this->registers.Flags.zf)==1);
        case GE:
            return this->registers.Flags.sf==this->registers.Flags.of;
        case LT:    
            return !(this->registers.Flags.sf==this->registers.Flags.of);
        case GT:
            return (this->registers.Flags.zf==1)&&(this->registers.Flags.sf==this->registers.Flags.of);
        case LE:    
            return !((this->registers.Flags.zf==1)&&(this->registers.Flags.sf==this->registers.Flags.of));
        default:
            return true;
        }
    }
    void execute(const int n){
        
        logicalBreakdown_t* logict = (logicalBreakdown_t*) &n;
        if(!shouldExec(logict->condition)){
            printf("NOT executing intruction #%#x\n",(this->registers.pc-1)*4);
            return;
        }
        int result=0;
        if((logict->logicalBranch&0x6)==6){
            printf("COPROC intruction #%#x\n",(this->registers.pc-1)*4);
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
                printf("BRANCh intruction #%#x\n",(this->registers.pc-1)*4);
            }else if(logict->branch==1){
                 printf("BRANCh intruction #%#x\n",(this->registers.pc-1)*4);
            }else if((((MultInstruction*) logict)->opCode==9)&&((logict->branch+logict->logicalBranch)==0)){
                MultInstruction* z=((MultInstruction*) logict);
                printf("MUL intruction #%#x\n",(this->registers.pc-1)*4);
            }else{
                MathHUB(this,n);
                //printf("Executing dsp intruction #%#x\n",(this->registers.pc-1)*4);
                //fprintf(stderr,"oppcode %d\n",((MultInstruction*) logict)->opCode);
            }
            
        }

    }
    void load(int* code,int size){
            this->ResetCLR();
            this->codeSize=size;
            this->base=code;

    } 
    void load(FILE* codeFile){
            this->ResetCLR();
            struct stat filedata;
            fstat(fileno(codeFile),&filedata);
            this->codeSize=filedata.st_size;
            this->base=(int*)malloc(this->codeSize);
            fread_s(this->base,this->codeSize,1,this->codeSize,codeFile);            
    } 
    void exec(){
        printf("Executing ARM binary\n");
            while (this->registers.Flags.hlt!=1 && (this->registers.pc<(codeSize/4))){
                
                this->execute(this->base[this->registers.pc++]);
            }    
    
    }
    
};
} // namespace ARMISA
using namespace ARMISA;
int main(){
    ARMEmulatorHTB* HTB=new ARMEmulatorHTB();
    FILE* binary = fopen("ARM.FLAT.BIN","rb");

    HTB->load(binary);
    fclose(binary);
    printf("Loaded ARM binary\n");
    HTB->exec();
    return 0;
}
