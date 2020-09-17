// dc1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "stdlib.h"
#pragma warning(disable : 4996)

int IMEM[1024];  // IMEM = 4KB
int DMEM[1024];  // DMEM = 4KB
int p; //IMEM[p] IMEM값 올려주는 변수
int PC; //program counter
int IR;
int GPR[32];
int delay;      //delay slot 변수
int delay_pc;
int delay_pc1;
int dp;
char a;

//변수
int im_code;
int offset;
int im_code_f;
unsigned op_code;
unsigned rs_code;
unsigned rt_code;
unsigned rd_code;
unsigned base_code;
unsigned addr_code;
unsigned field_code;
unsigned shamt_code;
unsigned op_code_f;
unsigned rs_code_f;
unsigned rt_code_f;
unsigned shamt_code_f;
unsigned field_code_f;
unsigned rd_code_f;
unsigned addr_code_f;

void code_desperation(unsigned IMEM)
{
	op_code = IMEM >> 26;
	
	if (op_code == 0)         //R-tpye(ADD, SUB, SUBU, AND, MOVZ, MOVN, NOP, NOR, OR, XOR, SLT), SLL, SRA. SRL
	{
		rs_code_f = IMEM << 6;
		rs_code = rs_code_f >> 27;
		rt_code_f = IMEM << 11;
		rt_code = rt_code_f >> 27;
		rd_code_f = IMEM << 16;
		rd_code = rd_code_f >> 27;
		shamt_code_f = IMEM << 21;
		shamt_code = shamt_code_f >> 27;
		field_code_f = IMEM << 26;
		field_code = field_code_f >> 26;
		
		////////////////////////////////////////
		// 1. ADD
		if (field_code == 0x20)		
			GPR[rd_code] = GPR[rs_code] + GPR[rt_code];		
			
		// 2. SUB
		else if (field_code == 0x22)
			GPR[rd_code] = GPR[rs_code] - GPR[rt_code];
			
		// 3. SUBU
		else if (field_code == 0x23)
			GPR[rd_code] = (unsigned)GPR[rs_code] - (unsigned)GPR[rt_code];
			
		// 4. NOR
		else if (field_code == 0x27)
			GPR[rd_code] = ~(GPR[rs_code] | GPR[rt_code]);
			
		// 5. AND
		else if (field_code == 0x24)
			GPR[rd_code] = GPR[rs_code] & GPR[rt_code];
			
		// 6. OR
		else if (field_code == 0x25)
			GPR[rd_code] = GPR[rs_code] | GPR[rt_code];
			
		// 7. XOR
		else if (field_code == 0x26)
			GPR[rd_code] = GPR[rs_code] ^ GPR[rt_code];
			
		// 8. SLT
		else if (field_code == 0x2A)
		{
			if (GPR[rs_code] < GPR[rt_code])
				GPR[rd_code] = 1;
			else
				GPR[rd_code] = 0;
		}

		// 9. MOVZ
		else if (field_code == 0x0A)
		{
			if (GPR[rt_code] == 0)
				GPR[rd_code] = GPR[rs_code];
		}

		// 10. MOVN
		else if (field_code == 0x0B)
		{
			if (GPR[rt_code] != 0)
				GPR[rd_code] = GPR[rs_code];
		}

		// 11. NOP
		else if ((field_code == 0x0) && (rs_code == 0x0) && (rt_code == 0x0) && (rd_code == 0x0) && (shamt_code == 0x0) && (rs_code == 0x0))
			GPR[rd_code] = GPR[rs_code] = GPR[rt_code] = 0;
			
		// 12. SLL
		else if ((rs_code == 0x0) && (field_code == 0x0) )
		{
			int temp = 0;
			temp = GPR[rt_code] << shamt_code;
			GPR[rd_code] = temp;
		}

		// 13. SRA
		else if ((rs_code == 0x0) && (field_code == 0x03))
		{
			int temp;
			int temp1;
			temp = rt_code >> 31;
			if (temp == 1)
			{
				GPR[rt_code] >> shamt_code;
				temp1 = 0xFFFFFFFF << shamt_code;
				GPR[rd_code] = GPR[rt_code] + temp1;
			}
			else if (temp == 0)
				GPR[rd_code] = GPR[rt_code] >> shamt_code;
		}

		// 14. SRL
		else if ((rs_code == 0x0) && (field_code == 0x02))
		{
			int temp = 0;
			temp = GPR[rt_code] >> shamt_code;
			GPR[rd_code] = temp;
		}
	}

	/////////////////////////////////////

	else if (op_code != 0)  // I-type(ADDI, ADDIU, ANDI, LUI, ORI, XORI, SLTI, SLTIU)
	{
		rs_code_f = IMEM << 6;
		rs_code = rs_code_f >> 27;
		rt_code_f = IMEM << 11;
		rt_code = rt_code_f >> 27;
		im_code_f = IMEM << 16;
		im_code = im_code_f >> 16;
		addr_code_f = IMEM << 6;
		addr_code = addr_code_f >> 6;
		base_code = rs_code;
		offset = im_code;
		
		// 15. ADDI
		if (op_code == 0x08)
			GPR[rt_code] = GPR[rs_code] + im_code;
							
		// 16. ADDIU
		else if (op_code == 0x09)
			GPR[rt_code] = (unsigned)GPR[rs_code] + (unsigned)im_code;
			
		// 17. ANDI
		else if (op_code == 0x0C)
			GPR[rt_code] = GPR[rs_code] & im_code;
			
		// 18. LUI
		else if ((op_code == 0x0F) && (rs_code == 0x0))
		{
			int temp = 0;
			temp = im_code << 16;
			GPR[rt_code] = temp;
		}

		// 19. ORI
		else if (op_code == 0x0D)
			GPR[rt_code] = GPR[rs_code] | im_code;
			
		// 20. XORI
		else if (op_code == 0x0E)
			GPR[rt_code] = GPR[rs_code] ^ im_code;
			
		// 21. SLTI
		else if (op_code == 0x0A)
		{
			if (GPR[rs_code] < im_code)
				GPR[rt_code] = 1;
			else
				GPR[rt_code] = 0;
		}

		// 22. SLTIU
		else if (op_code == 0x0B)
		{
			if ((unsigned)GPR[rs_code] < (unsigned)im_code)
				GPR[rt_code] = 1;
			else
				GPR[rt_code] = 0;
		}
		/* 23. LB ////DMEM[base + offset]에 저장되어 있는 값중 하위 8bit를 rt에 입력한다.
				  8bit의 MSB를 확인하여 1이면 앞을 전부 1로 채워주고 0이면 그대로 rt에 입력한다.*/
		else if (op_code == 0x20)
		{
			int temp;
			int temp2;
			temp = (DMEM[base_code + (unsigned)offset] & 0x000000FF);
			temp2 = temp >> 7;
			if (temp2 == 0X01)
				GPR[rt_code] = temp | 0xFFFFFF00;
			else
				GPR[rt_code] = temp;
		}

		// 24. LBU  ////DMEM[base + offset]에 저장되어 있는 값중 하위 8bit를 rt에 입력한다.
		else if (op_code == 0x24)
			GPR[rt_code] = (DMEM[base_code + (unsigned)offset] & 0x000000FF);
			
		/* 25. LH  ////DMEM[base + offset]에 저장되어 있는 값중 하위 16bit를 rt에 입력한다.
				   16bit의 MSB를 확인하여 1이면 앞을 전부 1로 채워주고 0이면 그대로 rt에 입력한다.*/
		else if (op_code == 0x21)
		{
			int temp;
			int temp2;
			temp = (DMEM[base_code + (unsigned)offset] & 0x0000FFFF);
			temp2 = temp >> 15;
			if (temp2 == 0X01)
				GPR[rt_code] = temp | 0xFFFF0000;
			else
				GPR[rt_code] = temp;
		}

		// 26. LHU  ////DMEM[base + offset]에 저장되어 있는 값중 하위 16bit를 rt에 입력한다.
		else if (op_code == 0x25)
			GPR[rt_code] = (DMEM[base_code + (unsigned)offset] & 0x0000FFFF);
			
		// 27. LW  
		else if (op_code == 0x23)
			GPR[rt_code] = DMEM[base_code + (unsigned)offset];
			
		// 28. SB  ///rt의 값 중 하위 8bit를 dmem[base + offset]에 입력한다.
		else if (op_code == 0x28)
			DMEM[base_code + (unsigned)offset] = (GPR[rt_code] & 0x000000FF);
			
		// 29. SH  ///rt의 값 중 하위 16bit를 dmem[base + offset]에 입력한다.
		else if (op_code == 0x29)
			DMEM[base_code + (unsigned)offset] = (GPR[rt_code] & 0x0000FFFF);
			
		// 30. SW  
		else if (op_code == 0x2B)
			DMEM[base_code + (unsigned)offset] = GPR[rt_code];
					
		// 31. BEQ
		else if (op_code == 0x04)
		{
			if (GPR[rs_code] == GPR[rt_code])
			{
				delay_pc = PC;
				PC = PC + (4 * (offset));
				delay_pc1 = PC;
				delay = p + offset;
				dp++;
			}
		}

		// 32. BNE
		else if (op_code == 0x05)
		{
			if (GPR[base_code] != GPR[rt_code])
			{
				delay_pc = PC;
				PC = PC + 4 * (offset);
				delay_pc1 = PC;
				delay = p + offset;
				dp++;
			}
		}

		// 33. J
		else if (op_code == 0x02)
		{
			delay_pc = PC;
			PC = 4 * (addr_code)-4;
			delay_pc1 = PC;
			delay = addr_code - 1;
			dp++;
		}

		// 34. JAL
		else if (op_code == 0x03)
		{
			delay_pc = PC;
			GPR[31] = PC + 8;
			PC = 4 * (addr_code)-4;
			delay_pc1 = PC;
			delay = addr_code - 1;
			dp++;
		}
	}	
}

