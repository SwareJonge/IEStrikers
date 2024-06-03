#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif

void MoError(void *instance, const char *msg);
void MoDebug(void *instance, const char *msg);

#ifdef __cplusplus
}
#endif
