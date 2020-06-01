#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"

static void keyboard_callback(registers_t regs) {
  /* The PIC leaves us the scancode in port 0x60 */
  u8 scancode = port_byte_in(0x60);
  char *sc_ascii;
  int_to_ascii(scancode, sc_ascii);
  printk("Keyboard scancode: ");
  printk(sc_ascii);
  printk(", ");
  print_letter(scancode);
  printk("\n");
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback); 
}

void print_letter(u8 scancode) {
  switch (scancode) {
    case 0x0:
      printk("ERROR");
			break;
    case 0x1:
      printk("ESC");
      break;
    case 0x2:
      printk("1");
      break;
    case 0x3:
      printk("2");
      break;
    case 0x4:
      printk("3");
      break;
    case 0x5:
      printk("4");
      break;
    case 0x6:
      printk("5");
      break;
    case 0x7:
      printk("6");
      break;
    case 0x8:
      printk("7");
      break;
    case 0x9:
      printk("8");
      break;
    case 0x0A:
      printk("9");
      break;
    case 0x0B:
      printk("0");
      break;
    case 0x0C:
      printk("-");
      break;
    case 0x0D:
      printk("+");
      break;
    case 0x0E:
      printk("Backspace");
      break;
    case 0x0F:
      printk("Tab");
      break;
    case 0x10:
      printk("Q");
      break;
    case 0x11:
      printk("W");
      break;
    case 0x12:
      printk("E");
      break;
    case 0x13:
      printk("R");
      break;
    case 0x14:
      printk("T");
      break;
    case 0x15:
      printk("Y");
      break;
    case 0x16:
      printk("U");
      break;
    case 0x17:
      printk("I");
      break;
    case 0x18:
      printk("O");
      break;
    case 0x19:
      printk("P");
      break;
		case 0x1A:
			printk("[");
			break;
		case 0x1B:
			printk("]");
			break;
		case 0x1C:
			printk("ENTER");
			break;
		case 0x1D:
			printk("LCtrl");
			break;
		case 0x1E:
			printk("A");
			break;
		case 0x1F:
			printk("S");
			break;
    case 0x20:
      printk("D");
      break;
    case 0x21:
      printk("F");
      break;
    case 0x22:
      printk("G");
      break;
    case 0x23:
      printk("H");
      break;
    case 0x24:
      printk("J");
      break;
    case 0x25:
      printk("K");
      break;
    case 0x26:
      printk("L");
      break;
    case 0x27:
      printk(";");
      break;
    case 0x28:
      printk("'");
      break;
    case 0x29:
      printk("`");
      break;
		case 0x2A:
			printk("LShift");
			break;
		case 0x2B:
			printk("\\");
			break;
		case 0x2C:
			printk("Z");
			break;
		case 0x2D:
			printk("X");
			break;
		case 0x2E:
			printk("C");
			break;
		case 0x2F:
			printk("V");
			break;
    case 0x30:
      printk("B");
      break;
    case 0x31:
      printk("N");
      break;
    case 0x32:
      printk("M");
      break;
    case 0x33:
      printk(",");
      break;
    case 0x34:
      printk(".");
      break;
    case 0x35:
      printk("/");
      break;
    case 0x36:
      printk("Rshift");
      break;
    case 0x37:
      printk("Keypad *");
      break;
    case 0x38:
      printk("LAlt");
      break;
    case 0x39:
      printk("Spc");
      break;
    default:
      /* 'keuyp' event corresponds to the 'keydown' + 0x80 
       * it may still be a scancode we haven't implemented yet, or
       * maybe a control/escape sequence */
      if (scancode <= 0x7f) {
        printk("Unknown key down");
      } 
			else if (scancode <= 0x39 + 0x80) {
        printk("key up ");
        print_letter(scancode - 0x80);
      } 
			else {
				printk("Unknown key up");
			}
      break;
  }
}

