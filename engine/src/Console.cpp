#include "Cute/Console.h"
#include "Cute/String.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

#if defined(_WIN32)
// FOREGROUND_INTENSITY:前景高亮显示,BACKGROUND_INTENSITY 背景高亮显示
static WORD WIN_COLOR_FOREGROUND[Console::COLOR_MAX] =
{
    0,                                                          // BLACK
    FOREGROUND_RED,                                             // RED
    FOREGROUND_GREEN,                                           // GREEN
    FOREGROUND_RED | FOREGROUND_GREEN,                          // BROWN
    FOREGROUND_BLUE,                                            // BLUE
    FOREGROUND_RED | FOREGROUND_BLUE,                           // MAGENTA
    FOREGROUND_GREEN | FOREGROUND_BLUE,                         // CYAN
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,        // WHITE
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,   // YELLOW
    FOREGROUND_INTENSITY | FOREGROUND_RED,                      // RED_BOLD
    FOREGROUND_INTENSITY | FOREGROUND_GREEN,                    // GREEN_BOLD
    FOREGROUND_INTENSITY | FOREGROUND_BLUE,                     // BLUE_BOLD
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,    // MAGENTA_BOLD
    FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,  // CYAN_BOLD
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE     // WHITE_BOLD
};
#else
enum AnsiTextAttr
{
    TA_NORMAL   = 0,
    TA_BOLD     = 1,
    TA_BLINK    = 5,
    TA_REVERSE  = 7
};
// 前景色
enum AnsiFGTextAttr
{
    FG_BLACK = 30, FG_RED, FG_GREEN, FG_BROWN, FG_BLUE,
    FG_MAGENTA, FG_CYAN, FG_WHITE, FG_YELLOW
};
// 背景色
enum AnsiBGTextAttr
{
    BG_BLACK = 40, BG_RED, BG_GREEN, BG_BROWN, BG_BLUE,
    BG_MAGENTA, BG_CYAN, BG_WHITE, BG_YELLOW
};

// 前景色对应数组
static int UNIX_COLOR_FOREGROUND[Console::COLOR_MAX] =
{
    FG_BLACK,                                           // BLACK
    FG_RED,                                             // RED
    FG_GREEN,                                           // GREEN
    FG_BROWN,                                           // BROWN
    FG_BLUE,                                            // BLUE
    FG_MAGENTA,                                         // MAGENTA
    FG_CYAN,                                            // CYAN
    FG_WHITE,                                           // WHITE
    FG_YELLOW,                                          // YELLOW
    FG_RED,                                             // LRED
    FG_GREEN,                                           // LGREEN
    FG_BLUE,                                            // LBLUE
    FG_MAGENTA,                                         // LMAGENTA
    FG_CYAN,                                            // LCYAN
    FG_WHITE                                            // LWHITE
};
#endif

void Console::setColor(Color color, bool stdout_stream)
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
    SetConsoleTextAttribute(hConsole, WIN_COLOR_FOREGROUND[color]);
#else
    fprintf(stdout_stream ? stdout : stderr, "\x1b[%d%sm", UNIX_COLOR_FOREGROUND[color], (color >= YELLOW ? ";1" : ""));
#endif
}

void Console::resetColor(bool stdout_stream)
{
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
    fprintf((stdout_stream ? stdout : stderr), "\x1b[0m");
#endif
}

Console::Color Console::parseColor(const String& color)
{
    static const EnumPair COLOR_MAP[] = {
        {"default", DEFAULT},
        {"black",   BLACK},
        {"red",     RED},
        {"green",   GREEN},
        {"brown",   BROWN},
        {"blue",    BLUE},
        {"magenta", MAGENTA},
        {"cyan",    CYAN},
        {"gray",    GRAY},
        {"lred",    LRED},
        {"lgreen",  LGREEN},
        {"lblue",   LBLUE},
        {"lmagenta",LMAGENTA},
        {"lcyan",   LCYAN},
        {"yellow",  YELLOW},
        {"white",   WHITE},
        {NULL,      0},
    };
    
    for(int i = 0; i < INT32_MAX; ++i)
    {
        if(COLOR_MAP[i].name == NULL){
            return DEFAULT;
//            throw InvalidArgumentException("Invalid color value", color);
        }
        
        if(color.iequals(COLOR_MAP[i].name))
            return (Color)COLOR_MAP[i].value;
    }
    
    return DEFAULT;
}

String Console::formatColor(Color color)
{
    switch (color)
    {
    case BLACK:    return "black";
    case RED:      return "red";
    case GREEN:    return "green";
    case BROWN:    return "brown";
    case BLUE:     return "blue";
    case MAGENTA:  return "magenta";
    case CYAN:     return "cyan";
//  case GRAY:     return "gray";
//  case DARKGRAY: return "darkGray";
    case LRED:     return "lred";
    case LGREEN:   return "lgreen";
    case YELLOW:   return "yellow";
    case LBLUE:    return "lblue";
    case LMAGENTA: return "lmagenta";
    case LCYAN:    return "lcyan";
    case WHITE:    return "white";
    default:       return "invalid";
    }
}

CUTE_NS_END
