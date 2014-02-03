/*********************************************************************
   PicoTCP. Copyright (c) 2012 TASS Belgium NV. Some rights reserved.
   See LICENSE and COPYING for usage.

   Authors: Stijn Haers, Mathias Devos, Gustav Janssens, Sam Van Den Berge
 *********************************************************************/

#ifndef __PICO_ZMTP_H
#define __PICO_ZMTP_H

#include <stdint.h>

#define ZMQ_TYPE_PUBLISHER               1
#define ZMQ_TYPE_SUBSCRIBER              2
#define ZMQ_TYPE_REQ                     3
#define ZMQ_TYPE_REP                     4
#define ZMQ_TYPE_END                     5


enum zmq_state {
    ST_OPEN = 0,
    ST_CONNECTED,
    ST_SIGNATURE,
    ST_VERSION,
    ST_GREETING,
    ST_RDY,
    ST_BUSY,
    ST_END //Marks the end of the enum
};

struct zmtp_socket {
    struct pico_socket* sock;
    enum zmq_state state;
    uint16_t type;
};

struct zmq_msg {
    uint8_t flags;
    uint64_t len;
    uint8_t *buf;
};

struct zmtp_socket* zmtp_socket_open(uint16_t net, uint16_t proto, uint16_t type, void (*wakeup)(uint16_t ev, struct zmtp_socket* s));
static void zmtp_tcp_cb(uint16_t ev, struct pico_socket *s);
int zmtp_socket_bind(struct zmtp_socket* s, void* local_addr, uint16_t* port);
int8_t zmtp_socket_connect(struct zmtp_socket* s, void* srv_addr, uint16_t remote_port);
int8_t zmtp_socket_send(struct zmtp_socket* s, struct zmq_msg** msg, uint16_t len);
int8_t zmtp_socket_close(struct zmtp_socket *s);

#endif
