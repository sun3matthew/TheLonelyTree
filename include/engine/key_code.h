#ifndef KEY_CODE_H
#define KEY_CODE_H

#include <string>

enum class KeyCode {
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,  /* ' */
    KEY_COMMA = 44,  /* , */
    KEY_MINUS = 45,  /* - */
    KEY_PERIOD = 46,  /* . */
    KEY_SLASH = 47,  /* / */
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,  /* ; */
    KEY_EQUAL = 61,  /* = */
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_BRACKET = 91,  /* [ */
    KEY_BACKSLASH = 92,  /* \ */
    KEY_RIGHT_BRACKET = 93,  /* ] */
    KEY_GRAVE_ACCENT = 96,  /* ` */
    KEY_WORLD_1 = 161, /* non-US #1 */
    KEY_WORLD_2 = 162, /* non-US #2 */
    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,
    KEY_F13 = 302,
    KEY_F14 = 303,
    KEY_F15 = 304,
    KEY_F16 = 305,
    KEY_F17 = 306,
    KEY_F18 = 307,
    KEY_F19 = 308,
    KEY_F20 = 309,
    KEY_F21 = 310,
    KEY_F22 = 311,
    KEY_F23 = 312,
    KEY_F24 = 313,
    KEY_F25 = 314,
    KEY_KP_0 = 320,
    KEY_KP_1 = 321,
    KEY_KP_2 = 322,
    KEY_KP_3 = 323,
    KEY_KP_4 = 324,
    KEY_KP_5 = 325,
    KEY_KP_6 = 326,
    KEY_KP_7 = 327,
    KEY_KP_8 = 328,
    KEY_KP_9 = 329,
    KEY_KP_DECIMAL = 330,
    KEY_KP_DIVIDE = 331,
    KEY_KP_MULTIPLY = 332,
    KEY_KP_SUBTRACT = 333,
    KEY_KP_ADD = 334,
    KEY_KP_ENTER = 335,
    KEY_KP_EQUAL = 336,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_MENU = 348,
    MAX_KEYS
};

inline std::string KeyCodeToString(KeyCode keyCode) {
    switch (keyCode) {
        case KeyCode::KEY_SPACE: return " ";
        case KeyCode::KEY_APOSTROPHE: return "'";
        case KeyCode::KEY_COMMA: return ",";
        case KeyCode::KEY_MINUS: return "-";
        case KeyCode::KEY_PERIOD: return ".";
        case KeyCode::KEY_SLASH: return "/";
        case KeyCode::KEY_0: return "0";
        case KeyCode::KEY_1: return "1";
        case KeyCode::KEY_2: return "2";
        case KeyCode::KEY_3: return "3";
        case KeyCode::KEY_4: return "4";
        case KeyCode::KEY_5: return "5";
        case KeyCode::KEY_6: return "6";
        case KeyCode::KEY_7: return "7";
        case KeyCode::KEY_8: return "8";
        case KeyCode::KEY_9: return "9";
        case KeyCode::KEY_SEMICOLON: return ";";
        case KeyCode::KEY_EQUAL: return "=";
        case KeyCode::KEY_A: return "A";
        case KeyCode::KEY_B: return "B";
        case KeyCode::KEY_C: return "C";
        case KeyCode::KEY_D: return "D";
        case KeyCode::KEY_E: return "E";
        case KeyCode::KEY_F: return "F";
        case KeyCode::KEY_G: return "G";
        case KeyCode::KEY_H: return "H";
        case KeyCode::KEY_I: return "I";
        case KeyCode::KEY_J: return "J";
        case KeyCode::KEY_K: return "K";
        case KeyCode::KEY_L: return "L";
        case KeyCode::KEY_M: return "M";
        case KeyCode::KEY_N: return "N";
        case KeyCode::KEY_O: return "O";
        case KeyCode::KEY_P: return "P";
        case KeyCode::KEY_Q: return "Q";
        case KeyCode::KEY_R: return "R";
        case KeyCode::KEY_S: return "S";
        case KeyCode::KEY_T: return "T";
        case KeyCode::KEY_U: return "U";
        case KeyCode::KEY_V: return "V";
        case KeyCode::KEY_W: return "W";
        case KeyCode::KEY_X: return "X";
        case KeyCode::KEY_Y: return "Y";
        case KeyCode::KEY_Z: return "Z";
        case KeyCode::KEY_LEFT_BRACKET: return "[";
        case KeyCode::KEY_BACKSLASH: return "\\";
        case KeyCode::KEY_RIGHT_BRACKET: return "]";
        case KeyCode::KEY_GRAVE_ACCENT: return "`";
        case KeyCode::KEY_ESCAPE: return "";
        case KeyCode::KEY_ENTER: return "";
        case KeyCode::KEY_TAB: return "";
        case KeyCode::KEY_BACKSPACE: return "";
        case KeyCode::KEY_INSERT: return "";
        case KeyCode::KEY_DELETE: return "";
        case KeyCode::KEY_RIGHT: return "";
        case KeyCode::KEY_LEFT: return "";
        case KeyCode::KEY_DOWN: return "";
        case KeyCode::KEY_UP: return "";
        case KeyCode::KEY_F1: return "";
        case KeyCode::KEY_F2: return "";
        case KeyCode::KEY_F3: return "";
        case KeyCode::KEY_F4: return "";
        case KeyCode::KEY_F5: return "";
        case KeyCode::KEY_F6: return "";
        case KeyCode::KEY_F7: return "";
        case KeyCode::KEY_F8: return "";
        case KeyCode::KEY_F9: return "";
        case KeyCode::KEY_F10: return "";
        case KeyCode::KEY_F11: return "";
        case KeyCode::KEY_F12: return "";
        case KeyCode::KEY_LEFT_SHIFT: return "";
        case KeyCode::KEY_RIGHT_SHIFT: return "";
        case KeyCode::KEY_LEFT_CONTROL: return "";
        case KeyCode::KEY_RIGHT_CONTROL: return "";
        case KeyCode::KEY_LEFT_ALT: return "";
        case KeyCode::KEY_RIGHT_ALT: return "";
        case KeyCode::KEY_LEFT_SUPER: return "";
        case KeyCode::KEY_RIGHT_SUPER: return "";
        case KeyCode::KEY_MENU: return "";
        default: return "";
    }
}


#endif // KEY_CODE_H
