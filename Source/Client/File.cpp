#include <File.h>

#include <fstream>
#include <sstream>

#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #include <string.h>
#elif _WIN32
    #include <Windows.h>
#endif

namespace Engine {

namespace File
{
    std::string slurp(const char *filename)
    {
        std::ifstream in;
        in.open(filename, std::ifstream::in | std::ifstream::binary);
        std::stringstream sstr;
        sstr << in.rdbuf();
        in.close();
        return sstr.str();
    }

    char* slurpBytes(const char* filename){
        FILE *f = fopen(filename, "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *string = (char *)malloc(fsize);
        fread(string, fsize, 1, f);
        fclose(f);
        return string;
    }

    #if defined(__linux__) || defined(__APPLE__)
    std::filesystem::path ExecutablePath(){
        const int MAXSIZE=255;
        char path[MAXSIZE];
        memset(path, 0, MAXSIZE);

        readlink("/proc/self/exe",path,MAXSIZE);
        *strrchr(path, '/') = '\0';

        return std::filesystem::path(path);
    }

    #elif _WIN32
    std::filesystem::path ExecutablePath(){
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return std::filesystem::path(buffer);
    }
    #else
    #endif
}

}