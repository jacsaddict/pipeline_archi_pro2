#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

char   ID_inst[6] = "NOP",
       EX_inst[6] = "NOP",
       DM_inst[6] = "NOP",
       WB_inst[6] = "NOP";

int IF_ID[32];
int ID_EX[32];
int EX_DM[32];
int DM_WB[32];

int EX_DM_M_index;
int DM_WB_M;
int computing_instruction_in_DM = 0;
int computing_instruction_in_WB = 0;

int halt_counting;

bool _flush;
int stall_IF;
int stall_ID;
int pc_adjust;

//R-type
unsigned int instruction_pipel[5];
unsigned int opcode_pipel[5];
int rs_pipel[5];
int rt_pipel[5];
int rd_pipel[5];
unsigned int shamt_pipel[5];
unsigned int funct_pipel[5];
//I-type
short immediate_pipel[5];
unsigned short unsigned_immediate_pipel[5];
//J-Type
unsigned int address_pipel[5];


int pre_inst;

unsigned int instruction_decode(unsigned char ,unsigned char ,unsigned char ,unsigned char );

int simulator(unsigned int,unsigned int,unsigned int,
              unsigned int,unsigned int,unsigned int, short,unsigned short,
              unsigned int,int[],unsigned int*,int*,int*);


int write_0 = 0;
int overflow = 0;
int overwrite_hi_lo = 0;
int memory_address_overflow = 0;
int data_misaligned = 0;
int no_load_with_hi_lo = 0;
int halt_ = 0;
int J_Type = 0;
int pre_HI,pre_pre_HI;
int pre_LO,pre_pre_LO;
unsigned char _iimage[1024] = {0};
unsigned char _dimage[1024] = {0};


