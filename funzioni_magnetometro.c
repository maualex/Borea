#ifndef __33FJ256GP710A_H
#include <p33FJ256GP710A.h>
#endif
	
int ReadMagneto(int* Magneticfield)
{
    int n_byte, timeout, corrido, msb, lsb;
	char data[6];
	
    //avvio la comunicazione
	I2C2CONbits.SEN=1;
	while(I2C2CONbits.SEN);
//******************* QUESTA PARTE NON SERVE *****************/
//perchè il puntatore interno punta già al primo di lettura(dopo la lettura del sesto 
//registro il puntatore torna in automatico al primo )	
	//invio indirizzo dispositivo in scrittura
	I2C2TRN=0x3c;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 1;
	
	//invio indirizzo primo registro da leggere in sequenza
	I2C2TRN=0x03;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 2;
	
	//repeated start
	I2C2CONbits.RSEN=1;
	while(I2C2CONbits.RSEN);
/**************************************************************/	
	//invio richiesta di lettura
	I2C2TRN=0x3d;   //queury di lettura (il puntatore interno è già pronto sul primo registro)
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 3;
	
	//fase di lettura
	n_byte=0;
	while(n_byte<6)
	{
    	I2C1STATbits.I2COV=0;
    	timeout=0; 
    	
    	//avvio ricezione di un byte
    	I2C2CONbits.RCEN=1;   
    	while(I2C2STATbits.RBF==0)  //attendo che il buffer si riempa
    	{
        	timeout++;
        	if(timeout>1000) return 4;  //non dovrebbe servire
        }   	
    	data[n_byte]=I2C2RCV;
    	if(I2C1STATbits.I2COV) return 5; //non dovrebbe servire
    	
    	//invio acknowledge
    	if(n_byte<5) I2C2CONbits.ACKDT=0;
    	    else I2C2CONbits.ACKDT=1;   //ultimo byte ricevuto, invio NACK
    	    
    	I2C2CONbits.ACKEN=1;
    	while(I2C2CONbits.ACKEN);   //attendo la fine del ack
    	
    	n_byte++;
    }   	
	
	//chiudo la comunicazione
	I2C2CONbits.PEN=1;
	while(I2C2CONbits.PEN);
	
	//impacchetto i dati(dati ricevuti nell'ordine xxzzyy)
    msb=(int)data[0];
    lsb=(int)data[1] & 0x00FF;  //questo prodotto logico serve per evitare che ci siano bit a caso nei ultimi 8 bit di lsb??
    corrido=msb<<8;
    Magneticfield[0]=corrido|lsb;
    msb=(int)data[2];
    lsb=(int)data[3] & 0x00FF;
    corrido=msb<<8;
    Magneticfield[2]=corrido|lsb;
    msb=(int)data[4];
    lsb=(int)data[5] & 0x00FF;
    corrido=msb<<8;
    Magneticfield[1]=corrido|lsb;       
    return 0;
};



int ConfigMagneto()
{
    //avvio la comunicazione
	I2C2CONbits.SEN=1;
	while(I2C2CONbits.SEN);
    
//INVIO CONFIGURAZIONE 1
    //invio indirizzo dispositivo in scrittura
	I2C2TRN=0x3c;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 1;
	
	//invio indirizzo registro di configurazione
	I2C2TRN=0x00;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);    //attendo che il buffer venga trasmesso
	while(I2C2STATbits.TRSTAT); //attendo la ricezione del bit di acknowledge
	if(I2C2STATbits.ACKSTAT!=0) return 2;
	
    //invio configurazione
	I2C2TRN=0b01111000; //75Hz,8 medie,nessun offset artificiale
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 3;
    	
//INVIO CONFIGURAZIONE 2
	//repeated start            FORSE NON SERVE!
	I2C2CONbits.RSEN=1;
	while(I2C2CONbits.RSEN);
	
	//invio indirizzo dispositivo in scrittura
	I2C2TRN=0x3c;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 1;
	
	//invio indirizzo registro di configurazione
	I2C2TRN=0x01;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 2;
	
    //invio configurazione
	//I2C2TRN=0x00;   //guadagno massimo -> max risoluzione
	I2C2TRN=0b10100000;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 3;
	
//INVIO CONFIGURAZIONE 3
	//repeated start
	I2C2CONbits.RSEN=1;
	while(I2C2CONbits.RSEN);
	
	//invio indirizzo dispositivo in scrittura
	I2C2TRN=0x3c;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 1;
	
	//invio indirizzo registro di configurazione
	I2C2TRN=0x02;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 2;
	
    //invio configurazione
	I2C2TRN=0x00;
	//attendo invio e verifico acknowledge
	while(I2C2STATbits.TBF);
	while(I2C2STATbits.TRSTAT);
	if(I2C2STATbits.ACKSTAT!=0) return 3;	

    	
    //chiudo la comunicazione
	I2C2CONbits.PEN=1;
	while(I2C2CONbits.PEN);
	return 0;	
};	
