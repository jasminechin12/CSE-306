#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    char arr[3];
    while (1) {
        readmouse(arr);
        printf(1, "1st byte: %x 2nd byte: %x 3rd byte: %x ", arr[0], arr[1], arr[2]);
        char state = arr[0] & 7;
        if ((state & 1) == 1)
            printf(1, "Left Btn: Pressed ");
        else
            printf(1, "Left Btn: Released ");
        if ((state & 2) == 0)
            printf(1, "Right Btn: Released ");
        else
            printf(1, "Right Btn: Pressed ");
        if ((state & 4) == 0)
            printf(1, "Middle Btn: Released ");
        else
            printf(1, "Middle Btn: Pressed ");
        printf(1, "Delta X: %d Delta Y: %d\n", arr[1], arr[2]);
    }
    return 0;
}
