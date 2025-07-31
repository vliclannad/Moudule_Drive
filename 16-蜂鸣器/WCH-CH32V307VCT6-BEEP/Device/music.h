#ifndef __MUSIC_H
#define __MUSIC_H

#include "beep.h"

// 音符时长定义 (毫秒)
#define NOTE_DURATION_WHOLE      2000   // 全音符
#define NOTE_DURATION_HALF       1000   // 二分音符  
#define NOTE_DURATION_QUARTER    500    // 四分音符
#define NOTE_DURATION_EIGHTH     250    // 八分音符
#define NOTE_DURATION_SIXTEENTH  125    // 十六分音符

// 常用音乐定义结构体
typedef struct {
    uint16_t note;      // 音符频率
    uint16_t duration;  // 持续时间(ms)
} MusicNote_t;

// 生日快乐歌
extern const MusicNote_t HappyBirthday[];
extern const uint8_t HappyBirthdayLength;

// 小星星
extern const MusicNote_t TwinkleStar[];  
extern const uint8_t TwinkleStarLength;

// 两只老虎
extern const MusicNote_t TwoTigers[];
extern const uint8_t TwoTigersLength;

// 音乐播放函数
void PlayMusic(const MusicNote_t* music, uint8_t length);
void PlayHappyBirthday(void);
void PlayTwinkleStar(void);
void PlayTwoTigers(void);

// 高级音乐播放函数
void PlayBadApple(void);        // 播放《Bad Apple!!》- 东方Project经典曲目

#endif
