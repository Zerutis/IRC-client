#pragma once
volatile bool running = false;

void signalHandler(int signal) {
    running = false;
}