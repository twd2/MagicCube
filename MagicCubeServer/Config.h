#pragma once

#define MEM_DEBUG

typedef unsigned int package_len_t;

#define MAGIC_MARK "MC"

#define ENABLE_IPV4
#define LISTEN_ADDR "0.0.0.0"
#define LISTEN_PORT 2333
#define LISTEN_BACKLOG 1024

#define ENABLE_IPV6
#define LISTEN_ADDR6 "::"
#define LISTEN_PORT6 2333
#define LISTEN_BACKLOG6 1024

// CHECK_INTERVAL should be less then TIMEOUT_S
#define CHECK_INTERVAL 1 // 10 
#define TIMEOUT_S 30

#define PACKAGE_MAXLENGTH ((package_len_t)(-1)) //(1024)
#define HTTP_HEADER_MAXLENGTH ((size_t)(16384))
