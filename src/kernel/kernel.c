#include "../drivers/screen.h"
#include "util.h"

void main() {
    clear_screen();

    /* Fill up the screen */
    int i = 0;
    for (i = 0; i < 25; i++) {
        char str[255];
        int_to_ascii(i, str);
        printk_at(str, 0, i);
    }

    printk("\nThis text forces the kernel to scroll. Row 0 will disappear. ");
    printk("And with this text, the kernel will scroll again, and row 1 will disappear too!");
}
