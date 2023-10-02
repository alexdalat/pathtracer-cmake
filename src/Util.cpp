
#include "Util.h"

static std::string Util::getFileTimestamp() {
    // Fetch the current time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert to local time
    std::tm localTime;
    localtime_r(&currentTime, &localTime);

    // Format the timestamp
    char buffer[32];
    strftime(buffer, sizeof(buffer), ":%Y-%m-%d_%H-%M-%S", &localTime);

    return std::string(buffer);
}
