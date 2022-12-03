// Simple test of Unicode filename.
// Unicode is supported as UTF-8 encoded strings.
#include <M5Core2.h>

#include "SdFat.h"


#define SOFTSD_MOSI_PIN (GPIO_NUM_23)
#define SOFTSD_MISO_PIN (GPIO_NUM_19)
#define SOFTSD_SCK_PIN (GPIO_NUM_18)
#define SD_CS_PIN (GPIO_NUM_4)

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_HZ(20000000))

// USE_UTF8_LONG_NAMES must be non-zero in SdFat/src/SdFatCongfig.h
#if USE_UTF8_LONG_NAMES

#define UTF8_FOLDER u8"笑脸"
const char* names[] = {u8"россиянин", u8"très élégant", u8"狗.txt", nullptr};

// Remove files if non-zero.
#define REMOVE_UTF8_FILES 1

// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 0

#if SD_FAT_TYPE == 0
SdFat sd;
File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

void setup() {
  M5.begin();  //Init M5Core2.  初始化 M5Core2
  /* Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project */
  M5.Lcd.print("Hello World");  // Print text on the screen (string) 在屏幕上打印文本(字符串)
  // Serial.begin(115200);
  Serial.println("Type any character to begin");
  //  while (!Serial.available()) {
  //    // yield();
  //  }
  if (!sd.begin(SD_CONFIG)) {
    sd.initErrorHalt(&Serial);
  }
  if (!sd.exists(UTF8_FOLDER)) {
    if (!sd.mkdir(UTF8_FOLDER)) {
      Serial.println("sd.mkdir failed");
      return;
    }
  }
  if (!sd.chdir(UTF8_FOLDER)) {
    Serial.println("sd.chdir failed");
    return;
  }
  //  for (uint8_t i = 0; names[i]; i++) {
  //    if (!file.open(names[i], O_WRONLY | O_CREAT)) {
  //      Serial.println("file.open failed");
  //      return;
  //    }
  //    file.println(names[i]);
  //    file.close();
  //  }
  Serial.println("ls:");
  sd.ls("/", LS_SIZE | LS_R);
#if REMOVE_UTF8_FILES  // For debug test of remove and rmdir.
  for (uint8_t i = 0; names[i]; i++) {
    sd.remove(names[i]);
  }
  sd.chdir();
  sd.rmdir(UTF8_FOLDER);
  Serial.println("After remove and rmdir");
  sd.ls(LS_SIZE | LS_R);
#endif  // REMOVE_UTF8_FILES
  Serial.println("Done!");
}
void loop() {
}
#else  // USE_UTF8_LONG_NAMES
#error USE_UTF8_LONG_NAMES must be non-zero in SdFat/src/SdFatCongfig.h
#endif  // USE_UTF8_LONG_NAMES