int  main()
{
  FILE *iimage = fopen("iimage.bin","rb");
  FILE *dimage = fopen("dimage.bin","rb");
  FILE *error = fopen("error_dump2.rpt","w");
  FILE *snapshot = fopen("snapshot2.rpt","w");

  int reg[32] = {0};
  int reg_pre[32] = {0};
  int reg_pre_pre[32] = {0};
  int HI = 0;
  int LO = 0;
  unsigned int pc;
  unsigned int pc_pre;
  unsigned int pc_init;
  unsigned int sp;
  unsigned int _iimage_num = 0;
  unsigned int _dimage_num = 0;

  //loading iimage
  fread(_iimage,sizeof(unsigned char),4,iimage);
  pc = ((unsigned int)_iimage[0]<<24) + ((unsigned int)_iimage[1]<<16) + ((unsigned int)_iimage[2]<<8) + (unsigned int)_iimage[3];
  pc_init = pc;
  pc_pre = pc;
  fread(_iimage,sizeof(unsigned char),4,iimage);
  _iimage_num = ((unsigned int)_iimage[0]<<24) + ((unsigned int)_iimage[1]<<16) +((unsigned int)_iimage[2]<<8) + (unsigned int)_iimage[3];
  fread(_iimage,sizeof(unsigned char),4*_iimage_num,iimage);

  //loading dimage
  fread(_dimage,sizeof(unsigned char),4,dimage);
  sp =  (_dimage[0]<<24) + (_dimage[1]<<16) + (_dimage[2]<<8) + _dimage[3];
  reg[29] = sp;
  reg_pre[29] = sp;
  reg_pre_pre[29] = sp;
  fread(_dimage,sizeof(unsigned char),4,dimage);
  _dimage_num =  (_dimage[0]<<24) + (_dimage[1]<<16) + (_dimage[2]<<8) + _dimage[3];
  fread(_dimage,sizeof(unsigned char),4*_dimage_num,dimage);

  int i=0;
  int cycle = 0;
  while (1) {
    /* code */
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //stage_WB
    //TODO write_0 overwrite_hi_lo  //write_0 needed in mfhi mflo
    if(instruction_pipel[4]!=0)
    {
      switch (opcode_pipel[4]) {
        //S-type
        case 0x3F:
        {//halt
          strcpy(WB_inst,"HALT");
          break;
        }
        //R-type
        case 0x00:
        {
          switch (funct_pipel[4]) {
            case 0x20:
            {//add
              strcpy(WB_inst,"ADD");

              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x21:
            {//addu
              strcpy(WB_inst,"ADDU");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x22:
            {//sub
              strcpy(WB_inst,"SUB");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x24:
            {//and
              strcpy(WB_inst,"AND");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x25:
            {//or
              strcpy(WB_inst,"OR");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x26:
            {//xor
              strcpy(WB_inst,"XOR");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x27:
            {//nor
              strcpy(WB_inst,"NOR");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x28:
            {//nand
              strcpy(WB_inst,"NAND");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x2A:
            {//slt
              strcpy(WB_inst,"SLT");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x00:
            {//sll
              strcpy(WB_inst,"SLL");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x02:
            {//srl
              strcpy(WB_inst,"SRL");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x03:
            {//sra
              strcpy(WB_inst,"SRA");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x08:
            {//jr
              strcpy(WB_inst,"JR");
              break;
            }
            case 0x18:
            {//mult
              strcpy(WB_inst,"MULT");
              break;
            }
            case 0x19:
            {//multu
              strcpy(WB_inst,"MULTU");
              break;
            }
            case 0x10:
            {//mfhi
              strcpy(WB_inst,"MFHI");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
            case 0x12:
            {//mflo
              strcpy(WB_inst,"MFLO");
              reg[rd_pipel[4]] = DM_WB[rd_pipel[4]];
              if(rd_pipel[4]==0)
              {
                reg[rd_pipel[4]] = 0;
                write_0 = 1;
              }
              break;
            }
          }
          break;
        }
        case 0x08:
        {//addi
          strcpy(WB_inst,"ADDI");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x09:
        {//addiu
          strcpy(WB_inst,"ADDIU");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x23:
        {//lw
          strcpy(WB_inst,"LW");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x21:
        {//lh
          strcpy(WB_inst,"LH");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x25:
        {//lhu
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          strcpy(WB_inst,"LHU");
          break;
        }
        case 0x20:
        {//lb
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          strcpy(WB_inst,"LB");
          break;
        }
        case 0x24:
        {//lbu
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          strcpy(WB_inst,"LBU");
          break;
        }
        case 0x2B:
        {//sw
          strcpy(WB_inst,"SW");
          break;
        }
        case 0x29:
        {//sh
          strcpy(WB_inst,"SH");
          break;
        }
        case 0x28:
        {//sb
          strcpy(WB_inst,"SB");
          break;
        }
        case 0x0F:
        {//lui
          strcpy(WB_inst,"LUI");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x0C:
        {//andi
          strcpy(WB_inst,"ANDI");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x0D:
        {//ori
          strcpy(WB_inst,"ORI");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x0E:
        {//nori
          strcpy(WB_inst,"NORI");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x0A:
        {//slti
          strcpy(WB_inst,"SLTI");
          reg[rt_pipel[4]] = DM_WB[rt_pipel[4]];
          if(rt_pipel[4]==0)
          {
            reg[rt_pipel[4]] = 0;
            write_0 = 1;
          }
          break;
        }
        case 0x04:
        {//beq
          strcpy(WB_inst,"BEQ");
          break;
        }
        case 0x05:
        {//bne
          strcpy(WB_inst,"BNE");
          break;
        }
        case 0x07:
        {//bgtz
          strcpy(WB_inst,"BGTZ");
          break;
        }
        //J-type
        case 0x02:
        {//j
          strcpy(WB_inst,"J");
          break;
        }
        case 0x03:
        {//jal
          strcpy(WB_inst,"JAL");
          reg[31] = DM_WB[31];
          break;
        }
      }
    }else{
      strcpy(WB_inst,"NOP");
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //stage_DM
    //TODO data_misaligned memory_address_overflow

    if(instruction_pipel[3]!=0){
        switch (opcode_pipel[3]) {
          //S-type
          case 0x3F:
          {//halt
            strcpy(DM_inst,"HALT");
            break;
          }
          //R-type
          case 0x00:
          {
            switch (funct_pipel[3]) {
              case 0x20:
              {//add
                strcpy(DM_inst,"ADD");
                break;
              }
              case 0x21:
              {//addu
                strcpy(DM_inst,"ADDU");
                break;
              }
              case 0x22:
              {//sub
                strcpy(DM_inst,"SUB");
                break;
              }
              case 0x24:
              {//and
                strcpy(DM_inst,"AND");
                break;
              }
              case 0x25:
              {//or
                strcpy(DM_inst,"OR");
                break;
              }
              case 0x26:
              {//xor
                strcpy(DM_inst,"XOR");
                break;
              }
              case 0x27:
              {//nor
                strcpy(DM_inst,"NOR");
                break;
              }
              case 0x28:
              {//nand
                strcpy(DM_inst,"NAND");
                break;
              }
              case 0x2A:
              {//slt
                strcpy(DM_inst,"SLT");
                break;
              }
              case 0x00:
              {//sll
                strcpy(DM_inst,"SLL");
                break;
              }
              case 0x02:
              {//srl
                strcpy(DM_inst,"SRL");
                break;
              }
              case 0x03:
              {//sra
                strcpy(DM_inst,"SRA");
                break;
              }
              case 0x08:
              {//jr
                strcpy(DM_inst,"JR");
                break;
              }
              case 0x18:
              {//mult
                strcpy(DM_inst,"MULT");
                break;
              }
              case 0x19:
              {//multu
                strcpy(DM_inst,"MULTU");
                break;
              }
              case 0x10:
              {//mfhi
                strcpy(DM_inst,"MFHI");
                break;
              }
              case 0x12:
              {//mflo
                strcpy(DM_inst,"MFLO");
                break;
              }
            }
            break;
          }
          case 0x08:
          {//addi
            strcpy(DM_inst,"ADDI");
            break;
          }
          case 0x09:
          {//addiu
            strcpy(DM_inst,"ADDIU");
            break;
          }
          case 0x23:
          {//lw
            strcpy(DM_inst,"LW");
            int m_halt = 0;
            if(EX_DM_M_index>=1021||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(EX_DM_M_index%4!=0)
            {
              data_misaligned = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              EX_DM[rt_pipel[3]] =(int)(( _dimage[EX_DM_M_index]<<24)+(_dimage[EX_DM_M_index+1]<<16)+(_dimage[EX_DM_M_index+2]<<8)+(_dimage[EX_DM_M_index+3]));
            }
            break;
          }
          case 0x21:
          {//lh
            strcpy(DM_inst,"LH");
            int m_halt = 0;
            if(EX_DM_M_index>=1023||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(EX_DM_M_index%2!=0)
            {
              data_misaligned = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              EX_DM[rt_pipel[3]] =(short)((_dimage[EX_DM_M_index]<<8)+(_dimage[EX_DM_M_index+1]));
            }
            break;
          }
          case 0x25:
          {//lhu
            strcpy(DM_inst,"LHU");
            int m_halt = 0;
            if(EX_DM_M_index>=1023||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(EX_DM_M_index%2!=0)
            {
              data_misaligned = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              EX_DM[rt_pipel[3]] =(unsigned short)((_dimage[EX_DM_M_index]<<8)+(_dimage[EX_DM_M_index+1]));
            }
            break;
          }
          case 0x20:
          {//lb
            strcpy(DM_inst,"LB");
            int m_halt = 0;
            if(EX_DM_M_index>=1024||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              EX_DM[rt_pipel[3]] =(char)_dimage[EX_DM_M_index];
            }
            break;
          }
          case 0x24:
          {//lbu
            strcpy(DM_inst,"LBU");
            int m_halt = 0;
            if(EX_DM_M_index>=1024||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              EX_DM[rt_pipel[3]] =(unsigned char)_dimage[EX_DM_M_index];
            }
            break;
          }
          case 0x2B:
          {//sw
            strcpy(DM_inst,"SW");
            int m_halt = 0;
            if(EX_DM_M_index>=1021||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(EX_DM_M_index%4!=0)
            {
              data_misaligned = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              _dimage[EX_DM_M_index]   = DM_WB[rt_pipel[3]]>>24;
              _dimage[EX_DM_M_index+1] = DM_WB[rt_pipel[3]]>>16;
              _dimage[EX_DM_M_index+2] = DM_WB[rt_pipel[3]]>>8;
              _dimage[EX_DM_M_index+3] = DM_WB[rt_pipel[3]];
            }
            break;
          }
          case 0x29:
          {//sh
            strcpy(DM_inst,"SH");
            int m_halt = 0;
            if(EX_DM_M_index>=1023||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(EX_DM_M_index%2!=0)
            {
              data_misaligned = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              _dimage[EX_DM_M_index]   = DM_WB[rt_pipel[3]]>>8;
              _dimage[EX_DM_M_index+1] = DM_WB[rt_pipel[3]];
            }
            break;
          }
          case 0x28:
          {//sb
            strcpy(DM_inst,"SB");
            int m_halt = 0;
            if(EX_DM_M_index>=1024||EX_DM_M_index<0)
            {
              memory_address_overflow = 1;
              m_halt = 1;
            }
            if(!m_halt)
            {
              _dimage[EX_DM_M_index] = DM_WB[rt_pipel[3]];
            }
            break;
          }
          case 0x0F:
          {//lui
            strcpy(DM_inst,"LUI");
            break;
          }
          case 0x0C:
          {//andi
            strcpy(DM_inst,"ANDI");
            break;
          }
          case 0x0D:
          {//ori
            strcpy(DM_inst,"ORI");
            break;
          }
          case 0x0E:
          {//nori
            strcpy(DM_inst,"NORI");
            break;
          }
          case 0x0A:
          {//slti
            strcpy(DM_inst,"SLTI");
            break;
          }
          case 0x04:
          {//beq
            strcpy(DM_inst,"BEQ");
            break;
          }
          case 0x05:
          {//bne
            strcpy(DM_inst,"BNE");
            break;
          }
          case 0x07:
          {//bgtz
            strcpy(DM_inst,"BGTZ");
            break;
          }
          //J-type
          case 0x02:
          {//j
            strcpy(DM_inst,"J");
            break;
          }
          case 0x03:
          {//jal
            strcpy(DM_inst,"JAL");
            break;
          }
        }
    }else{
      strcpy(DM_inst,"NOP");
    }



    //DM_WB_M remember! to check

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //stage_EX
    //TODO fwd rd != 0
    int to_be_fwd_EX_rs = 0;
    int to_be_fwd_EX_rt = 0;
    int m_to_be_fwd_EX_rs = 0;
    int m_to_be_fwd_EX_rt = 0;
    int MUX_ID_EX_rs = ID_EX[rs_pipel[2]];
    int MUX_ID_EX_rt = ID_EX[rt_pipel[2]];
    // printf(" %d", rt_pipel[3]);
    // printf(" %d", rs_pipel[2]);
    // printf(" %d\n", rt_pipel[2]);

    if((rd_pipel[4]!=0&&rs_pipel[2]==rd_pipel[4]&&computing_instruction_in_WB==1))
    {
      m_to_be_fwd_EX_rs = rd_pipel[4];
      MUX_ID_EX_rs = DM_WB[rd_pipel[4]];
    }
    if((rt_pipel[4]!=0&&rs_pipel[2]==rt_pipel[4]&&computing_instruction_in_WB==2))
    {
      m_to_be_fwd_EX_rs = rt_pipel[4];
      MUX_ID_EX_rs = DM_WB[rt_pipel[4]];
    }

    if((rd_pipel[3]!=0&&rs_pipel[2]==rd_pipel[3]&&computing_instruction_in_DM==1))
    {
      m_to_be_fwd_EX_rs = 0;
      to_be_fwd_EX_rs = rd_pipel[3];
      MUX_ID_EX_rs = EX_DM[rd_pipel[3]];
    }
    if((rt_pipel[3]!=0&&rs_pipel[2]==rt_pipel[3]&&computing_instruction_in_DM==2))
    {
      m_to_be_fwd_EX_rs = 0;
      to_be_fwd_EX_rs = rt_pipel[3];
      MUX_ID_EX_rs = EX_DM[rt_pipel[3]];
    }//TODO:::::::::::　else if  DM to EX

    if((rd_pipel[4]!=0&&rt_pipel[2]==rd_pipel[4]&&computing_instruction_in_WB==1))
    {
      m_to_be_fwd_EX_rt = rd_pipel[4];
      MUX_ID_EX_rt = DM_WB[rd_pipel[4]];
    }
    if((rt_pipel[4]!=0&&rt_pipel[2]==rt_pipel[4]&&computing_instruction_in_WB==2))
    {
      m_to_be_fwd_EX_rt = rt_pipel[4];
      MUX_ID_EX_rt = DM_WB[rt_pipel[4]];
    }
    if((rd_pipel[3]!=0&&rt_pipel[2]==rd_pipel[3]&&computing_instruction_in_DM==1))
    {
      m_to_be_fwd_EX_rt = 0;
      to_be_fwd_EX_rt = rd_pipel[3];
      MUX_ID_EX_rt = EX_DM[rd_pipel[3]];
    }
    if((rt_pipel[3]!=0&&rt_pipel[2]==rt_pipel[3]&&computing_instruction_in_DM==2))
    {
      m_to_be_fwd_EX_rt = 0;
      to_be_fwd_EX_rt = rt_pipel[3];
      MUX_ID_EX_rt = EX_DM[rt_pipel[3]];
    }//TODO:::::::::::　else if  DM to EX
    int computing_instruction = 0;
    if(instruction_pipel[2]!=0)
    {
      //TODO record computing_instruction  1->rd  2->rt
      switch (opcode_pipel[2]) {
        //S-type
        case 0x3F:
        {//halt
          strcpy(EX_inst,"HALT");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          break;
        }
        //R-type
        case 0x00:
        {
          switch (funct_pipel[2]) {
            case 0x20:
            {//add
              strcpy(EX_inst,"ADD");
              int tmp1 = MUX_ID_EX_rs;
              int tmp2 = MUX_ID_EX_rt;
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rs + MUX_ID_EX_rt;

              if((tmp1>0&&tmp2>0&&EX_DM[rd_pipel[2]]<=0)||(tmp1<0&&tmp2<0&&EX_DM[rd_pipel[2]]>=0))
                overflow = 1;
              computing_instruction = 1;
              break;
            }
            case 0x21:
            {//addu
              strcpy(EX_inst,"ADDI");
                ID_EX[rd_pipel[2]] = MUX_ID_EX_rs + MUX_ID_EX_rt;

                computing_instruction = 1;
                break;
            }
            case 0x22:
            {//sub
              strcpy(EX_inst,"SUB");
              int tmp1 = MUX_ID_EX_rs;
              int tmp2 = -MUX_ID_EX_rt;
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rs - MUX_ID_EX_rt;

              if((tmp1>0&&tmp2>0&&EX_DM[rd_pipel[2]]<=0)||(tmp1<0&&tmp2<0&&EX_DM[rd_pipel[2]]>=0))
                overflow = 1;
              computing_instruction = 1;
              break;
            }
            case 0x24:
            {//and
              strcpy(EX_inst,"AND");
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rs & MUX_ID_EX_rt;

              computing_instruction = 1;
              break;
            }
            case 0x25:
            {//or
              strcpy(EX_inst,"OR");
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rs | MUX_ID_EX_rt;

              computing_instruction = 1;
              break;
            }
            case 0x26:
            {//xor
              strcpy(EX_inst,"XOR");
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rs ^ MUX_ID_EX_rt;

              computing_instruction = 1;
              break;
            }
            case 0x27:
            {//nor
              strcpy(EX_inst,"XOR");
              ID_EX[rd_pipel[2]] =~(MUX_ID_EX_rs | MUX_ID_EX_rt);

              computing_instruction = 1;
              break;
            }
            case 0x28:
            {//nand
              strcpy(EX_inst,"NAND");
              ID_EX[rd_pipel[2]] =~(MUX_ID_EX_rs & MUX_ID_EX_rt);

              computing_instruction = 1;
              break;
            }
            case 0x2A:
            {//slt
              strcpy(EX_inst,"SLT");
              ID_EX[rd_pipel[2]] = (MUX_ID_EX_rs < MUX_ID_EX_rt);

              computing_instruction = 1;
              break;
            }
            case 0x00:
            {//sll TODO change!!
              strcpy(EX_inst,"SLL");
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rt<<shamt_pipel[2];
              to_be_fwd_EX_rs = 0;
              m_to_be_fwd_EX_rs = 0;
              computing_instruction = 1;
              break;
            }
            case 0x02:
            {//srl
              strcpy(EX_inst,"SRL");
              ID_EX[rd_pipel[2]] = (unsigned)MUX_ID_EX_rt>>shamt_pipel[2];
              to_be_fwd_EX_rs = 0;
              m_to_be_fwd_EX_rs = 0;
              computing_instruction = 1;
              break;
            }
            case 0x03:
            {//sra
              strcpy(EX_inst,"SRA");
              ID_EX[rd_pipel[2]] = MUX_ID_EX_rt>>shamt_pipel[2];
              to_be_fwd_EX_rs = 0;
              m_to_be_fwd_EX_rs = 0;
              computing_instruction = 1;
              break;
            }
            case 0x08:
            {//jr
              strcpy(EX_inst,"JR");
              to_be_fwd_EX_rt = 0;
              m_to_be_fwd_EX_rt = 0;
              break;
            }
            case 0x18:
            {//mult
              strcpy(EX_inst,"MULT");
              int tmp1 = MUX_ID_EX_rs;
              int tmp2 = MUX_ID_EX_rt;
              long long int product = 0;
              product = (long long int)MUX_ID_EX_rs * (long long int)MUX_ID_EX_rt;
              if((tmp1>0&&tmp2>0&&product<=0)||(tmp1<0&&tmp2<0&&product<=0)||
                 (tmp1<0&&tmp2>0&&product>=0)||(tmp1>0&&tmp2<0&&product>=0))
                {overflow = 1;}
              if(no_load_with_hi_lo)
                {overwrite_hi_lo = 1;}
              int temp = product>>32;
              long long int temp2 = product<<32;
              temp2 = temp2>>32;
              no_load_with_hi_lo = 1;
              HI = temp;
              LO = temp2;
              break;
            }
            case 0x19:
            {//multu
              strcpy(EX_inst,"MULTU");
              unsigned long long int product;
              product = (unsigned long long int)(unsigned int)MUX_ID_EX_rs * (unsigned long long int)(unsigned int)MUX_ID_EX_rt;
              unsigned int temp = product>>32;
              if(no_load_with_hi_lo)
                {overwrite_hi_lo = 1;}
              no_load_with_hi_lo = 1;
              HI = temp;
              LO = (unsigned)product;
              break;
            }
            case 0x10:
            {//mfhi
              strcpy(EX_inst,"MFHI");
              ID_EX[rd_pipel[2]] = HI;
              to_be_fwd_EX_rs = 0;
              m_to_be_fwd_EX_rs = 0;
              to_be_fwd_EX_rt = 0;
              m_to_be_fwd_EX_rt = 0;
              computing_instruction = 1;
              no_load_with_hi_lo = 0;
              break;
            }
            case 0x12:
            {//mflo
              strcpy(EX_inst,"MFLO");
              ID_EX[rd_pipel[2]] = LO;
              to_be_fwd_EX_rs = 0;
              m_to_be_fwd_EX_rs = 0;
              to_be_fwd_EX_rt = 0;
              m_to_be_fwd_EX_rt = 0;
              computing_instruction = 1;
              no_load_with_hi_lo = 0;
              break;
            }
          }
          break;
        }
        case 0x08:
        {//addi
          strcpy(EX_inst,"ADDI");
          int tmp1 = MUX_ID_EX_rs;
          ID_EX[rt_pipel[2]] = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((tmp1>=0&&immediate_pipel[2]>=0&&ID_EX[rt_pipel[2]]<0)||(tmp1<0&&immediate_pipel[2]&&ID_EX[rt_pipel[2]]>=0))
            overflow = 1;
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          computing_instruction = 2;
          break;
        }
        case 0x09:
        {//addiu
          strcpy(EX_inst,"ADDIU");
          ID_EX[rt_pipel[2]] = MUX_ID_EX_rs + (int)immediate_pipel[2];
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          computing_instruction = 2;
          break;
        }
        case 0x23:
        {//lw
          strcpy(EX_inst,"LW");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          computing_instruction = 2;
          EX_DM_M_index = index;
          break;
        }
        case 0x21:
        {//lh
          strcpy(EX_inst,"LH");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          computing_instruction = 2;
          EX_DM_M_index = index;
          break;
        }
        case 0x25:
        {//lhu
          strcpy(EX_inst,"LHU");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          computing_instruction = 2;
          EX_DM_M_index = index;
          break;
        }
        case 0x20:
        {//lb
          strcpy(EX_inst,"LB");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          computing_instruction = 2;
          EX_DM_M_index = index;
          break;
        }
        case 0x24:
        {//lbu
          strcpy(EX_inst,"LBU");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          computing_instruction = 2;
          EX_DM_M_index = index;
          break;
        }
        case 0x2B:
        {//sw
          strcpy(EX_inst,"SW");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          EX_DM_M_index = index;
          break;
        }
        case 0x29:
        {//sh
          strcpy(EX_inst,"SH");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          EX_DM_M_index = index;
          break;
        }
        case 0x28:
        {//sb
          strcpy(EX_inst,"SB");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          int index = MUX_ID_EX_rs + (int)immediate_pipel[2];
          if((MUX_ID_EX_rs>0&&immediate_pipel[2]>0&&index<=0)||(MUX_ID_EX_rs<0&&immediate_pipel[2]<0&&index>=0))
            overflow = 1;
          EX_DM_M_index = index;
          break;
        }
        case 0x0F:
        {//lui
          strcpy(EX_inst,"LUI");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          ID_EX[rt_pipel[2]] = immediate_pipel[2]<<16;
          computing_instruction = 2;
          break;
        }
        case 0x0C:
        {//andi
          strcpy(EX_inst,"ANDI");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          ID_EX[rt_pipel[2]] = MUX_ID_EX_rs&unsigned_immediate_pipel[2];
          computing_instruction = 2;
          break;
        }
        case 0x0D:
        {//ori
          strcpy(EX_inst,"ORI");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          ID_EX[rt_pipel[2]] = MUX_ID_EX_rs|unsigned_immediate_pipel[2];
          computing_instruction = 2;
          break;
        }
        case 0x0E:
        {//nori
          strcpy(EX_inst,"NORI");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          ID_EX[rt_pipel[2]] = ~(MUX_ID_EX_rs|unsigned_immediate_pipel[2]);
          computing_instruction = 2;
          break;
        }
        case 0x0A:
        {//slti
          strcpy(EX_inst,"SLTI");
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          ID_EX[rt_pipel[2]] = (MUX_ID_EX_rs<unsigned_immediate_pipel[2]);
          computing_instruction = 2;
          break;
        }
        case 0x04:
        {//beq
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          strcpy(EX_inst,"BEQ");
          break;
        }
        case 0x05:
        {//bne
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          strcpy(EX_inst,"BNE");
          break;
        }
        case 0x07:
        {//bgtz
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          strcpy(EX_inst,"BGTZ");
          break;
        }
        //J-type
        case 0x02:
        {//j
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          strcpy(EX_inst,"J");
          break;
        }
        case 0x03:
        {//jal
          to_be_fwd_EX_rt = 0;
          m_to_be_fwd_EX_rt = 0;
          to_be_fwd_EX_rs = 0;
          m_to_be_fwd_EX_rs = 0;
          strcpy(EX_inst,"JAL");
          break;
        }
      }
    }else{
      strcpy(EX_inst,"NOP");
    }



    //index remember! to check

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //stage_ID
    //R-type
    unsigned int opcode = 0;
    unsigned int rs = 0;
    unsigned int rt = 0;
    unsigned int rd = 0;
    unsigned int shamt = 0;
    unsigned int funct = 0;
    //I-type
    short immediate = 0;
    unsigned short unsigned_immediate = 0;
    //J-Type
    unsigned int address;

    opcode = instruction_pipel[1]>>26;
    rs = (unsigned)((instruction_pipel[1]<<6)>>27);
    rt = (unsigned)((instruction_pipel[1]<<11)>>27);
    rd = (unsigned)((instruction_pipel[1]<<16)>>27);
    shamt = (unsigned)((instruction_pipel[1]<<21)>>27);
    funct = (unsigned)((instruction_pipel[1]<<26)>>26);
    if(opcode!=0)
    {
      if(opcode!=2 && opcode!=3)
      {
        immediate = (short)((instruction_pipel[1]<<16)>>16);
        unsigned_immediate = (unsigned short)((unsigned)(instruction_pipel[1]<<16)>>16);
      }
      else
        address = (unsigned)(((unsigned)instruction_pipel[1]<<6)>>6);
    }
    //line into pipelline

    opcode_pipel[1] = opcode;
    rs_pipel[1]     = rs;
    rt_pipel[1]     = rt;
    rd_pipel[1]     = rd;
    shamt_pipel[1]  = shamt;
    funct_pipel[1]  = funct;
    immediate_pipel[1]          = immediate;
    unsigned_immediate_pipel[1] = unsigned_immediate;
    address_pipel[1]            = address;
    ////////////////////////////////////////////////////////////////////////////////////////////////
//TODO Nop...............has other possible value
    //rs_pipel[1] == rd_pipel[2]....fwd
    //rs_pipel[1] == rd_pipel[3]....fwd
    int to_be_fwd_ID_rs = 0;
    int to_be_fwd_ID_rt = 0;
    int jal_store = 0;
    bool jal = 0;
    if(instruction_pipel[1]!=0)
    {
      switch (opcode_pipel[1]) {
        //halt_
        case 0x3F:
        {//halt
          strcpy(ID_inst,"HALT");
          break;
        }
        //R-type
        case 0x00:
        {
          switch (funct_pipel[1]) {
            case 0x20: //add
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"ADD");
              break;
            }
            case 0x21: //addu
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"ADDU");
              break;
            }
            case 0x22: //sub
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"SUB");
              break;
            }
            case 0x24: //and
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"AND");
              break;
            }
            case 0x25: //or
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"OR");
              break;
            }
            case 0x26: //xor
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"XOR");
              break;
            }
            case 0x27: //nor
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"NOR");
              break;
            }
            case 0x28: //nand
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"NAND");
              break;
            }
            case 0x2A: //slt
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"SLT");
              break;
            }
            case 0x00: //sll
            {//TODO change
              strcpy(ID_inst,"SLL");
              break;
            }
            case 0x02: //srl
            {
              if(EX_inst[0]=='L'&& (rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"SRL");
              break;
            }
            case 0x03: //sra
            {
              if(EX_inst[0]=='L'&& (rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"SRA");
              break;
            }
            case 0x08: //jr ******************
            {
              strcpy(ID_inst,"JR");
              if((EX_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[2]))||
                 (DM_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[3])))
                 stall_ID = 1;
              if((computing_instruction==1&&rs_pipel[1]==rd_pipel[2])||
                 (computing_instruction==2&&rs_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              if(!stall_ID)
              {
                J_Type = 1;
              }
              if(!stall_ID&&rs_pipel[1]==rd_pipel[3]&&computing_instruction_in_DM==1)
              {
                to_be_fwd_ID_rs = rs_pipel[1];
                pc_adjust = EX_DM[to_be_fwd_ID_rs];
                _flush = 1;
                break;
              }
              if(!stall_ID&&rs_pipel[1]==rt_pipel[3]&&computing_instruction_in_DM==2)
              {
                to_be_fwd_ID_rs = rs_pipel[1];
                pc_adjust = EX_DM[to_be_fwd_ID_rs];
                _flush = 1;
                break;
              }
              if(!stall_ID)
              {
                pc_adjust = reg[rs_pipel[1]];
                _flush = 1;
              }
              break;
            }
            case 0x18: //mult
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"MULT");
              break;
            }
            case 0x19: //multu
            {
              if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
                stall_ID = 1;
              strcpy(ID_inst,"MULTU");
              break;
            }
            case 0x10: //mfhi
            {
              strcpy(ID_inst,"MFHI");
              break;
            }
            case 0x12: //mflo
            {
              strcpy(ID_inst,"MFLO");
              break;
            }
          }
          break;
        }
        //I-type
        case 0x08://addi
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"ADDI");
          break;
        }
        case 0x09://addiu
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"ADDIU");
          break;
        }
        case 0x23://lw
        {
          if(EX_inst[0]=='L'&& rs_pipel[1]==rt_pipel[2])
            stall_ID = 1;
          strcpy(ID_inst,"LW");
          break;
        }
        case 0x21://lh
        {
          if(EX_inst[0]=='L'&& rs_pipel[1]==rt_pipel[2])
            stall_ID = 1;
          strcpy(ID_inst,"LH");
          break;
        }
        case 0x25://lhu
        {
          if(EX_inst[0]=='L'&& rs_pipel[1]==rt_pipel[2])
            stall_ID = 1;
          strcpy(ID_inst,"LHU");
          break;
        }
        case 0x20://lb
        {
          if(EX_inst[0]=='L'&& rs_pipel[1]==rt_pipel[2])
            stall_ID = 1;
          strcpy(ID_inst,"LB");
          break;
        }
        case 0x24://lbu
        {
          if(EX_inst[0]=='L'&& rs_pipel[1]==rt_pipel[2])
            stall_ID = 1;
          strcpy(ID_inst,"LBU");
          break;
        }
        case 0x2B://sw
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"SW");
          break;
        }
        case 0x29://sh
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"SH");
          break;
        }
        case 0x28://sb
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"SB");
          break;
        }
        case 0x0F://lui
        {
          strcpy(ID_inst,"LUI");
          break;
        }
        case 0x0C://andi
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"ANDI");
          break;
        }
        case 0x0D://ori
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"ORI");
          break;
        }
        case 0x0E://nori
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"NORI");
          break;
        }
        case 0x0A://slti
        {
          if(EX_inst[0]=='L'&& (rs_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"SLTI");
          break;
        }
        //TODO using computing_instruction to DETECT stall
        case 0x04:
        {//beq
          if((EX_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))||
             (DM_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[3]||rt_pipel[1]==rt_pipel[3])))
             stall_ID = 1;
          if((computing_instruction==1&&rs_pipel[1]==rd_pipel[2])||(computing_instruction==1&&rt_pipel[1]==rd_pipel[2])||
             (computing_instruction==2&&rs_pipel[1]==rt_pipel[2])||(computing_instruction==2&&rt_pipel[1]==rt_pipel[2]))
            stall_ID = 1;
          strcpy(ID_inst,"BEQ");
          if(!stall_ID&&((computing_instruction_in_DM==1&&rs_pipel[1]==rd_pipel[3]&&rt_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rs_pipel[1]==rt_pipel[3]&&rt_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rs = rd_pipel[3];
              to_be_fwd_ID_rt = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rs = rt_pipel[3];
              to_be_fwd_ID_rt = rt_pipel[3];
            }
            pc_adjust = 4*immediate_pipel[1];  //sure to be eq.
            _flush =  1;
            break;
          }
          if(!stall_ID&&((computing_instruction_in_DM==1&&rs_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rs_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rs = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rs = rt_pipel[3];
            }
            if(reg[rt_pipel[1]] == EX_DM[to_be_fwd_ID_rs])
            {
              pc_adjust = 4*immediate_pipel[1];
              _flush =  1;
            }
            break;
          }
          if(!stall_ID&&((computing_instruction_in_DM==1&&rt_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rt_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rt = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rt = rt_pipel[3];
            }
            if(reg[rs_pipel[1]] == EX_DM[to_be_fwd_ID_rt])
            {
              pc_adjust = 4*immediate_pipel[1];
              _flush =  1;
            }
            break;
          }
          if(!stall_ID&&reg[rt_pipel[1]] == reg[rs_pipel[1]])
          {
            pc_adjust = 4*immediate_pipel[1];
            _flush = 1;
          }
          break;
        }
        case 0x05:
        {//bne
          if((EX_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[2]||rt_pipel[1]==rt_pipel[2]))||
             (DM_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[3]||rt_pipel[1]==rt_pipel[3])))
             stall_ID = 1;
         if((computing_instruction==1&&rs_pipel[1]==rd_pipel[2])||(computing_instruction==1&&rt_pipel[1]==rd_pipel[2])||
            (computing_instruction==2&&rs_pipel[1]==rt_pipel[2])||(computing_instruction==2&&rt_pipel[1]==rt_pipel[2]))
             stall_ID = 1;
          strcpy(ID_inst,"BNE");
          if(!stall_ID&&((computing_instruction_in_DM==1&&rs_pipel[1]==rd_pipel[3]&&rt_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rs_pipel[1]==rt_pipel[3]&&rt_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rs = rd_pipel[3];
              to_be_fwd_ID_rt = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rs = rt_pipel[3];
              to_be_fwd_ID_rt = rt_pipel[3];
            }
            //sure to be eq.
            break;
          }
          if(!stall_ID&&((computing_instruction_in_DM==1&&rs_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rs_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rs = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rs = rt_pipel[3];
            }
            if(reg[rt_pipel[1]] != EX_DM[to_be_fwd_ID_rs])
            {
              pc_adjust = 4*immediate_pipel[1];
              _flush =  1;
            }
            break;
          }
          if(!stall_ID&&((computing_instruction_in_DM==1&&rt_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rt_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rt = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rt = rt_pipel[3];
            }
            if(reg[rs_pipel[1]] != EX_DM[to_be_fwd_ID_rt])
            {
              pc_adjust = 4*immediate_pipel[1];
              _flush =  1;
            }
            break;
          }
          if(!stall_ID&&reg[rt_pipel[1]] != reg[rs_pipel[1]])
          {
            pc_adjust = 4*immediate_pipel[1];
            _flush = 1;
          }
          break;
        }
        case 0x07:
        {//bgtz
          if((EX_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[2]))||
             (DM_inst[0]=='L'&&(rs_pipel[1]==rt_pipel[3])))
             stall_ID = 1;
             if((computing_instruction==1&&rs_pipel[1]==rd_pipel[2])||
                (computing_instruction==2&&rs_pipel[1]==rt_pipel[2]))
             stall_ID = 1;
          strcpy(ID_inst,"BGTZ");
          if(!stall_ID&&((computing_instruction_in_DM==1&&rs_pipel[1]==rd_pipel[3])||
                         (computing_instruction_in_DM==2&&rs_pipel[1]==rt_pipel[3])))
          {
            if(computing_instruction_in_DM==1)
            {
              to_be_fwd_ID_rs = rd_pipel[3];
            }
            if(computing_instruction_in_DM==2)
            {
              to_be_fwd_ID_rs = rt_pipel[3];
            }
            if(EX_DM[to_be_fwd_ID_rs]>0)
            {
              pc_adjust = 4*immediate_pipel[1];
              _flush =  1;
            }
            break;
          }
          if(!stall_ID&&reg[rs_pipel[1]] > 0)
          {
            //detect_overflow
            if((immediate_pipel[1]>0&&4*immediate_pipel[1]<=0)||(immediate_pipel[1]<0&&4*immediate_pipel[1]>=0))
              overflow = 1;
            pc_adjust = 4*immediate;
            _flush = 1;
          }
          break;
        }
        //J-Type
        case 0x02:
        {//j
          strcpy(ID_inst,"J");
          address_pipel[1]<<=2;
          pc_adjust = pc + 4;
          pc_adjust = (unsigned)pc_adjust>>28;
          pc_adjust<<=28;
          pc_adjust = (unsigned)pc_adjust|address_pipel[1];
          J_Type = 1;
          _flush = 1;
          break;
        }
        case 0x03:
        {//jal
          strcpy(ID_inst,"JAL");
          jal_store = pc_pre + 4;
          pc_adjust = pc + 4;
          address_pipel[1] = address_pipel[1] << 2;
          pc_adjust = (unsigned)pc_adjust>>28;
          pc_adjust<<=28;
          pc_adjust = (unsigned)pc_adjust|address_pipel[1];
          J_Type = 1;
          jal = 1;
          _flush = 1;
          break;
        }
      }
    }else
    {
      //NOP
      strcpy(ID_inst,"NOP");
    }
    instruction_pipel[4] = instruction_pipel[3];
    opcode_pipel[4] = opcode_pipel[3];
    rs_pipel[4] = rs_pipel[3];
    rt_pipel[4] = rt_pipel[3];
    rd_pipel[4] = rd_pipel[3];
    shamt_pipel[4] = shamt_pipel[3];
    funct_pipel[4] = funct_pipel[3];
    immediate_pipel[4] = immediate_pipel[3];
    unsigned_immediate_pipel[4] = unsigned_immediate_pipel[3];
    address_pipel[4] = address_pipel[3];

    computing_instruction_in_WB = computing_instruction_in_DM;
    for(int j=0;j<32;j++)
    {
      DM_WB[j] = EX_DM[j];
    }

    instruction_pipel[3] = instruction_pipel[2];
    opcode_pipel[3] = opcode_pipel[2];
    rs_pipel[3] = rs_pipel[2];
    rt_pipel[3] = rt_pipel[2];
    rd_pipel[3] = rd_pipel[2];
    shamt_pipel[3] = shamt_pipel[2];
    funct_pipel[3] = funct_pipel[2];
    immediate_pipel[3] = immediate_pipel[2];
    unsigned_immediate_pipel[3] = unsigned_immediate_pipel[2];
    address_pipel[3] = address_pipel[2];

    computing_instruction_in_DM = computing_instruction;
    for(int j=0;j<32;j++)
    {
      EX_DM[j] = ID_EX[j];
    }

    if(!stall_ID)
    {
      instruction_pipel[2] = instruction_pipel[1];
      opcode_pipel[2] = opcode_pipel[1];
      rs_pipel[2] = rs_pipel[1];
      rt_pipel[2] = rt_pipel[1];
      rd_pipel[2] = rd_pipel[1];
      shamt_pipel[2] = shamt_pipel[1];
      funct_pipel[2] = funct_pipel[1];
      immediate_pipel[2] = immediate_pipel[1];
      unsigned_immediate_pipel[2] = unsigned_immediate_pipel[1];
      address_pipel[2] = address_pipel[1];
    }
    else
    {
      instruction_pipel[2] = 0;
      opcode_pipel[2] = 0;
      rs_pipel[2] = 0;
      rt_pipel[2] = 0;
      rd_pipel[2] = 0;
      shamt_pipel[2] = 0;
      funct_pipel[2] = 0;
      immediate_pipel[2] = 0;
      unsigned_immediate_pipel[2] = 0;
      address_pipel[2] = 0;
    }

    for(int j=0;j<32;j++)
    {
      ID_EX[j] = reg[j];
    }
    if(jal)
      ID_EX[31] = jal_store;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //stage_IF
    unsigned int instruction = instruction_decode(_iimage[i],_iimage[i+1],_iimage[i+2],_iimage[i+3]);
    if(!stall_ID)
    {
      instruction_pipel[1] = instruction;
    }
    if(_flush)
    {
      instruction_pipel[1] = 0;
    }
    if(stall_ID)
    {
      stall_IF = 1;
    }
    //TODO***NEED HALT*5 DETECT***/
    if(instruction>>26==0x3F&&!_flush)
    {
      halt_counting++;
    }else{
      halt_counting = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(snapshot,"cycle %d\n",cycle);
    if(cycle==0)
    {
      for(int j=0; j<32; j++)
      {
          fprintf(snapshot,"$%02d: 0x%08X\n",j,reg[j]);
      }
      fprintf(snapshot,"$HI: 0x%08X\n",HI);
      fprintf(snapshot,"$LO: 0x%08X\n",LO);
      //fprintf(snapshot,"pc_pre: 0x%08X\n",pc_pre);
      fprintf(snapshot,"PC: 0x%08X\n",pc);
      fprintf(snapshot, "IF: 0x%08X\n",instruction);
      fprintf(snapshot, "ID: %s\n",ID_inst );
      fprintf(snapshot, "EX: %s\n",EX_inst );
      fprintf(snapshot, "DM: %s\n",DM_inst );
      fprintf(snapshot, "WB: %s\n",WB_inst );

    }else{
      for(int j=0;j<32;j++)
      {
        if(reg_pre_pre[j]!=reg_pre[j])
        {
          fprintf(snapshot,"$%02d: 0x%08X\n",j,reg_pre[j]);
        }
        reg_pre_pre[j] = reg_pre[j];
        reg_pre[j] = reg[j];
      }
      if(pre_pre_HI!=pre_HI)
      {
        fprintf(snapshot,"$HI: 0x%08X\n",pre_HI);
      }
      pre_pre_HI = pre_HI;
      pre_HI = HI;
      if(pre_pre_LO!=pre_LO)
      {
        fprintf(snapshot,"$LO: 0x%08X\n",pre_LO);
      }
      pre_pre_LO = pre_LO;
      pre_LO = LO;
      //fprintf(snapshot,"pc_pre: 0x%08X\n",pc_pre);
      fprintf(snapshot,"PC: 0x%08X\n",pc);
      fprintf(snapshot, "IF: 0x%08X",instruction);
        if(_flush)     {fprintf(snapshot, " to_be_flushed");}
        if(stall_IF)   {fprintf(snapshot, " to_be_stalled");}
        fprintf(snapshot, "\n");
      fprintf(snapshot, "ID: %s",ID_inst );
        if(stall_ID)   {fprintf(snapshot, " to_be_stalled");}
        if(to_be_fwd_ID_rs) {fprintf(snapshot, " fwd_EX-DM_rs_$%d", to_be_fwd_ID_rs);}
        if(to_be_fwd_ID_rt) {fprintf(snapshot, " fwd_EX-DM_rt_$%d", to_be_fwd_ID_rt);}
        fprintf(snapshot, "\n");
      fprintf(snapshot, "EX: %s",EX_inst );
        if(to_be_fwd_EX_rs) {fprintf(snapshot, " fwd_EX-DM_rs_$%d", to_be_fwd_EX_rs);}
        if(m_to_be_fwd_EX_rs) {fprintf(snapshot, " fwd_DM-WB_rs_$%d", m_to_be_fwd_EX_rs);}
        if(to_be_fwd_EX_rt) {fprintf(snapshot, " fwd_EX-DM_rt_$%d", to_be_fwd_EX_rt);}
        if(m_to_be_fwd_EX_rt) {fprintf(snapshot, " fwd_DM-WB_rt_$%d", m_to_be_fwd_EX_rt);}
        fprintf(snapshot, "\n");
      fprintf(snapshot, "DM: %s\n",DM_inst );
      fprintf(snapshot, "WB: %s\n",WB_inst );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    cycle++;
    pc_pre = pc;
    if(!J_Type&&!stall_IF&&!_flush)       pc+=4;
    if(!J_Type) pc+= pc_adjust;
    if(J_Type)                      {J_Type = 0; pc = pc_adjust;}
    pc_adjust = 0;
    if(stall_IF)                    {stall_ID = 0; stall_IF = 0;}
    if(_flush)                      {_flush   = 0;}
    to_be_fwd_ID_rs = 0;
    to_be_fwd_EX_rt = 0;
    to_be_fwd_EX_rs = 0;
    to_be_fwd_EX_rt = 0;
    m_to_be_fwd_EX_rs = 0;
    m_to_be_fwd_EX_rt = 0;
    i = pc - pc_init;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    int flag = 0;
    if(write_0)
    {
      fprintf( error , "In cycle %d: Write $0 Error\n", cycle);
      write_0 = 0;
    }
    if(overwrite_hi_lo)
    {
      fprintf(error , "In cycle %d: Overwrite HI-LO registers\n", cycle);
      overwrite_hi_lo = 0;
    }
    if(memory_address_overflow)
    {
      fprintf(error , "In cycle %d: Address Overflow\n", cycle);
      flag = 1;
    }
    if(data_misaligned)
    {
      fprintf(error , "In cycle %d: Misalignment Error\n", cycle);
      flag = 1;
    }
    if (overflow) {
      fprintf( error , "In cycle %d: Number Overflow\n", cycle);
      overflow = 0;
    }
    fprintf(snapshot,"\n\n");
    if(flag==1||halt_==1||halt_counting==5) break;
    //system("pause");
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
  }

  return 0;
}

unsigned int instruction_decode(unsigned char a1,unsigned char a2,unsigned char a3,unsigned char a4)
{
  unsigned int instruction =  ((unsigned int)a1<<24) + ((unsigned int)a2<<16) + ((unsigned int)a3<<8) + (unsigned int)(a4);
  return instruction;
}
//TODO lw........change from DM_WB_M to $t
//TODO using temp registers to store the result value to avoid coflict;
