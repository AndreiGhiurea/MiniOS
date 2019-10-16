#include "keyboard_map.h"

CHAR GetCharFromScancode(BYTE Code)
{
    CHAR Key = 0x0;

    switch (Code)
    {
    case 0x01:
        Key = 0x01; // Escape Pressed
        break;
    case 0x02:
        Key = '1';
        break;
    case 0x03:
        Key = '2';
        break;
    case 0x04:
        Key = '3';
        break;
    case 0x05:
        Key = '4';
        break;
    case 0x06:
        Key = '5';
        break;
    case 0x07:
        Key = '6';
        break;
    case 0x08:
        Key = '7';
        break;
    case 0x09:
        Key = '8';
        break;
    case 0x0A:
        Key = '9';
        break;
    case 0x0B:
        Key = '0';
        break;
    case 0x0C:
        Key = '-';
        break;
    case 0x0D:
        Key = '=';
        break;
    case 0x0E:
        Key = 0x0E; // Backspace Pressed
        break;
    case 0x0F:
        Key = 0x0F; // Tab Pressed
        break;
    case 0x10:
        Key = 'q';
        break;
    case 0x11:
        Key = 'w';
        break;
    case 0x12:
        Key = 'e';
        break;
    case 0x13:
        Key = 'r';
        break;
    case 0x14:
        Key = 't';
        break;
    case 0x15:
        Key = 'y';
        break;
    case 0x16:
        Key = 'u';
        break;
    case 0x17:
        Key = 'i';
        break;
    case 0x18:
        Key = 'o';
        break;
    case 0x19:
        Key = 'p';
        break;
    case 0x1A:
        Key = '[';
        break;
    case 0x1B:
        Key = ']';
        break;
    case 0x1C:
        Key = 0x1C; // Enter Pressed
        break;
    case 0x1D:
        Key = 0x1D; // Left Control Pressed
        break;
    case 0x1E:
        Key = 'a';
        break;
    case 0x1F:
        Key = 's';
        break;
    case 0x20:
        Key = 'd';
        break;
    case 0x21:
        Key = 'f';
        break;
    case 0x22:
        Key = 'g';
        break;
    case 0x23:
        Key = 'h';
        break;
    case 0x24:
        Key = 'j';
        break;
    case 0x25:
        Key = 'k';
        break;
    case 0x26:
        Key = 'l';
        break;
    case 0x27:
        Key = ';';
        break;
    case 0x28:
        Key = '\'';
        break;
    case 0x29:
        Key = '`';
        break;
    case 0x2A:
        Key = 0x2A; // Left Shift Pressed
        break;
    case 0x2B:
        Key = '\\';
        break;
    case 0x2C:
        Key = 'z';
        break;
    case 0x2D:
        Key = 'x';
        break;
    case 0x2E:
        Key = 'c';
        break;
    case 0x2F:
        Key = 'v';
        break;
    case 0x30:
        Key = 'b';
        break;
    case 0x31:
        Key = 'n';
        break;
    case 0x32:
        Key = 'm';
        break;
    case 0x33:
        Key = ',';
        break;
    case 0x34:
        Key = '.';
        break;
    case 0x35:
        Key = '/';
        break;
    case 0x36:
        Key = 0x36; // Right Shift Pressed
        break;
    case 0x37:
        Key = '*'; // * Keypad Pressed
        break;
    case 0x38:
        Key = 0x38; // Left Alt Pressed
        break;
    case 0x39:
        Key = ' ';
        break;
    case 0x3A:
        Key = 0x3A; // Caps Lock Pressed
        break;
    case 0x3B:
        Key = 0x3B; // F1 Pressed
        break;
    case 0x3C:
        Key = 0x3C; // F2 Pressed
        break;
    case 0x3D:
        Key = 0x3D; // F3 Pressed
        break;
    case 0x3E:
        Key = 0x3E; // F4 Pressed
        break;
    case 0x3F:
        Key = 0x3F; // F5 Pressed
        break;
    case 0x40:
        Key = 0x40; // F6 Pressed
        break;
    case 0x41:
        Key = 0x41; // F7 Pressed
        break;
    case 0x42:
        Key = 0x42; // F8 Pressed
        break;
    case 0x43:
        Key = 0x43; // F9 Pressed
        break;
    case 0x44:
        Key = 0x44; // F10 Pressed
        break;
    case 0x45:
        Key = 0x45; // NumLock Pressed
        break;
    case 0x46:
        Key = 0x46; // ScrollLock Pressed
        break;
    case 0x47:
        Key = '7'; // NumPad
        break;
    case 0x48:
        Key = '8'; // NumPad
        break;
    case 0x49:
        Key = '9'; // NumPad
        break;
    case 0x4A:
        Key = '-'; // NumPad
        break;
    case 0x4B:
        Key = '4'; // NumPad
        break;
    case 0x4C:
        Key = '5'; // NumPad
        break;
    case 0x4D:
        Key = '6'; // NumPad
        break;
    case 0x4E:
        Key = '+'; // NumPad
        break;
    case 0x4F:
        Key = '1'; // NumPad
        break;
    case 0x50:
        Key = '2'; //NumPad
        break;
    case 0x51:
        Key = '3'; // NumPad
        break;
    case 0x52:
        Key = '0'; // NumPad
        break;
    case 0x53:
        Key = '.'; // NumPad
        break;
    case 0x57:
        Key = 0x57; // F11 Pressed
        break;
    case 0x58:
        Key = 0x58; // F12 Pressed
        break;
    default:
        Key = 0x0; // Unknown scancode
        break;
    }

    return Key;
}