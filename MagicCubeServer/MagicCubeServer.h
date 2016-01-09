#pragma once

#include "stdafx.h"
#include "Config.h"
#include "Session.h"

void eventEntry(event_base*);

#ifdef ENABLE_IPV4
void accept_cb(evutil_socket_t, short, void*);
#endif
#ifdef ENABLE_IPV6
void accept6_cb(evutil_socket_t, short, void*);
#endif

void read_cb(bufferevent*, void*);
void error_cb(bufferevent*, short, void*);
void write_cb(bufferevent*, void*);