// STEP BY STEP EXECUTION
void execute_step()
{
	printf("PC: 0x%08X\t", PC);
	printf("IR: 0x%08X\n", IMEM[p]);
	code_desperation(IMEM[p]);
}

// EXECUTE TO END
void execute_run()
{
	printf("PC: 0x%08X\t", PC);
	printf("IR: 0x%08X\n", IMEM[p]);
	code_desperation(IMEM[p]);
}

// READ 1 CHARACTOR (s or r)
char get_cmd()
{
	printf("Insert a command s or r: ");
	a = getchar();
	getchar();

	return a;
}

// DUMP DATA MEMORY
// FROM start_addr (0x00)
// TO   end_addr (0xFF)
void dmem_dump(FILE * fout, int start_addr, int end_addr)
{
	for (int i = start_addr; i < end_addr; i += 4)
		fprintf(fout, "DMEM[0x%02x]=0x%08X DMEM[0x%02x]=0x%08X DMEM[0x%02x]=0x%08X  DMEM[0x%02x]=0x%08X\n", i, DMEM[i], i + 1, DMEM[i + 1], i + 2, DMEM[i + 2], i + 3, DMEM[i + 3]);
}

// PRINT LOGO
void print_logo()
{
	// USE YOUR STUDENT ID & NAME
	printf("*******************************************\n");
	printf("*                                         *\n");
	printf("*  Simple MIPS Instruction Set Simulator  *\n");
	printf("*                                         *\n");
	printf("*                     201401815 권순용    *\n");
	printf("*                     201401867 정봉민    *\n");
	printf("*******************************************\n");

	printf("\n");
}

