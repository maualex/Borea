#include <p33FJ256GP710A.h>
#include "comm.h"
#include <stdlib.h>     //funzione malloc()
#include <math.h>       //funzione floor()
#include "system.h"
#include "state_machine.h"

//variabili globali
char pc_msg[4];     //message from ground station(PC)


//variabili interne
unsigned char i_bytes;
unsigned char tot_bytes_to_send;
unsigned char n_bytes_full_buffer;
unsigned char flag_tx; //flag
unsigned char data_to_be_send[N_DATA];		// vector of bytes to be send
unsigned char *pchar;
extern float ext_mis_vb;


void prepare_msg(float *data_float, unsigned int control_step, unsigned char state, unsigned char *data, unsigned char n_var)
{
    unsigned int i,kk;
    unsigned char *p = data;
    unsigned char *pp;  //puntatore ai byte interni di un float! :)
    
    for(i=0;i<n_var;i++)   
    {
        kk = 4*i;
        pp = (unsigned char*)&data_float[i];   //dal LSB fino al MSB
        *(p + kk)     = *pp;
        *(p + kk + 1) = *(pp+1);
        *(p + kk + 2) = *(pp+2);
        *(p + kk + 3) = *(pp+3);
    }
//    *(p+kk+4) = control_step;
//    *(p+kk+5) = control_step>>8;
//    *(p+kk+6) = state; 
};

//void serialize(void *data_in, unsigned char *serial_out, unsigned int length)
//{
//    unsigned int i, j, offset, n;
//    unsigned char *p;
//    n = sizeof(*data_in);
//    for(i=0;i<length;i++)
//    {
//        offset  = n*i;
//        p       = (unsigned char*)&data_in[i];
//        //moves single bytes
//        for(j=0;j<n;j++)
//        {
//            *(serial_out + offset + j) = *(p+1);
//        }
//    }
//}

void prepare_msg2(int *data_int, unsigned int control_step, unsigned char state, unsigned char *data, unsigned char n_var)
{
    unsigned int i,kk;
    unsigned char *p = data;
    unsigned char *pp;  //puntatore ai byte interni di un int
    
    for(i=0;i<n_var;i++)   
    {
        kk = 2*i;
        pp = (unsigned char*)&data_int[i];   //dal LSB fino al MSB
        *(p + kk)     = *pp;
        *(p + kk + 1) = *(pp+1);
    }
    *(p+kk+2) = control_step;
    *(p+kk+3) = control_step>>8;
    *(p+kk+4) = state; 
};

unsigned char send2gs(unsigned char *data)
{
    unsigned char i, err;
    //unsigned int time, begin;
    int old_ipl;
    
    err     = 0;    // flag_tx ok
    
//    if(flag_tx==1)	//trasmissione precedente ancora attiva -> controllo se rimane solo un frame altrimenti inizio col nuovo msg
//    {
//	    //attendo che il buffer sia stato svuotato prima 
//	    //di inziare una nuova trasmissione o controllare se rimane l'ultimo pacchetto
//        while(!U2STAbits.TRMT) {}
//        
//        if(i_bytes>=n_bytes_full_buffer) 		 //last packet left
//        {
//			begin 			= TMR1;
//            time  			= TMR1-begin;
//            while(flag_tx==1 && time<TIMEOUT)    //wait for the last packet
//            {}
//
//            
//            if(time>=TIMEOUT)
//            {
//                err = 2;    //trasmission aborted: timeout exploit
//            }
//        }
//        else    //more than 2 packets left
//        {
//            err = 1; //message is lost: insufficient time
//        }  
//        	 
//    }

    /* send new message */
    
    SET_AND_SAVE_CPU_IPL(old_ipl,7);
    for(i=0;i<TX_BUFF_LENGTH;i++) //write first 4 byte into the tx buffer
    {
        U2TXREG = data[i];
    }
    i_bytes      = TX_BUFF_LENGTH;
    flag_tx 	 = 1;
    RESTORE_CPU_IPL(old_ipl);
    
    return err;       //return the error code value
};


