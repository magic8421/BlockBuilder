#pragma once

#define  ZLOG(...) zLogImpl(__FILE__, __LINE__, __VA_ARGS__)

void zLogInit();
void zLogImpl(const char *source, int line, const char *format, ...);
