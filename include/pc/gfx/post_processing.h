#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include <stdbool.h>

void Post_Init(int width, int height);
void Post_Process(void);
void Post_Resize(int width, int height);
void Post_Shutdown(void);

#endif
