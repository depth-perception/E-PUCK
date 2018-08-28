/********************************************************************
* 			Advance agenda events of e-puck							*
*			December 2004: first version							*
*			Lucas Meier & Francesco Mondada  						*
*																	*
********************************************************************/

/*! \file
 * \ingroup motor_LED
 * \brief Manage the agendas (timer2)
 *
 * This module manage the agendas with the timer2.
 * \n \n An agenda is a structure made to work as chained list. It containts:
 * the function you want to launch, the time setup between two launching events,
 * a counter to measure the current time, a pointer to the next element of the list.
 * \n \n Each times the timer2 has an interrupt, all the agenda chained list is
 * scanned to look if an agenda has to be treated according to the cycle value
 * and current counter value.
 * \n \n If one (or more) agenda has to be treated, his callback function is launch.
 * \author Jonathan Besuchet
 */

#ifndef __AGENDA_H__
#define __AGENDA_H__


#define AG_ALREADY_CREATED	1
#define AG_NOT_FOUND		2

/**********************************************************************
 * ------------------------ Type definition ---------------------------
 **********************************************************************/

typedef struct AgendaType Agenda;

/*! \struct AgendaType
 * \brief srtuct Agenda as chained list
 *
 * The role of Agenda is to launch the pointed function when the
 * member "counter" is greater than the member "cycle".
 * \n The member "activate" can be on=1 or off=0. When it is off, the counter
 * don't increase.
 * \n This struct is designed to be used as chained list so we need a pointer
 * to the next element.
 */
struct AgendaType
{
  unsigned int  cycle;		/*!< length in 10e of ms of a cycle between two events */
  int  counter;				/*!< count the number of interrupts */
  char activate;			/*!< can be on=1 or off=0*/
  void (*function) (void);	/*!< function called when counter > cycle,
                             * \warning This function must have the following
                             * prototype: "void func(void)" */
  Agenda *next;				/*!< pointer on the next agenda*/
};


/***********************************************************************
 * ------------------------ From agenda.c file --------------------------
 **********************************************************************/
void e_start_agendas_processing(void);
void e_end_agendas_processing(void);

int e_activate_agenda(void (*func)(void), int cycle);
int e_destroy_agenda(void (*func)(void));

int e_set_agenda_cycle(void (*func)(void), int cycle);
int e_reset_agenda(void (*func)(void));

int e_pause_agenda(void (*func)(void));
int e_restart_agenda(void (*func)(void));

#endif /* __AGENDA_H__ */


/* End of File : agenda.h */
