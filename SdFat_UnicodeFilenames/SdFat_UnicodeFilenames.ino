// Simple test of Unicode filename.
// Unicode is supported as UTF-8 encoded strings.
#include <M5Core2.h>

#include "SdFat.h"


#define SOFTSD_MOSI_PIN (GPIO_NUM_23)
#define SOFTSD_MISO_PIN (GPIO_NUM_38)
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
//File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
//File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
//ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
//FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

void setup() {
  // Serial.begin(115200);
  // Serial.println("Hello World");
  M5.begin();
  // M5.Axp.begin();
  //  pinMode(SD_CS_PIN, OUTPUT);
  //  digitalWrite(SD_CS_PIN, HIGH);
  // SPI.begin(SOFTSD_SCK_PIN, SOFTSD_MISO_PIN, SOFTSD_MOSI_PIN, -1);
  // M5.Lcd.begin();
  M5.Lcd.print("Hello World");
  //  SPI.setFrequency(20000000);
  // SPI.begin();
  // SPI.begin(SOFTSD_SCK_PIN, SOFTSD_MISO_PIN, SOFTSD_MOSI_PIN, -1);

  // SD.begin(SD_CS_PIN, SPI, 20000000);
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
  auto myFile = sd.open("flac/蔡淳佳 - 依恋.flac");
  size_t cnt = 0;
  if (myFile) {
    Serial.println("flac:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      auto c = myFile.read();
      Serial.write(c);
      cnt++;
      if (cnt > 256) {
        break;
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening");
  }
  Serial.println("Done!");
}
void loop() {
}
#else  // USE_UTF8_LONG_NAMES
#error USE_UTF8_LONG_NAMES must be non-zero in SdFat/src/SdFatCongfig.h
#endif  // USE_UTF8_LONG_NAMES
