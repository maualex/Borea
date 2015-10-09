#include "quadcopter_borea.h"

// Transmission of all data to SLAVE
int tx_to_slave2(unsigned char* data, char* risp, int lenght)
{
}

int tx_to_slave(unsigned char* data, char* risp, int lenght)
{
	        int i;

			CloseI2C2(); //spegne la periferica, disattiva interruzione e abbassa flag interrupt

		//	OpenI2C2(MASTER, BAUD_RATE_I2C);
		    
		    OpenI2C2(0x9300, 395); //setta i registri I2C2CON=arg1 e I2C2BRG=arg2
			IdleI2C2(); //controlla che i 5 bit di stato siano a 0 -> bus inattivo (io nn lo faccio)

			StartI2C2();    //SEN=1
			IdleI2C2();     //io qui faccio il while(SEN) lui invece controllo sempre i 5 bit

			MasterWriteI2C2(0xA2); //idem al mio con in più il controllo del bit di collisione
			IdleI2C2();

			MasterWriteI2C2('b');
			IdleI2C2();

			for(i=0;i<lenght;i++){
				MasterWriteI2C2(data[i]);
				IdleI2C2();
			}

/*			
			RestartI2C2();	
			IdleI2C2();
			MasterWriteI2C2(0xA3);
			IdleI2C2();
			*(risp)=MasterReadI2C2();
			AckI2C2();
			IdleI2C2();
			MasterReadI2C2();
			NotAckI2C2();

			CloseI2C2();
			
	*/		
			
			
			RestartI2C2();	//repeated start -> RSEN=1
			IdleI2C2();
			
			MasterWriteI2C2(0xA3);      //indirizzo di lettura
			IdleI2C2();
			if(I2C2STATbits.ACKSTAT!=0) return 1;

			int timeout=0;
			I2C2STATbits.I2COV=0;
    	
    	    //avvio ricezione del primo byte
    	    I2C2CONbits.RCEN=1;   
    	    
    	    while(I2C2STATbits.RBF == 0)  //attendo che il buffer si riempa
        	{
            	timeout++;
            	if(timeout>5000) return 2;  
            }   	
        	*(risp) = I2C2RCV;
        	
        	if(I2C2STATbits.I2COV) return 3; 
      
            I2C2CONbits.ACKDT=1;    //invio NACK
            I2C2CONbits.ACKEN=1;
            while(I2C2CONbits.ACKEN);   //attendo la fine del ack 
     
        	//chiudo la comunicazione
	        I2C2CONbits.PEN=1;
	        while(I2C2CONbits.PEN);
        	
        	CloseI2C2();    //spegne l'I2C
        	
      
        	return 0;  	
}




// FUNCION DE TRANSMISION
void tx_to_gps(void)
{

	U1STAbits.UTXEN = 1; //enable bit de transmision

	while(busy_uart_gps()); 
	write_uart_gps('$'); 	 // envio el caracter 0xA0 hacia el GPS 
	while(busy_uart_gps());  // espero por la completa transmision
	write_uart_gps('P');    // envio el siguiente caracter
	while(busy_uart_gps());  // espero de nuevo
	write_uart_gps('S'); 	 // etc
	while(busy_uart_gps()); 
	write_uart_gps('R');
	while(busy_uart_gps()); 
	write_uart_gps('F'); 
	while(busy_uart_gps()); 
	write_uart_gps('1'); 
	while(busy_uart_gps()); 
	write_uart_gps('5'); 
	while(busy_uart_gps()); 
	write_uart_gps('1'); 
	while(busy_uart_gps()); 
	write_uart_gps(','); 
	while(busy_uart_gps()); 
	write_uart_gps('0');
	while(busy_uart_gps()); 
	write_uart_gps('1'); 
	while(busy_uart_gps());  
	write_uart_gps('*'); 
	while(busy_uart_gps()); 
	write_uart_gps('0');
	while(busy_uart_gps()); 
	write_uart_gps('F'); 
	while(busy_uart_gps()); 
 
	write_uart_gps(0x0D); 
	while(busy_uart_gps()); 
	write_uart_gps(0x0A); 
	while(busy_uart_gps()); 

	U1STAbits.UTXEN = 0; //disable bit de transmision
}


