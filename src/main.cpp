// Multiclicker by Ketaslava Ket and KTVINCCO Team
// Licensed under MIT License


#include <libinput.h>
#include <linux/input.h>
#include <fcntl.h>
#include <ostream>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <random>
#include <fstream>


// SETTINGS


// Log
bool is_enable_click_logging = false; // false (May cause performance issues)

// Device
bool is_read_device_name_from_file = true;
std::string deviceName = "USB OPTICAL MOUSE";

// Trigger
int maximumTriggerTimeFor4ClicksMs = 666; // 666
bool isUse3ClicksInsteadOf4 = false; // false
int maximumTriggerTimeFor3ClicksMs = 500; // 500
bool is_use_optimized_continuation_rule = true; // false (May cause input loops)

// Clicks
int additionalClicksCountFrom = 8; // 8
int additionalClicksCountTo = 12; // 12
int timeBetweenClicksFromMs = 28; // 32
int timeBetweenClicksToMs = 34; // 42


// VARIABLES


int currentButtonId = 0;
char *device_path; // Mouse device path
bool isEmulationRunning = false;
bool isLastAdditionalClickEmited = false;
int emulated_clicks_count = 0;
int clicks_per_emulation = 0;


// BASE


int getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}


void clickProcessLog(std::string msg) {
    if (is_enable_click_logging) {
        std::cout << msg << std::endl;
    }
}


// EMIT INPUT


void emit(int fd, int type, int code, int value) {
    // Write information about device action to device file
    struct input_event ie;
    ie.type = type;
    ie.code = code;
    ie.value = value;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
    write(fd, &ie, sizeof(ie));
}


void emitClick(int fd, int buttonId) {
    // Emulate click

    emit(fd, EV_KEY, buttonId, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);

    emit(fd, EV_KEY, buttonId, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}


// MAIN - PROCESS CLICKS


void emulateAdditionalClicks() {
    // Emulate a series of additional clicks

    clickProcessLog("Emulate additional clicks");

    // Open device file
    int fd = open(device_path, O_WRONLY);
    if (fd < 0) {
        std::cerr << "Error: Cannot open input device" << std::endl;
    }

    // Set state
    isEmulationRunning = true;
    emulated_clicks_count = 0;
    clicks_per_emulation = 1; // 1 is reserved for last click

    // Emulate clicks
    int clicksCount = 0;
    int additionalClicksCount = getRandomInt(additionalClicksCountFrom, additionalClicksCountTo);
    while (clicksCount < additionalClicksCount) {
        std::this_thread::sleep_for(std::chrono::milliseconds(
            getRandomInt(timeBetweenClicksFromMs, timeBetweenClicksToMs)));
        emitClick(fd, currentButtonId);
        clicksCount ++;
        emulated_clicks_count ++;
    }

    // Set state
    isEmulationRunning = false;
    isLastAdditionalClickEmited = true;

    // Close device file
    close(fd);
}


std::chrono::milliseconds lastClicks[4] = { std::chrono::milliseconds(0),
    std::chrono::milliseconds(0), std::chrono::milliseconds(0), std::chrono::milliseconds(0)};

bool checkTriggerRule() {
    // Check if it clicks quickly

    // Get time
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch());

    // Move array
    lastClicks[3] = lastClicks[2];
    lastClicks[2] = lastClicks[1];
    lastClicks[1] = lastClicks[0];
    lastClicks[0] = ms;

    // Check
    if (isUse3ClicksInsteadOf4) {
        // Time between 3 clicks
        return (lastClicks[0] - lastClicks[2]) < std::chrono::milliseconds(maximumTriggerTimeFor3ClicksMs);
    } else {
        // Time between 4 clicks
        return (lastClicks[0] - lastClicks[3]) < std::chrono::milliseconds(maximumTriggerTimeFor4ClicksMs);
    }
}


void processClick(int button_id) {

    clickProcessLog("Process click");

    // Check trigger rule
    if (!checkTriggerRule() && !isEmulationRunning && !isLastAdditionalClickEmited) { return; }
    clickProcessLog("PASS TRIGGER");

    // Check state
    if (isEmulationRunning) {
        // Count clicks
        clicks_per_emulation ++;
        // Exit
        return;
    }

    if (isLastAdditionalClickEmited) {
        // Reset state
        isLastAdditionalClickEmited = false;
        // Check continue state
        clickProcessLog("CpE: " + std::to_string(clicks_per_emulation) + "ECC:" + std::to_string(emulated_clicks_count));
        if (clicks_per_emulation == emulated_clicks_count || !is_use_optimized_continuation_rule) {
            return;
            clickProcessLog("STOP");
        } else {
            clickProcessLog("CONTINUE");
        }
    }

    // Assign button to emulate
    currentButtonId = button_id;

    // Emulate advanced clicks (async)
    std::thread bt(emulateAdditionalClicks);
    bt.detach();
}


