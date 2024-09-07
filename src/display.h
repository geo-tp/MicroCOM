#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <string>
#include <M5Cardputer.h> // Assurez-vous que cette bibliothèque est la bonne selon votre matériel (M5Stack, M5Cardputer, etc.)

#define BACKGROUND_COLOR TFT_BLACK
#define PRIMARY_COLOR 0xfa03
#define RECT_COLOR_DARK 0x0841
#define RECT_COLOR_LIGHT 0xd69a
#define TEXT_COLOR 0xef7d
#define TEXT_COLOR_ALT TFT_DARKGRAY

#define DEFAULT_MARGIN 5
#define DEFAULT_ROUND_RECT 5

void displayInit();
void displayWelcome();
void displayConfig(int baud, uint8_t rx, uint8_t tx, uint8_t dataBits, 
                   std::string parity, uint8_t stopBits, bool flowControl, 
                   bool inverted, uint8_t selectedIndex);
void displayStart(bool selected);
void displayTerminal(std::string terminalSting);
void displayPrompt(std::string sendString);
void displayClearMainView(uint8_t offsetY=0);
void displayClearTerminalView();

// Utility Function (forward declaration if needed)
void drawRect(bool selected, uint8_t margin, uint16_t startY, uint16_t sizeX, uint16_t sizeY);

#endif // DISPLAY_CONFIG_H