//FUNCION DE RECEPCION
void rx_from_gps(void)
{

	char c='0';         // declaro un caracter
	char datos[80];  	// declaro un arreglo de caracteres para el mensaje completo 
	int conta = 0;		// declaro un entero para utilizarlo como contador
	double valor[80]={0};
	//variabili GPS
double tiempo_utc=0;
double grados_lat=0;
double minutos_lat=0;
double grados_lon=0;
double minutos_lon=0;
//volatile double satelites=0;
double altitud=0;
double velocidad=0;

		while (1)	
		{

			if(U1STAbits.FERR == 1){
				continue;
			}	
		
			if(U1STAbits.OERR == 1){        // verifico si hay un error de Overrun
				U1STAbits.OERR = 0;    	 	// pongo en cero para permitir recibir mensajes
			}
		
			if(U1STAbits.URXDA==1){       	// verifico si el dato esta listo para ser leido 
				c = U1RXREG; 			    // obtengo un dato desde el buffer de recepcion
									
				if(c == '$'){				// c == $, hex=0x0024 
					datos[0] = c; 			
				} 
			
				if(datos[0]=='$'){
					datos[conta] = c;
					valor[conta] = (double)datos[conta];
					if (datos[conta]=='*'){
						if (datos[3]=='G' && datos[4]=='G' && datos[5]=='A'){
							tiempo_utc = ((valor[7]-48)*100000) + ((valor[8]-48)*10000) + ((valor[9]-48)*1000) + ((valor[10]-48)*100) + ((valor[11]-48)*10) + (valor[12]-48) + ((valor[14]-48)*0.1) + ((valor[15]-48)*0.01) + ((valor[16]-48)*0.001);
							grados_lat = ((valor[18]-48)*10) + (valor[19]-48);
							minutos_lat = ((valor[20]-48)*10) + (valor[21]-48) + ((valor[23]-48)*0.1) + ((valor[24]-48)*0.01) + ((valor[25]-48)*0.001) + ((valor[26]-48)*0.0001);
							grados_lon = ((valor[30]-48)*100) + ((valor[31]-48)*10) + (valor[32]-48);
							minutos_lon = ((valor[33]-48)*10) + (valor[34]-48) + ((valor[36]-48)*0.1) + ((valor[37]-48)*0.01) + ((valor[38]-48)*0.001) + ((valor[39]-48)*0.0001);
							//satelites = ((valor[45]-48)*10) + (valor[46]-48);
							altitud = ((valor[52]-48)*100) + ((valor[53]-48)*10) + (valor[54]-48) + ((valor[56]-48)*0.1);
							LATEbits.LATE2 = ~LATEbits.LATE2;   //Toggle output to LED2
							conta=-1;
							datos[0]=0;
							}
						if (datos[3]=='R' && datos[4]=='M' && datos[5]=='C'){
						//	cont_comas=0;
							velocidad = (valor[45]-48) + ((valor[47]-48)*0.1) + ((valor[48]-48)*0.01);
							conta=-1;
							datos[0]=0;
							}
					}

					conta++;
				}

			}

		}	
}

/*************************************************************************
* Function Name     : busy_uart_gps                                      *
* Description       : This returns status whether the transmission       *
*                     is in progress or not, by checking Status bit TRMT *
* Parameters        : None                                               *
* Return Value      : Info about whether transmission is in progress.    *
*************************************************************************/
char busy_uart_gps(void)
{  
    return(!U1STAbits.TRMT);
}

////////////////////////////////////////////////////////////////////////////
/*********************************************************************
* Function Name     : write_uart_gps                                 *
* Description       : This function writes data into the UxTXREG,    *
* Parameters        : unsigned int data the data to be written       *
* Return Value      : None                                           *
*********************************************************************/
void write_uart_gps(unsigned int data)
{
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

////////////////////////////////////////////////////////////////////////////

