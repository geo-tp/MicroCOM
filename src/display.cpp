#include "display.h"

void displayInit() {
    // Initialize display
   M5.Lcd.begin();
   M5.Lcd.setRotation(1);
   M5.Lcd.setTextColor(TFT_LIGHTGREY);
}

void displayWelcome() {
    M5.Lcd.fillScreen(BACKGROUND_COLOR);

    // Main title
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(PRIMARY_COLOR);
    M5.Lcd.setCursor(50, 50);
    M5.Lcd.printf("Micro");

    M5.Lcd.setTextSize(2.5);
    M5.Lcd.setTextColor(TEXT_COLOR);
    M5.Lcd.setCursor(146, 54);
    M5.Lcd.printf("COM");

    // Sub title
    M5.Lcd.setTextSize(1.2);
    M5.Lcd.setTextColor(TEXT_COLOR);
    M5.Lcd.setCursor(50, 78);
    M5.Lcd.printf("Serial");

    M5.Lcd.setTextSize(1.3);
    M5.Lcd.setCursor(98, 78);
    M5.Lcd.printf("Communication");

};

void displayConfig(int baud, uint8_t rx, uint8_t tx, uint8_t dataBits, 
                   std::string parity, uint8_t stopBits, bool flowControl, 
                   bool inverted, uint8_t selectedIndex) {

    std::vector<std::string> configStrings = {
        "Rx Pin: " + std::to_string(rx),
        "Tx Pin: " + std::to_string(tx),
        "Baud: " + std::to_string(baud),
        "Data Bits: " + std::to_string(dataBits),
        "Parity: " + parity,
        "Stop Bits: " + std::to_string(stopBits),
        "Flow Ctrl: " + std::string(flowControl ? "Yes" : "No"),
        "Inverted: " + std::string(inverted ? "Yes" : "No")
    };

    uint8_t halfWidth = (M5Cardputer.Display.width() - 20) / 2; // largeur de chaque bloc dans une colonne
    uint8_t sizeY = 22;  // hauteur de chaque bloc
    uint8_t startY = 5; // position de départ en Y pour le premier bloc
    uint8_t stepY = 26;  // intervalle entre chaque bloc
    uint8_t margin = DEFAULT_MARGIN;
    uint8_t startText = 13;  // position de départ du texte
    bool selected;

    displayClearMainView();
   M5.Lcd.setTextSize(1.3);

    for (size_t i = 0; i < configStrings.size(); ++i) {
        selected = (i == selectedIndex);

        // Calculer la position en X pour la colonne (gauche ou droite)
        uint8_t col = i / 4; // 0 pour la colonne de gauche, 1 pour la colonne de droite
        uint8_t xPos = margin + col * (halfWidth + margin);

        // Calculer la position en Y pour chaque ligne
        uint8_t yPos = startY + (i % 4) * stepY;
        
        drawRect(selected, xPos, yPos, halfWidth, sizeY);

        uint8_t marginText = margin + 5;
       M5.Lcd.setCursor(xPos + marginText, yPos + startText - startY);

       M5.Lcd.printf(configStrings[i].c_str());
    }

    if (selectedIndex == configStrings.size()) {
        displayStart(true);
    } else {
        displayStart(false);
    }
}

void displayStart(bool selected) {
    drawRect(selected, DEFAULT_MARGIN, 110,M5.Lcd.width() - 15, 25);
   M5.Lcd.setCursor(79, 118);
   M5.Lcd.print("START SERIAL");
};

void displayTerminal(std::string receiveString) {
    const uint8_t charsPerLine = 34;
    const uint8_t linesPerScreen = 10;

    // Split receiveString by \n and wrap text
    std::vector<std::string> lines;
    std::string currentLine;
    for (size_t i = 0; i < receiveString.length(); ++i) {
        char c = receiveString[i];
        if (c == '\n') {
            lines.push_back(currentLine);  // Add the current line and reset
            currentLine.clear();
        } else {
            currentLine += c;
            // If the current line exceeds the allowed number of characters per line, wrap it
            if (currentLine.length() >= charsPerLine) {
                lines.push_back(currentLine);
                currentLine.clear();
            }
        }
    }
    // Push the last remaining part of the string (if any)
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    // Now, calculate the number of lines and only display the last ones that fit on the screen
    size_t totalLines = lines.size();
    size_t startLine = 0;
    if (totalLines > linesPerScreen) {
        startLine = totalLines - linesPerScreen;
    }

    // Clear the terminal view before displaying the new content
    displayClearTerminalView();
   M5.Lcd.setCursor(0, DEFAULT_MARGIN);

    // Print only the visible portion of the terminal string
    for (size_t i = startLine; i < totalLines; ++i) {
       M5.Lcd.println(lines[i].c_str());
    }
}


void displayPrompt(std::string sendString) {

   if (sendString.length() > 26) {
        sendString = sendString.substr(sendString.length() - 26);
    }
    drawRect(false, DEFAULT_MARGIN, 110,M5.Lcd.width() - 15, 25);
   M5.Lcd.setCursor(DEFAULT_MARGIN*2, 118);
   M5.Lcd.print(" > ");
   M5.Lcd.print(sendString.c_str());
}

void drawRect(bool selected, uint8_t margin, uint16_t startY, uint16_t sizeX, uint16_t sizeY) {
    // Draw rect
    if (selected) {
       M5.Lcd.fillRoundRect(margin, startY, sizeX, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
       M5.Lcd.setTextColor(TEXT_COLOR);
    } else {
       M5.Lcd.fillRoundRect(margin, startY, sizeX, sizeY, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
       M5.Lcd.drawRoundRect(margin, startY, sizeX, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
       M5.Lcd.setTextColor(TEXT_COLOR);
    }
}

void displayClearMainView(uint8_t offsetY) {
   M5.Lcd.fillRect(0, 0,M5.Lcd.width(),M5.Lcd.height(), BACKGROUND_COLOR);
}

void displayClearTerminalView() {
   M5.Lcd.fillRect(0, 0,M5.Lcd.width(),M5.Lcd.height()-30, BACKGROUND_COLOR);
}