// PRINT 1 LINE
void print_line()
{
	printf("-------------------------------------------\n");
}
// PRINT STATUS (PC, IR, PS, GPR)
void print_status()
{
	int i;

	printf("\nGPR:\n");
	for (i = 0; i<32; i++)
	{
		printf("[%02d] %08X", i, GPR[i]);
		if (i % 8<7)
			printf(" ");
		else
			printf("\n");
	}
}

// INTIALIZE PROCESSOR
void initialize()
{
	int i;

	PC = 0;
	IR = 0;

	for (i = 0; i<32; i++)
	{
		GPR[i] = 0;
	}

	// STACK POINTER
	GPR[29] = 0x4000;
}

// DETERMINE WHETHER PROGRAM REACHED END
int is_prg_end(int p, int counter)
{
	if (p == counter - 1)  //REACHED THE END
		return 1;
	else if (p < counter - 1)  //OTHERWISE
		return 0;
}

// MAIN FUNCTION
int main(int argc, char *argv[])
{
	int counter = 1; //step by step 멈추기 위한 변수
	int dump_start_addr = 0x00;
	int dump_end_addr = 0xFF;
	print_logo();
	initialize();

	// CHECK INPUT PARAMETERS FORMAT
	if (argc != 3)
	{
		printf("\nProgram Argument Error\n");
		printf("Usage: mipsiss [input_file] [output_file]\n");

		system("PAUSE");
		exit(-1);
	}

	// READ IN TEST VECTOR
	// and INITIALIZE IMEM
	FILE *fin = fopen(argv[1], "rt");
	while (1)
	{
		if (feof(fin) != 0)
			break;
		fscanf(fin, "%x", &IMEM[p]);
		counter++;
		p++;
	}
	
	p = 0;

	// MAIN LOOP
	while (1)
	{
		while (p < counter - 1)
		{
			get_cmd();

			// INPUT == 's'
			if (a == 's')  //step by step
			{
				IR = IMEM[p];
				execute_step();
				print_status();
				print_line();
				printf("\n");
				if (dp > 0)   //Delay Slot
				{
					p += 1;
					PC = delay_pc + 4;
					get_cmd();
					if (a == 's')
					{
						execute_step();
						print_status();
						print_line();
						PC = delay_pc1;
						p = delay;
						dp = 0;
					}
					else
					{
						printf("\nInput Command Format Error\n");
						printf("s  -  Step\n");
						printf("r  -  Run\n");

						system("PAUSE");
						exit(-1);
					}
				}
				PC += 4;
				p++;
			}
			else if (a == 'r')  //run
			{
				while (p < counter - 1)
				{
					IR = IMEM[p];
					execute_run();
					if (dp > 0)   //Delay Slot
					{
						p += 1;
						PC = delay_pc + 4;
						execute_run();
						PC = delay_pc1;
						p = delay;
						dp = 0;
					}
					PC += 4;
					p++;
				}
				print_status();
				print_line();
				printf("\n");
			}

			else if (a != 's' || a != 'r')
			{
				printf("\nInput Command Format Error\n");
				printf("s  -  Step\n");
				printf("r  -  Run\n");

				system("PAUSE");
				exit(-1);
			}

		}
		// DETERMINE WHETHER PROGRAM REACHED END
		if (is_prg_end(p, counter))
			break;
	}

	printf("FINISH!\n\n");
	fclose(fin);

	// AFTER END OF PROGRAM
	// DUMP DATA MEMORY
	// FROM 0x00 TO 0xFF
	FILE *fout = fopen(argv[2], "wt");
	
	dmem_dump(fout, dump_start_addr, dump_end_addr);
	fclose(fout);
		
	system("PAUSE");
	return 0;
}
