#pragma once

class Util {
    std::string getFileTimestamp();

    // thread local fast rng(seed)
    static thread_local std::mt19937 rng;
};
