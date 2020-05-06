#ifndef ANT_EVENT_H
#define ANT_EVENT_H

#include <stdio.h>


/**@brief   ANT stack event. */
typedef struct
{
    ANT_MESSAGE message;    //!< ANT Message.
    uint8_t     channel;    //!< Channel number.
    uint8_t     event;      //!< Event code.
} ant_evt_t;

/**@brief   ANT stack event handler. */
typedef void (*nrf_sdh_ant_evt_handler_t)(ant_evt_t * p_ant_evt, void * p_context);

/**@brief   ANT event observer. */
typedef struct
{
    nrf_sdh_ant_evt_handler_t handler;      //!< ANT event handler.
    void *                    p_context;    //!< A parameter to the event handler.
} const nrf_sdh_ant_evt_observer_t;

#endif