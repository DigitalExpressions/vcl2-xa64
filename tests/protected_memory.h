/*
 * Copyright 2026 DigitalExpressions Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VCL_TEST_PROTECTED_MEMORY_H
#define VCL_TEST_PROTECTED_MEMORY_H
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#define VCL_TEST_UNDEF_NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define VCL_TEST_UNDEF_LEAN
#endif
#include <windows.h>
#ifdef VCL_TEST_UNDEF_NOMINMAX
#undef NOMINMAX
#undef VCL_TEST_UNDEF_NOMINMAX
#endif
#ifdef VCL_TEST_UNDEF_LEAN
#undef WIN32_LEAN_AND_MEAN
#undef VCL_TEST_UNDEF_LEAN
#endif
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace vcl_test {
[[noreturn]] inline void memory_failure(const char* operation) {
#if defined(_WIN32)
    std::fprintf(stderr, "%s failed (Windows error %lu)\n", operation,
                 static_cast<unsigned long>(GetLastError()));
#else
    std::perror(operation);
#endif
    std::abort();
}

inline size_t page_size() {
#if defined(_WIN32)
    SYSTEM_INFO info{};
    GetSystemInfo(&info);
    // Protection boundaries use dwPageSize, not dwAllocationGranularity.
    if (!info.dwPageSize) memory_failure("GetSystemInfo page size");
    return size_t(info.dwPageSize);
#else
    const long page = sysconf(_SC_PAGESIZE);
    if (page <= 0) memory_failure("sysconf page size");
    return size_t(page);
#endif
}

// Start inaccessible; callers explicitly open only their original data pages.
// PAGE_NOACCESS, unlike PAGE_GUARD, remains inaccessible after a first fault.
class ProtectedMemory {
    void* memory_;
    size_t bytes_;
public:
    explicit ProtectedMemory(size_t bytes) : memory_(nullptr), bytes_(bytes) {
#if defined(_WIN32)
        memory_ = VirtualAlloc(nullptr, bytes_, MEM_RESERVE | MEM_COMMIT, PAGE_NOACCESS);
        if (!memory_) memory_failure("VirtualAlloc");
#else
        memory_ = mmap(nullptr, bytes_, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (memory_ == MAP_FAILED) memory_failure("mmap");
#endif
    }
    ProtectedMemory(const ProtectedMemory&) = delete;
    ProtectedMemory& operator=(const ProtectedMemory&) = delete;
    unsigned char* data() { return static_cast<unsigned char*>(memory_); }
    void writable(size_t offset, size_t bytes) {
        if (offset > bytes_ || bytes > bytes_ - offset) std::abort();
#if defined(_WIN32)
        DWORD previous;
        if (!VirtualProtect(data() + offset, bytes, PAGE_READWRITE, &previous))
            memory_failure("VirtualProtect");
#else
        if (mprotect(data() + offset, bytes, PROT_READ | PROT_WRITE) != 0)
            memory_failure("mprotect");
#endif
    }
    bool release() {
        if (!memory_) return true;
#if defined(_WIN32)
        if (!VirtualFree(memory_, 0, MEM_RELEASE)) return false;
#else
        if (munmap(memory_, bytes_) != 0) return false;
#endif
        memory_ = nullptr;
        return true;
    }
    ~ProtectedMemory() { if (!release()) memory_failure("protected memory release"); }
};
}
#endif
