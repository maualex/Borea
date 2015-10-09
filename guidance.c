#include "quadcopter_borea.h"

void vertical_guidance(float* pz, float* vz, float* az)
{
	/*
    pz = 1;
    vz = 0;
    az = 0;
    
    
    //__________VERTICAL GUIDANCE
            state_supervisor = nstate;
            pZ_ref  = npos;
            vZ_ref  = nv;
            d       = vertical_target - pZ_ref;
            a       = 0;
            ad      = fabs(d);
            nd      = fabs(d-vZ_ref);    //spazio rimanente con la corrente vel
            alfa    = fabs((vZ_ref*vZ_ref)/(2*d-vZ_ref));  //decelerazione costante richiesta per avere d=0 con v=0 
            if (signbit(d)==0) sd=1;
            else sd=-1;
            if (signbit(vZ_ref)==0) sv=1;
            else sv=-1;
            
                
            switch(state_supervisor)
            {
                case 0:
                    if(ad>=RHOY)  //PARTENZA
                    {
                        a=sd*AMAX;
                        nstate=1;
                    }
                    //aggiorno target
                    vertical_target = floor(target_from_PC[2]/RHOY)*RHOY;
                break;
                
                case 1:         //ACCELERAZIONE
                    a=sd*AMAX;
                    if(fabs(vZ_ref)>=VMAX)    //velocità saturata -> si va allo stato 2
                    {
                        a=0;
                        vZ_ref=sd*VMAX;
                        nstate=2;
                    }
                    //dall'accelerazione passo direttamente al freno (salta la fase 2)
                    if(alfa>=AMAX*(1-DELTA))    //decelerazione richiesta oltre il limite max
                    {
                        //FRENO (è il primo passo di frenata)
                        a=-sd*alfa*(1+DELTA);
                        nstate=3;
                    }
                    
                    
                    //controllo se può verificarsi un overshoot nel passo successivo con il valore di acc appena calcolato
                    nv=fabs(vZ_ref+a);    //spazio percorso con la nuova velocità
                    //condizione di overshoot
                    if(nv>nd)       //-> sorpasserò il target (overshoot)
                    {
                        a = d-2*vZ_ref;
                    }
                    
                    //controllo se abbiamo raggiunto il target
                    if(nd<AMAX) //siamo arrivati al target -> si va allo stato 4 (attesa)
                    {
                        a=-vZ_ref;
                        nstate=4;
                    }
                    
                    //aggiorno target
                    vertical_target = floor(target_from_PC[2]/RHOY)*RHOY;
                break;
                
                case 2:         //FOLLE
                    if(sd!=sv)      //devo invertare il verso di marcia
                    {
                        nstate=1;   
                        a=sd*AMAX; 
                    }
                    if(alfa>=AMAX*(1-DELTA))    //decelerazione richiesta oltre il limite max
                    {
                        //FRENO (è il primo passo di frenata)
                        a=-sd*alfa*(1+DELTA);
                        nstate=3;
                    }
                    
                    
                    //controllo se può verificarsi un overshoot nel passo successivo con il valore di acc appena calcolato
                    nv=fabs(vZ_ref+a);    //spazio percorso con la nuova velocità
                    //condizione di overshoot
                    if(nv>nd)       //-> sorpasserò il target (overshoot)
                    {
                        a = d-2*vZ_ref;
                    }
                    
                    //controllo se abbiamo raggiunto il target
                    if(nd<AMAX) //siamo arrivati al target -> si va allo stato 4 (attesa)
                    {
                        a=-vZ_ref;
                        nstate=4;
                    }
                    
                    //aggiorno target
                    vertical_target = floor(target_from_PC[2]/RHOY)*RHOY;
                break;
                
                case 3:         //FRENATA - LO STATO NON CAMBIA
                    a=-sd*alfa*(1+DELTA);   //frenata con smorzamento
                    
                    
                    //controllo se può verificarsi un overshoot nel passo successivo  con il valore di acc appena calcolato
                    nv=fabs(vZ_ref+a);    //spazio percorso con la nuova velocità
                    //condizione di overshoot
                    if(nv>nd)       //-> sorpasserò il target (overshoot)
                    {
                        a = d-2*vZ_ref;
                    }
                    
                    //controllo se abbiamo raggiunto il target
                    if(nd<AMAX) //siamo arrivati al target -> si va allo stato 4 (attesa)
                    {
                        a=-vZ_ref;       //al prossimo passo andrà a zero perchè viene inizializata ad ogni passo
                        nstate=4;
                    }
                break;
                
                case 4:
                    if(ad>=RHOY)  //RIPARTENZA
                    {
                        a=sd*AMAX;
                        nstate=1;
                        cont=0;
                    }
                    else
                    {
                        cont++;
                    }
                    
                    if(cont>WAIT)
                    {
                        nstate  = 0;
                        cont    = 0;
                    }
                    //aggiorno target
                    vertical_target = floor(target_from_PC[2]/RHOY)*RHOY;
                break;
                
            }
            
            //states update ( vertical guidance )
            nv   = vZ_ref + a;
            npos = pZ_ref + vZ_ref;
            
            if(fabs(nv)>=VMAX)  //velocity is saturated -> 'a' is wrong (guidance fix it at hte next step)
            {
                a=0;
            }    
            az_ref = a;         //reference vertical command [meters] 
    */        
};
