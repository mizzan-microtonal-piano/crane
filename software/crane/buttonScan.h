#ifdef __cplusplus
 extern "C" {
#endif

#include <Arduino.h>



#define CRANE_KEY_C0  PC_6
#define CRANE_KEY_C1  PC_7
#define CRANE_KEY_C2  PC_8
#define CRANE_KEY_R0  PB_12
#define CRANE_KEY_R1  PB_13
#define CRANE_KEY_R2  PB_14
#define CRANE_KEY_R3  PB_15

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