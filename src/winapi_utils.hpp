#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

#include <fcntl.h>
#include <io.h>
#include <windows.h>


class ScopedWinHandle {
public:
    ScopedWinHandle() = default;
    explicit ScopedWinHandle(HANDLE handle) noexcept : handle{handle} { }

    ScopedWinHandle(const ScopedWinHandle&) = delete;
    ScopedWinHandle& operator=(const ScopedWinHandle&) = delete;

    ScopedWinHandle(ScopedWinHandle&& other) noexcept : handle{other.handle} {
        other.handle = nullptr;
    }

    ScopedWinHandle& operator=(ScopedWinHandle&& other) noexcept {
        if(this != std::addressof(other)) {
            close();
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    ~ScopedWinHandle() {
        close();
    }

    void close() noexcept {
        if(handle != nullptr && handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
            handle = nullptr;
        }
    }

    HANDLE* operator&() noexcept {
        close();
        return &handle;
    }

    HANDLE get() const noexcept {
        return handle;
    }

    HANDLE release() noexcept {
        HANDLE tempHandle = handle;
        handle = nullptr;
        return tempHandle;
    }

private:
    HANDLE handle = nullptr; 
};


// placeholder type indicating that the return type of the callable was void
struct VoidReturnTypeWrapper {};

template<typename Callable>
auto invokeWithStdOutRedirectedToStr(Callable callable) {

    const auto exitWithError = []() {
        std::cerr << "An error occurred while setting up standard output redirection. Exiting.\n";
        std::exit(EXIT_FAILURE);
    };

    const auto originalStdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    ScopedWinHandle readPipeHandle, writePipeHandle;
    SECURITY_ATTRIBUTES securityAttributes = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if(!CreatePipe(&readPipeHandle, &writePipeHandle, &securityAttributes, 0)) {
        exitWithError();
    }
    if(!SetStdHandle(STD_OUTPUT_HANDLE, writePipeHandle.get())) {
        exitWithError();
    }

    const auto originalStdOutFd = _dup(_fileno(stdout));
    const auto writePipeFd = _open_osfhandle(reinterpret_cast<intptr_t>(writePipeHandle.release()), O_WRONLY | O_TEXT);
    _dup2(writePipeFd, _fileno(stdout));
    _close(writePipeFd);

    decltype(auto) returnValue = [&callable]() -> decltype(auto) {
        if constexpr (std::is_void_v<std::invoke_result_t<Callable>>) {
            callable();
            return VoidReturnTypeWrapper{};
        } else {
            return callable();
        }
    }();

    fflush(stdout);

    _dup2(originalStdOutFd, _fileno(stdout));
    _close(originalStdOutFd);

    if(!SetStdHandle(STD_OUTPUT_HANDLE, originalStdOutHandle)) {
        exitWithError();
    }

    std::ostringstream output;
    char buffer[128];
    DWORD bytesRead;
    while(ReadFile(readPipeHandle.get(), buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead != 0) {
       buffer[bytesRead] = '\0';
       output << buffer;
    }

    return std::pair<decltype(returnValue), std::string>{ returnValue, output.str() };
}
