#include "lbxlib.h"

#include "gfx/LBX.h"
#include "platform/Path.h"

#include "gfx/Font.h"
#include "gfx/Gfx.h"

#include <unordered_map>
#include <string>

#define DEBUG_LOG(x, ...) do { printf(x "\n", __VA_ARGS__); fflush(stdout); } while (false)

#define JAVA_CLASS_PALETTE "com/pixbits/lib/ui/color/Palette"
#define JAVA_CLASS_SPRITE_SHEET "com/github/jakz/openmom/lbx/SpriteSheet"

using namespace lbx;

//static char buffer[512];
void debugprintf(const char* str, ...)
{
  /*va_list args;
  va_start (args, str);
  vsnprintf (buffer, 512, str, args);
  printf("%s\n", buffer);*/
}

IndexedPalette* Gfx::mainPalette = new IndexedPalette(256);
IndexedPalette* Gfx::mainPaletteOpaque = new IndexedPalette(256);
FontData* FontData::fonts[FONT_TYPE_COUNT];

struct jni_string
{
  JNIEnv* const _env;
  const char* data;
  jstring ref;
  operator const char*() const { return data; }
  
  jni_string(JNIEnv* env, jstring str) : data(env->GetStringUTFChars(str, nullptr)), _env(env), ref(str) { }
  jni_string(const jni_string& other) = delete;
  jni_string& operator=(const jni_string& other) = delete;
  
  std::string toString() const { return std::string(data); }
  operator std::string() const { return std::string(data); }
  
  ~jni_string()
  {
    _env->ReleaseStringUTFChars(ref, data);
  }
};

struct JavaEnv
{
  jclass classPalette;
  jclass classSpritesheet;
  
  jmethodID methodPaletteInit;
  
  jmethodID methodSpriteSheetInit;
  jmethodID methodSpriteSheetSetData;
};

static bool isInit = false;
static std::unordered_map<std::string, FileInfo> headerCache;
static JavaEnv jenv;

void initEnvironment(JNIEnv* env)
{
  jenv.classPalette = (jclass)env->NewGlobalRef(env->FindClass(JAVA_CLASS_PALETTE));
  jenv.classSpritesheet = (jclass)env->NewGlobalRef(env->FindClass(JAVA_CLASS_SPRITE_SHEET));
  
  jenv.methodPaletteInit = env->GetMethodID(jenv.classPalette, "<init>", "([I)V");
  jenv.methodSpriteSheetInit = env->GetMethodID(jenv.classSpritesheet, "<init>", "(IIIL" JAVA_CLASS_PALETTE ";)V");
  jenv.methodSpriteSheetSetData = env->GetMethodID(jenv.classSpritesheet, "data", "(I[B)V");

  
  bool isOk = jenv.classPalette && jenv.classSpritesheet && jenv.methodPaletteInit && jenv.methodSpriteSheetInit && jenv.methodSpriteSheetSetData;
  
  DEBUG_LOG("JNI initialization: %s", isOk ? "success" : "failed");
}

jobject paletteToJava(JNIEnv* env, const Palette* palette, size_t size)
{
  jboolean isCopy;
  
  jintArray intArray = env->NewIntArray((jint)size);
  int* idata = reinterpret_cast<int*>(env->GetPrimitiveArrayCritical(intArray, &isCopy));
  
  for (size_t i = 0; i < size; ++i)
  {
    Color c = palette->get(i);
    idata[i] = (c.a << 24) | (c.r << 16) | (c.g << 8) | c.b;
  }
  
  env->ReleasePrimitiveArrayCritical(intArray, idata, 0);
  
  return env->NewObject(jenv.classPalette, jenv.methodPaletteInit, intArray);
}

extern "C"
{
  /* 
   LBXFile& lbx = file(LBXID::FONTS);
   file_handle in = getDescriptor(lbx);
   
   LBX::loadHeader(lbx.info, in);
   LBX::loadFonts(lbx.info.header, lbx.info.offsets, in.fd());
   LBX::loadPalettes(lbx.info.header, lbx.info.offsets, in.fd());
   */
  
  JNIEXPORT void JNICALL Java_com_github_jakz_openmom_lbx_LBX_init(JNIEnv* env, jobject obj)
  {
    if (!isInit)
    {
      initEnvironment(env);
      
      Path base = "../../data/lbx/fonts.lbx";
      file_handle in(base, file_mode::READING);
      
      FileInfo info;
      LBX::loadHeader(info, in);
      LBX::loadFonts(info.header, info.offsets, in.fd());
      LBX::loadPalettes(info.header, info.offsets, in.fd());
      isInit = true;
    }
  }
  
  
  JNIEXPORT jobject JNICALL Java_com_github_jakz_openmom_lbx_LBX_internalLoadSprite(JNIEnv* env, jobject obj, jstring jpath, jint index)
  {
    if (!isInit)
      Java_com_github_jakz_openmom_lbx_LBX_init(env, obj);
    
    Path path = Path(jni_string(env, jpath).toString());
    
    DEBUG_LOG("Loading sprite %d from %s", index, path.c_str());
    DEBUG_LOG("File exists: %s", path.exists() ? "yes" : "no");
    
    file_handle handle = file_handle(path, file_mode::READING);
    
    auto it = headerCache.find(path.c_str());
    
    if (it == headerCache.end())
    {
      FileInfo info;
      LBX::loadHeader(info, handle);
      headerCache[path.c_str()] = info;
    }
    
    FileInfo info = headerCache[path.c_str()];
    
    const LBXSpriteData* sprite = LBX::scanGfx(info.header, info.offsets[index], handle.fd());
    DEBUG_LOG("Loaded sprite (size: %dx%d, count: %d)", sprite->width, sprite->height, sprite->count);

    jobject jpalette = paletteToJava(env, sprite->palette, 256);
    jobject jsprite = env->NewObject(jenv.classSpritesheet, jenv.methodSpriteSheetInit, (jint)sprite->count, (jint)sprite->width, (jint)sprite->height, jpalette);
    
    for (jint i = 0; i < sprite->count; ++i)
    {
      jboolean isCopy;
      jbyteArray byteArray = env->NewByteArray(sprite->width*sprite->height);
      byte* idata = reinterpret_cast<byte*>(env->GetPrimitiveArrayCritical(byteArray, &isCopy));
      
      std::copy(sprite->data[i], sprite->data[i]+(sprite->width*sprite->height), idata);
  
      env->ReleasePrimitiveArrayCritical(byteArray, idata, 0);
      env->CallVoidMethod(jsprite, jenv.methodSpriteSheetSetData, i, byteArray);
    }
    
    return jsprite;
  }

  JNIEXPORT void JNICALL Java_com_github_jakz_openmom_lbx_LBX_clearCache(JNIEnv* env, jobject obj)
  {
    headerCache.clear();
  }
}


#ifdef _WIN32

#include "platform/Path.h"
#include "platform/platform.h"

class PlatformWin : public Platform
{
public:
  Path getResourcePath() const override
  {
    return "C:\\Users\\Jack\\Documents\\dev\\openmom";
  }

  bool exists(const Path& path) const override
  {
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
  }

  Path absolute(const Path& path) const override
  {
    // TODO
    return path;
  }
};

Platform* Platform::instance()
{
  static PlatformWin platform;
  return &platform;
}

#endif