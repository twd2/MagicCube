#pragma once

#define MEM_DEBUG

typedef unsigned int package_len_t;

#define ENABLE_IPV4
#define LISTEN_ADDR "0.0.0.0"
#define LISTEN_PORT 2333
#define LISTEN_BACKLOG 1024

#define ENABLE_IPV6
#define LISTEN_ADDR6 "::"
#define LISTEN_PORT6 2333
#define LISTEN_BACKLOG6 1024

#define CHECK_INTERVAL 0 // 10 
#define TIMEOUT_S 30

#define PACKAGE_MAXLENGTH (package_len_t)1024
