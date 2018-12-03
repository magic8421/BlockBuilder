#include "stdafx.h"
#include "Logger.h"
#include <time.h>
#include <strsafe.h>


static bool log_to_ods = true;
static bool log_to_file = true;
static HANDLE log_file_handle = 0;

static const wchar_t *LOG_DIR = L"C:\\ZLOG";
static const int MAX_LOG_NUM = 15;

static void CreateLogFile()
{
    if (log_file_handle) {
        return;
    }
    if (!::PathFileExists(LOG_DIR)) {
        ::CreateDirectory(LOG_DIR, NULL);
    }
    wchar_t buffer[64];
    tm tm_;
    time_t time_ = time(NULL);
    localtime_s(&tm_, &time_);
    wcsftime(buffer, ARRAYSIZE(buffer), L"%Y%m%d-%H%M%S", &tm_);
    std::wstring path = LOG_DIR;
    path += L"\\";
    path += buffer;
    path += L".log";
    log_file_handle = ::CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == log_file_handle) {
        assert(false);
    }
}

static bool Compare(const WIN32_FIND_DATA &lhs, const WIN32_FIND_DATA &rhs)
{
    return ::CompareFileTime(&lhs.ftLastWriteTime, &rhs.ftLastWriteTime) < 0;
}

static void DeleteOldLog()
{
    std::vector<WIN32_FIND_DATA> vecFiles;
    WIN32_FIND_DATA wfd;
    std::wstring path = LOG_DIR;
    path += L"\\*.log";
    HANDLE hFind = ::FindFirstFile(path.c_str(), &wfd);
    if (hFind != INVALID_HANDLE_VALUE) {
        vecFiles.push_back(wfd);
        while (::FindNextFile(hFind, &wfd))
        {
            vecFiles.push_back(wfd);
        }
        ::FindClose(hFind);
    }
    if (vecFiles.size() > MAX_LOG_NUM) {
        std::sort(vecFiles.begin(), vecFiles.end(), Compare);
        for (size_t i = 0; i < vecFiles.size() && i <= vecFiles.size() - MAX_LOG_NUM; ++i) {
            std::wstring file = LOG_DIR;
            file += L"\\";
            file += vecFiles.at(i).cFileName;
            ::DeleteFile(file.c_str());
        }
    }
}

void zLogInit()
{
    if (::PathFileExists(LOG_DIR)) {
        DeleteOldLog();
        CreateLogFile();
    }
}

void zLogImpl(const char *source, int line, const char *format, ...)
{
    const size_t SIZE = 1024 * 2;
    char buffer[SIZE];
    va_list varg;
    va_start(varg, format);
    ::StringCbVPrintfA(buffer, SIZE, format, varg);
    va_end(varg);
    char buffer2[SIZE];
    ::StringCbPrintfA(buffer2, SIZE, "%s\t%s(%d)\r\n", buffer, source, line);

    if (log_to_ods) {
        ::OutputDebugStringA(buffer2);
    }
    if (log_to_file) {
        size_t len = strlen(buffer2);
        DWORD out_len = 0;
        ::WriteFile(log_file_handle, buffer2, (DWORD)len, &out_len, NULL);
    }
}

