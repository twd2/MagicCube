#pragma once

#include "stdafx.h"
#include "Config.h"

void eventEntry(event_base*);

void accept_cb(evutil_socket_t, short, void*);
void read_cb(bufferevent*, void*);
void error_cb(bufferevent*, short, void*);
void write_cb(bufferevent*, void*);