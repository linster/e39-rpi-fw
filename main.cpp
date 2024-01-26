#include "factory/pimoroniFactory/PimoroniFactory.h"
#include "startupManager/StartupManager.h"

// The built in LED
#define LED_PIN 25

int main() {

    auto* factory = new pico::di::PimoroniFactory();

    pico::ApplicationContainer* applicationContainer = factory->getApplicationContainer();

    auto* startupManager = new pico::startupManager::StartupManager(applicationContainer);

    startupManager->cpu0main();
}
