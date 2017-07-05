#pragma once

#include <string>

class path
{
private:
  std::string data;
  
public:
  path(const char* data) : data(data) { }
  path(const std::string& data) : data(data) { }
  
  bool exists() const;
  size_t length() const;
  
  const char* c_str() const { return data.c_str(); }
  
  friend std::ostream& operator<<(std::ostream& os, const class path& path) { os << path.data; return os; }
};

enum class file_mode
{
  WRITING,
  APPENDING,
  READING
};

class file_handle
{
private:
  path path;
  mutable FILE* file;
  
public:
  static bool read(void* dest, size_t size, size_t count, const file_handle& handle) { return handle.read(dest, size, count); }
  static bool write(const void* src, size_t size, size_t count, const file_handle& handle) { return handle.write(src, size, count); }
  
  file_handle(const class path& path, file_mode mode, bool readOnly = false) : file(nullptr), path(path)
  {
    open(path, mode);
  }
  
  ~file_handle() { if (file) close(); }
  
  file_handle& operator=(file_handle& other) { this->file = other.file; this->path = other.path; other.file = nullptr; return *this; }
  file_handle(const file_handle& other) : file(other.file), path(other.path) { other.file = nullptr; }
  
  template <typename T> bool write(const T& src) const { return write(&src, sizeof(T), 1); }
  template <typename T> bool read(T& dst) const { return read(&dst, sizeof(T), 1); }
  
  bool write(const void* ptr, size_t size, size_t count) const {
    assert(file);
    size_t r = fwrite(ptr, size, count, file);
    bool success = r == count;
    return success;
  }
  
  bool read(void* ptr, size_t size, size_t count) const {
    assert(file);
    size_t r = fread(ptr, size, count, file);
    bool success = r == count;
    return success;
  }
  
  void seek(long offset, int origin) const {
    assert(file);
    fseek(file, offset, origin);
  }
  
  long tell() const {
    assert(file);
    return ftell(file);
  }
  
  void rewind() const { fseek(file, 0, SEEK_SET); }
  void flush() const { fflush(file); }
  
  bool open(const class path& path, file_mode mode)
  {
#ifdef _WIN32
    const wchar_t* smode = L"rb";
    if (mode == file_mode::WRITING) smode = L"wb+";
    else if (mode == file_mode::APPENDING) smode = L"rb+";
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::wstring wpath = conv.from_bytes(path.c_str());
    file = _wfopen(wpath.c_str(), smode);
#else
    const char* smode = "rb";
    if (mode == file_mode::WRITING) smode = "wb+";
    else if (mode == file_mode::APPENDING) smode = "rb+";
    
    file = fopen(path.c_str() , smode);
#endif
    
    //if (!file || ferror(file))
    //  printf("FILE* %s (mode: %s) error: (%d) %s\n", path.c_str(), smode, errno, strerror(errno));
    
    return file != nullptr;
  }
  
  bool close() const
  {
    if (file == nullptr)
      assert(false);
    else
    {
      fclose(file);
      file = nullptr;
    }
    
    return true;
  }
  
  size_t length() const
  {
    assert(file != nullptr);
    long c = ftell(file);
    fseek(file, 0, SEEK_END);
    long s = ftell(file);
    fseek(file, c, SEEK_SET);
    return s;
  }
  
  std::string toString()
  {
    size_t len = length();
    std::unique_ptr<char[]> data = std::unique_ptr<char[]>(new char[len+1]);
    read(data.get(), sizeof(char), len);
    data.get()[len] = '\0';
    close();
    return std::string(data.get());
  }
  
  operator bool() const { return file != nullptr; }
};


#pragma mark Platform
class Platform
{
public:
  virtual bool exists(const path& path) const = 0;
  virtual std::string getResourcePath() const = 0;
  
  static Platform* instance();
};


