#ifndef _SRAM_HEADER_H_
#define _SRAM_HEADER_H_

/////////////////////////////////////////////////
// Posisiones
#define SRAM_CONTROL_POS_VALUE		    0x0000
#define SRAM_NUM_EXECUTIONS_POS_VALUE	0x0002
#define SRAM_RECORDS_TABLE_POS		    0x0004

////////////////////////////////////////////////
// valores
#define SRAM_CONTROL    0xA130F1
#define SRAM_BASE       0x200001

#define SRAM_CONTROL_VALUE			    3011

#define NUM_REGISTERS_RECORDS		    10
#define SIZE_RECORDS_REGISTER			16	// (char * 9 + long + u8 = 16 bytes por registro)

//////////////////////////////////////////////
// globales
u16 num_executions;

/////////////////////////////////////////////
// funciones
u16 SRAM_readWord(u32 offset);	// Read a word from the SRAM.
u32 SRAM_readLong(u32 offset);	// Read a long from the SRAM.
void SRAM_writeWord(u32 offset, u16 val);  // Write a word to the SRAM.
void SRAM_writeLong(u32 offset, u32 val); // Write a long to the SRAM.

u8 initSRAM();
void writeRecordsTableSRAM();
void readRecordsTableSRAM();

void SRAM_enable();
void SRAM_disable();

///////////////////////////////////////////
// macros
#define writeNumExecutionsSRAM(); \
	SRAM_writeWord(SRAM_NUM_EXECUTIONS_POS_VALUE, num_executions);

#define loadNumExecutionsSRAM(); \
	num_executions = SRAM_readWord(SRAM_NUM_EXECUTIONS_POS_VALUE);

#define writeControlValueSRAM(); \
	SRAM_writeWord(SRAM_CONTROL_POS_VALUE, SRAM_CONTROL_VALUE);

////////////////////////////////////////////////////////////////////////////////////
// definiciones de sram propias para el juego
////////////////////////////////////////////////////////////////////////////////////
// tamaño de la cadena para los nombres de la pantalla de records
#define GD_MAX_NAME_LEN        9

////////////////////////////////////////////////////////////////////////////////////
// Game data struct, arranged like this:
//
//   0    1    2    3    4     5    6      14  15
//  ___________________________________//_________
// |    |    |    |    |    |    |           |    |
// | ID |       SCORE       | ST |    NAME   | CS |
// |____|____|____|____|____|____|_____//____|____|
//
////////////////////////////////////////////////////////////////////////////////////
typedef union{

    u8 data[16];                        // Raw data
    struct{
        u8 id;                          // Game ID
        u8 score[4];                    // Score (24 bits)
        u8 st;                          // Stage
        char name[GD_MAX_NAME_LEN];     // Player name
        u8 cs;                          // Checksum
    };
}game_data;

// puntuacion
u32 pointsTable[3][10];
char namesTable[3][10][GD_MAX_NAME_LEN+1];
u8 ranksTable[3][10];

////////////////////////////////////////////////////////////////////////////////////
// logros
typedef union{

    u32 achievements;
    struct{

        u8 achiev1:1;
        u8 achiev2:1;
        u8 achiev3:1;
        u8 achiev4:1;
        u8 achiev5:1;
        u8 achiev6:1;
        u8 achiev7:1;
        u8 achiev8:1;
        u8 achiev9:1;
        u8 achiev10:1;
        u8 achiev11:1;
        u8 achiev12:1;
        u8 achiev13:1;
        u8 achiev14:1;
        u8 achiev15:1;
        u8 achiev16:1;
        u8 achiev17:1;
        u8 achiev18:1;
        u8 achiev19:1;
        u8 achiev20:1;
        u8 achiev21:1;
        u8 achiev22:1;
        u8 achiev23:1;
        u8 achiev24:1;
        u8 achiev25:1;
        u8 achiev26:1;
        u8 achiev27:1;
        u8 achiev28:1;
        u8 achiev29:1;
        u8 achiev30:1;
        u8 achiev31:1;
        u8 achiev32:1;
    };
}achievData;

// Datos de logros
achievData ad;

#endif
