#pragma once

typedef unsigned int package_len_t;

#define MAGIC_MARK "MCP/0.0"
#define LINE_ENDDING "\r\n"

#define ENABLE_IPV4
#define ENABLE_IPV6

// CHECK_INTERVAL should be less than TIMEOUT_S
#define CHECK_INTERVAL 1 // 10 
#define TIMEOUT_S 30

#define PACKAGE_MAXLENGTH (static_cast<package_len_t>(-1)) //(1024)
#define HTTP_HEADER_MAXLENGTH (static_cast<size_t>(16384))
#define LINE_MAXLENGTH (static_cast<size_t>(16384))
