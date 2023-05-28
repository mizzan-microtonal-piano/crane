#ifdef __cplusplus
 extern "C" {
#endif

#include <Arduino.h>



#define CRANE_KEY_C0  2
#define CRANE_KEY_C1  14
#define CRANE_KEY_C2  15
#define CRANE_KEY_R0  6
#define CRANE_KEY_R1  5
#define CRANE_KEY_R2  4
#define CRANE_KEY_R3  3

void buttonClearFlag();
void buttonFlagAutoClear(int clearTimeMs);
void buttonScanInit();
void buttonEdgeScan();
void buttonScan();
int readRow(int row);
void selectCol(int col);






#ifdef __cplusplus
}
#endif