/* 
 * File:   state_predictor.h
 * Author: Lotufo
 *
 * Created on 06 October 2015, 13:17
 */

#ifndef STATE_PREDICTOR_H
#define	STATE_PREDICTOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    //definizione del tipo di dato per un predittore dello stato
    typedef struct
    {
        float* x;
        float* xp;
        float* u;
        float* y;
        float* em;
    } STATE_PREDICTOR;

    /* prototipi */
    
    //alloca la struttura dati delle dimensioni richieste e la inizializza
    void state_predictor_init(STATE_PREDICTOR* predictor, unsigned int nx, unsigned int nu, unsigned int ny);
    //aggiorna gli stati e l'errore di modello (gl ingressi sono impostati fuori dall'utente)
    void state_predictor_update_attitudeX(STATE_PREDICTOR* predictor);
    void state_predictor_update_attitudeY(STATE_PREDICTOR* predictor);
    void state_predictor_update_attitudeZ(STATE_PREDICTOR* predictor);
    //dealloca la struttura dati
    void state_predictor_delete(STATE_PREDICTOR* predictor);
    
#ifdef	__cplusplus
}
#endif

#endif	/* STATE_PREDICTOR_H */

