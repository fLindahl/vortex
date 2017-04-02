#pragma once

class Console
{
    public:
        Console();
        ~Console();

        const char* GetLog();
        void Print(const char* s, ...);
    private:

};