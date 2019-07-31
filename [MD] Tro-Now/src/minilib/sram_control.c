#include "..\main.h"

void SRAM_enable(){    *(vu8*)SRAM_CONTROL = 1; }
void SRAM_enableRO(){    *(vu8*)SRAM_CONTROL = 3; }
void SRAM_disable(){    *(vu8*)SRAM_CONTROL = 0; }
u8 SRAM_readByte(u32 offset){    return *(vu8*)(SRAM_BASE + (offset * 2)); }
void SRAM_writeByte(u32 offset, u8 val) {    *(vu8*)(SRAM_BASE + (offset * 2)) = val; }

/////////////////////////////////////////////////////////////////
//
// u8 initSRAM(){
// inicializa la SRAM
//
/////////////////////////////////////////////////////////////////
u8 initSRAM(){

	num_executions = 0;

	// Activar el acceso a SRAM
	SRAM_enable();

	// Si ya se ha escrito previamente en la SRAM lugar se cargan
	// los datos en las variables y buffers correspondientes
	if(SRAM_readWord(SRAM_CONTROL_POS_VALUE) == SRAM_CONTROL_VALUE){

		loadNumExecutionsSRAM();	// Numero de veces que se ha ejecutado del juego
		readRecordsTableSRAM();
	}
	else{ // En caso de se primera ejecucion escribir los valores inciales en la SRAM

		writeControlValueSRAM();	// escribimos el valor de control

		initRecords();              // inicia la tabla de records
		writeRecordsTableSRAM();
	}

	// Escribir numero de ejecuciones actualizado
	num_executions++;
	writeNumExecutionsSRAM();	// (macro)

	// Desconectar el accceso a SRAM
	SRAM_disable();

	// Devolver true para indicar que el acceso ha sido correcto
	return 1;
}

/////////////////////////////////////////////////////////////////
//
// void writeRecordsTableSRAM(){
// escribe la tabla de records en la sram
//
/////////////////////////////////////////////////////////////////
void writeRecordsTableSRAM(){

	u8 i, j, c;	// Indices

    for( j = 0; j < 3; j++ ){
        for( i = 0; i < NUM_REGISTERS_RECORDS; i++){

            // Escribir nombres
            for( c = 0; c < GD_MAX_NAME_LEN+1; c++ )
                SRAM_writeByte(SRAM_RECORDS_TABLE_POS + (i * SIZE_RECORDS_REGISTER) + c + (SIZE_RECORDS_REGISTER*15*j), namesTable[j][i][c]);

            // Escribir puntuacion
            SRAM_writeLong(SRAM_RECORDS_TABLE_POS + (i * SIZE_RECORDS_REGISTER) + GD_MAX_NAME_LEN + 1 + (SIZE_RECORDS_REGISTER*15*j), pointsTable[j][i]);

            // escribe rank
            SRAM_writeByte(SRAM_RECORDS_TABLE_POS + (i * SIZE_RECORDS_REGISTER) + 14 + (SIZE_RECORDS_REGISTER*15*j), ranksTable[j][i]);
        }

        SRAM_writeLong(SRAM_RECORDS_TABLE_POS + ((i * SIZE_RECORDS_REGISTER) + 15) + (SIZE_RECORDS_REGISTER*15*j), ad.achievements);
    }
}


/////////////////////////////////////////////////////////////////
//
// void readRecordsTableSRAM(){
// lee la tabla de records en la sram
//
/////////////////////////////////////////////////////////////////
void readRecordsTableSRAM(){

	u8 i, j, c;	// Indices

    for( j = 0; j < 3; j++ ){
        for( i  = 0; i < NUM_REGISTERS_RECORDS; i++){

            // Leer nombres
            for( c = 0; c < GD_MAX_NAME_LEN+1; c++ )
                namesTable[j][i][c] = SRAM_readByte(SRAM_RECORDS_TABLE_POS + (i * SIZE_RECORDS_REGISTER) + c + (SIZE_RECORDS_REGISTER*15*j));

            // Leer puntuacion
            pointsTable[j][i] = SRAM_readLong(SRAM_RECORDS_TABLE_POS + (i * SIZE_RECORDS_REGISTER) + GD_MAX_NAME_LEN + 1 + (SIZE_RECORDS_REGISTER*15*j));

            ranksTable[j][i] = SRAM_readByte(SRAM_RECORDS_TABLE_POS + (i * SIZE_RECORDS_REGISTER) + 14 + (SIZE_RECORDS_REGISTER*15*j));
        }

        ad.achievements = SRAM_readLong(SRAM_RECORDS_TABLE_POS + ((i * SIZE_RECORDS_REGISTER) + 15) + (SIZE_RECORDS_REGISTER*15*j));
    }
}

