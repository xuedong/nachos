/* \file drvACIA.cc
   \brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return 
//      immediately, and an interrupt happens later on).  
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.  
//  See copyright_insa.h for copyright notice and limitation 
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h"         // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
  Initialize the ACIA driver. In the ACIA Interrupt mode, 
  initialize the reception index and semaphores and allow 
  reception and emission interrupts. 
  In the ACIA Busy Waiting mode, simply inittialize the ACIA 
  working mode and create the semaphore.
  */
//-------------------------------------------------------------------------

DriverACIA::DriverACIA()
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
#endif
#ifdef ETUDIANTS_TP
    send_sema = new Semaphore("ACIA-driver-send", 1);
    receive_sema = new Semaphore("ACIA-driver-receive", (g_machine->acia->GetWorkingMode() & REC_INTERRUPT) ? 0 : 1);
    ind_send = 0;
    ind_rec = 0;
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
  */
//-------------------------------------------------------------------------

int DriverACIA::TtySend(char* buff)
{ 
#ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
#endif
#ifdef ETUDIANTS_TP
    DEBUG('D', (char*)"ACIA: TtySend Pre.\n");
    send_sema->P();
    DEBUG('D', (char*)"ACIA: TtySend Post.\n");
    ind_send = 0;

    if(g_machine->acia->GetWorkingMode() & SEND_INTERRUPT){
        // Pretty waiting
        memcpy(send_buffer, buff, strlen(buff));

        g_machine->acia->PutChar(send_buffer[0]);
    }else{
        // Busy waiting
        do{
            g_machine->acia->PutChar(buff[ind_send]);
            while(g_machine->acia->GetOutputStateReg() != EMPTY);
        }while(buff[ind_send++] != 0);

        send_sema->V();
    }
    DEBUG('D', (char*)"ACIA: TtySend V.\n");
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA 
//  (Busy Waiting and Interrupt mode).
  */
//-------------------------------------------------------------------------

int DriverACIA::TtyReceive(char* buff, int lg)
{
#ifndef ETUDIANTS_TP
    printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
    exit(-1);
    return 0;
#endif
#ifdef ETUDIANTS_TP

    DEBUG('D', (char*)"ACIA: TtyRecv Pre.\n");
    receive_sema->P();
    DEBUG('D', (char*)"ACIA: TtyRecv Post.\n");
    ind_rec=0;
    if(g_machine->acia->GetWorkingMode() & REC_INTERRUPT){
        memcpy(buff, receive_buffer, min(BUFFER_SIZE, lg));
        full_rec=false;

    }else{
        // Busy waiting
        if(lg > 0){
            do{
                while(g_machine->acia->GetOutputStateReg() == EMPTY);
                buff[ind_rec] = g_machine->acia->GetChar();
            }while(buff[ind_rec++] != 0 && ind_rec < lg);
        }

        send_sema->V();
    }
    DEBUG('D', (char*)"ACIA: TtyRecv V.\n");

    return ind_rec;
#endif
}


//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
  Used in the ACIA Interrupt mode only. 
  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptSend()
{
#ifndef ETUDIANTS_TP
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
#endif
#ifdef ETUDIANTS_TP
    if(g_machine->acia->GetWorkingMode() & SEND_INTERRUPT){
        if(send_buffer[ind_send] != 0){
            //DEBUG('D', (char*)"ACIA: TtySend Pre.\n");
            g_machine->acia->PutChar(send_buffer[++ind_send]);
        }else{
            send_sema->V();
        }
    }
#endif
}

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA. 
  Releases the receive_sema semaphore and disables reception 
  interrupts when the last character of the message is received 
  (character '\0').
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptReceive()
{
#ifndef ETUDIANTS_TP
    printf("**** Warning: receive interrupt handler not implemented yet\n");
    exit(-1);
#endif
#ifdef ETUDIANTS_TP
    if((g_machine->acia->GetWorkingMode() & REC_INTERRUPT)  && ind_rec < BUFFER_SIZE && !full_rec){
        char c = g_machine->acia->GetChar();
        receive_buffer[ind_rec++] = c;
        if(c == 0){
            full_rec=true;
            receive_sema->V();
        }
    }
#endif
}