// DETECT


static int open_restricted(const char *path, int flags, void *user_data) {
    int fd = open(path, flags);
    if (fd < 0)
        std::cerr << "Failed to open " << path << std::endl;
    return fd;
}

static void close_restricted(int fd, void *user_data) {
    close(fd);
}

static const struct libinput_interface interface = {
    open_restricted,
    close_restricted,
};


// FIND DEVICE


std::string execCommand(const char* cmd) {
    // Execute command in terminal and get result
    std::array<char, 128> buffer;
    std::string result;
    using PipeDeleter = int(*)(FILE*);  // Explicitly declare the type
    std::unique_ptr<FILE, PipeDeleter> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


std::string removeSpacesAndLineBreaks(const std::string _str) {
    std::string str = _str;
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {
        return std::isspace(static_cast<unsigned char>(c));
    }), str.end());
    return str;
}


std::string findDevicePathByName(const std::string& deviceName) {
    // Find device path as "/dev/input/event1" by device name as "USB OPTICAL MOUSE"

    // Execute lsinput
    std::string output = execCommand("lsinput");

    // Process lsinput result
    std::istringstream stream(output);
    std::string line;
    std::string devicePath;

    while (std::getline(stream, line)) {
        if (line.find("/dev/input/") == 0) {
            devicePath = line;
        } else if (line.find("name") != std::string::npos) {
            std::size_t startPos = line.find("\"") + 1;
            std::size_t endPos = line.rfind("\"");
            if (startPos != std::string::npos && endPos != std::string::npos && endPos > startPos) {
                std::string name = line.substr(startPos, endPos - startPos);
                if (removeSpacesAndLineBreaks(name) == removeSpacesAndLineBreaks(deviceName)) {
                    return devicePath; // Return path when device is found
                }
            }
        }
    }

    return ""; // Return empty string if device not found
}


// MAIN - LOOP


int main() {

    // Read settings

    if (is_read_device_name_from_file) {
        std::fstream MyReadFile("./config/device_name.txt");
        std::string txt;
        if(getline (MyReadFile, txt)) {
            deviceName = txt;
        } else {
            std::cout << "WARNING: Can not read a configuration file for Device Name" << std::endl;
        }
        MyReadFile.close();
    }

    // Log
    std::cout << "Device Name: " << deviceName << std::endl;

    // Setup data

    std::string devicePath = findDevicePathByName(deviceName);

    if (!devicePath.empty()) {
        std::cout << "Device path: " << devicePath << std::endl;
        device_path = new char[devicePath.size() + 1]; // Allocate memory
        std::strcpy(device_path, devicePath.c_str()); // Copy the string
    } else {
        std::cout << "ERROR: Device not found. Check device name in configuration" << std::endl;
        return 0;
    }

    // Setup input

    struct libinput *li;
    struct libinput_event *event;

    li = libinput_path_create_context(&interface, nullptr);
    if (!li) {
        std::cerr << "Failed to initialize libinput" << std::endl;
        return 1;
    }

    // Open the input device
    struct libinput_device *device = libinput_path_add_device(li, device_path);
    if (!device) {
        std::cerr << "Failed to open device" << std::endl;
        libinput_unref(li);
        return 1;
    }

    // Ready

    std::cerr << "========" << std::endl;
    std::cerr << "Multiclicker is ready!" << std::endl;
    std::cerr << "Created by Ketaslava Ket" << std::endl;
    std::cerr << "KTVINCCO [ktvincco.com]" << std::endl;
    std::cerr << "========" << std::endl;

    // Main loop

    while (true) {

        bool is_was_input = false;
        int last_button_id = 0;

        // Getting input
        libinput_dispatch(li);
        while ((event = libinput_get_event(li)) != nullptr) {
            if (libinput_event_get_type(event) == LIBINPUT_EVENT_POINTER_BUTTON) {
                struct libinput_event_pointer *p = libinput_event_get_pointer_event(event);
                uint32_t button = libinput_event_pointer_get_button(p);
                uint32_t state = libinput_event_pointer_get_button_state(p);

                if (state == LIBINPUT_BUTTON_STATE_PRESSED) {
                    clickProcessLog("Mouse click: button " + std::to_string(button) +" pressed");
                } else if (state == LIBINPUT_BUTTON_STATE_RELEASED) {
                    clickProcessLog("Mouse click: button " + std::to_string(button) +" released");
                    is_was_input = true;
                    last_button_id = button;
                }
            }
            libinput_event_destroy(event);
        }

        // Process input
        if (is_was_input) {
            processClick(last_button_id);
        }
    }

    // Exit

    libinput_unref(li);

    return 0;
}
