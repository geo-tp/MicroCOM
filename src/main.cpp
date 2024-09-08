#include <M5Cardputer.h>
#include <thread>
#include <atomic>

#include "input.h"
#include "display.h"
#include "serial.h"
#include "event.h"

// Config
BaudRate baudRate = BAUD_9600;
uint8_t rxPin = 1;
uint8_t txPin = 2;
uint8_t dataBits = 8;
ParityType parity = NONE;
uint8_t stopBits = 1;
bool flowControl = false;
bool inverted = false;
uint8_t selectedIndex = LAUNCH_INDEX;

// Var atomic for input thread
std::atomic<bool> sendDataFlag(false);
std::atomic<bool> running(true);

// Strings for serial send/receive
std::string sendString;
std::string receiveString;

// Lock sendString for thread safe purpose
std::mutex sendMutex;

void config() {
  bool firstRender = true;
  while (true) {
    char input = configInputHandler();
    selectedIndex = handleIndexSelection(input, selectedIndex);
    handleConfigSelection(input, baudRate, rxPin, txPin, dataBits, parity, stopBits, flowControl, inverted, selectedIndex);
        
    if (input != KEY_NONE || firstRender) {
      displayConfig(baudRateToInt(baudRate), rxPin, txPin, dataBits, parityToString(parity), stopBits, flowControl, inverted, selectedIndex);
      firstRender = false;
    }

    // If user presses the start button, we leave config screen
    if (input == KEY_OK && selectedIndex == LAUNCH_INDEX) {
      displayClearMainView();
      running = true;
      break;
    }
  }
}

void terminal() {
  int16_t promptSize = -1;
  int16_t terminalSize = -1;
  
  const int maxReadSize = 512;
  char buffer[maxReadSize + 1];

  while (running) {

    if (Serial1.available()) {
      int bytesRead = Serial1.readBytes(buffer, maxReadSize);
      buffer[bytesRead] = '\0';
      receiveString += std::string(buffer);
      delay(20);
    }

    else if (sendDataFlag) {
      Serial1.println(sendString.c_str());
      sendDataFlag = false;
      std::lock_guard<std::mutex> lock(sendMutex);
      sendString.clear();
    }

    if (terminalSize != receiveString.size()) {
      displayTerminal(receiveString);
      terminalSize = receiveString.size();
    }

    if (promptSize != sendString.size()) {
      displayPrompt(sendString);
      promptSize = sendString.size();
    }
  }
}

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg);

  displayInit();
  displayWelcome();
  delay(2000);

  // Serial config
  config();
  auto serialConfig = serialGetConfig(dataBits, parity, stopBits, flowControl);
  Serial.begin(9600); // for some reason, we have to init Serial to make Serial1 works
  Serial1.begin(baudRateToInt(baudRate), serialConfig, rxPin, txPin);

  // Prompt thread
  std::thread inputThread(handlePrompt, std::ref(sendDataFlag), std::ref(sendString), 
                          std::ref(running), std::ref(sendMutex));
  inputThread.detach();
}

void loop() {
  terminal();
}