void __attribute__((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void)    //asynchronous message from ground station
{      
	IFS1bits.U2RXIF     = 0;    //clear interrupt flag
	int old_ipl;                                                                     //message is formed by 4 byte, the first one is the command
    unsigned char error = 0, i;  
   
    
    //check error before reading
    if(U2STAbits.FERR==1)   error = 1;  //framming error: stop bit has been detected at low level logic
    if(U2STAbits.PERR==1)   error = 2;  //parity error

    //read all data available in the buffer
    
    SET_AND_SAVE_CPU_IPL(old_ipl,7);
    for(i=0;i<4;i++) pc_msg[i] = 255;
    i = 0;
    while(U2STAbits.URXDA==1)   
    {
        if(i<4) pc_msg[i] = U2RXREG;
        i++;    //the value of 'i' indicate the number of data into the buffer
    }
    RESTORE_CPU_IPL(old_ipl);
    
    if(i!=4) error = 3;         //it has been read less than 4 byte
    if(U2STAbits.OERR==1) 
    {
        U2STAbits.OERR = 0;     //overrun bit cleared
        error = 4;
    }    

};

//unsigned int array[100];
//unsigned int h = 0;

void __attribute__((__interrupt__,__no_auto_psv__)) _U2TXInterrupt(void) //interrompe quando il buffer è stato scaricato
{
    //abbasso flag interruzione
    IFS1bits.U2TXIF  = 0;
    
//    //local variables
    unsigned char init;
    unsigned char i;
    int old_ipl;

    /*
    All'accensione della periferica viene generata una interruzione
    il flag flag_tx evita di generare una trasmissione'
    */
//    if(flag_tx==1 && i_bytes==tot_bytes_to_send) 
//    {
//        
//	    T5CONbits.TON   = 0;
//	    if(TMR5>6500)
//	    {
//		    if(h<100)
//		    {
//			    array[h]		= TMR5;
//			    h++;
//		    }
//		    else
//		    {
//			    h=100;
//		    }
//	    }
//	    TMR5			= 0;
//	    flag_tx 		= 0;   //end trasmission. The last buffer has been send.
//    }
    
    if(flag_tx==1)  
    {
        
        
	    if(i_bytes==n_bytes_full_buffer) //->rimane 1-2-3 byte dell'ultimo frame
	    {
	        init        = TX_BUFF_LENGTH - (tot_bytes_to_send - n_bytes_full_buffer);
	    }
	    else
	    {
	        init = 0;
	    }
	    
	    /*
	    se il ciclo che segue viene interrotto allora verrà trasmesso un buffer
	    non pieno pertanto verrà generata interruzione in eccesso.
	    Per tale motivo la scrittura nel buffer è protetta alzando la priorità 
	    della CPU al valore massimo (7)
	    */
	    if(i_bytes==tot_bytes_to_send) 
        {
            flag_tx 		= 0;
            flag_message_send = 1;
        }
        else
        {
            SET_AND_SAVE_CPU_IPL(old_ipl,7);   
            for(i=init;i<TX_BUFF_LENGTH;i++)   
            {  
                U2TXREG = data_to_be_send[i_bytes];  
                i_bytes++;                 
            }
            RESTORE_CPU_IPL(old_ipl);
        }
    }
             
};

void comm_init()
{
    tot_bytes_to_send   = N_DATA;
    n_bytes_full_buffer = floor(tot_bytes_to_send/TX_BUFF_LENGTH)*TX_BUFF_LENGTH;
	int i;
	for(i=0;i<4;i++)
    {
        pc_msg[i] = 0;	// message received from ground station (GS)
    }
    
//	data_to_be_send	= (unsigned char*)malloc(tot_bytes_to_send);	// allocation
};

unsigned char send_msg(int* data)
{
	prepare_msg2(data, control_step, machine_state, data_to_be_send, N_DATA);
	return send2gs(data_to_be_send);
};
