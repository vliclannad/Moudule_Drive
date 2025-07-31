#include "music.h"

// 生日快乐歌
const MusicNote_t HappyBirthday[] = {
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_C, NOTE_DURATION_EIGHTH},     // Do
    {NOTE_M1_D, NOTE_DURATION_QUARTER},    // Re  
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_E, NOTE_DURATION_HALF},       // Mi
    
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_C, NOTE_DURATION_EIGHTH},     // Do
    {NOTE_M1_D, NOTE_DURATION_QUARTER},    // Re
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_F, NOTE_DURATION_HALF},       // Fa
    
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_C, NOTE_DURATION_EIGHTH},     // Do
    {NOTE_H1_C, NOTE_DURATION_QUARTER},    // 高音Do
    {NOTE_M1_A, NOTE_DURATION_QUARTER},    // La
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_D, NOTE_DURATION_HALF},       // Re
    
    {NOTE_M1_B, NOTE_DURATION_QUARTER},    // Si  
    {NOTE_M1_B, NOTE_DURATION_EIGHTH},     // Si
    {NOTE_M1_A, NOTE_DURATION_QUARTER},    // La
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_F, NOTE_DURATION_HALF}        // Fa
};
const uint8_t HappyBirthdayLength = sizeof(HappyBirthday) / sizeof(HappyBirthday[0]);

// 小星星
const MusicNote_t TwinkleStar[] = {
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do  
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_A, NOTE_DURATION_QUARTER},    // La
    {NOTE_M1_A, NOTE_DURATION_QUARTER},    // La
    {NOTE_M1_G, NOTE_DURATION_HALF},       // Sol
    
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_D, NOTE_DURATION_QUARTER},    // Re
    {NOTE_M1_D, NOTE_DURATION_QUARTER},    // Re
    {NOTE_M1_C, NOTE_DURATION_HALF},       // Do
    
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_D, NOTE_DURATION_HALF},       // Re
    
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_G, NOTE_DURATION_QUARTER},    // Sol
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_D, NOTE_DURATION_HALF}        // Re
};
const uint8_t TwinkleStarLength = sizeof(TwinkleStar) / sizeof(TwinkleStar[0]);

// 两只老虎
const MusicNote_t TwoTigers[] = {
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_M1_D, NOTE_DURATION_QUARTER},    // Re
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do  
    {NOTE_M1_D, NOTE_DURATION_QUARTER},    // Re
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_G, NOTE_DURATION_HALF},       // Sol
    
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_F, NOTE_DURATION_QUARTER},    // Fa
    {NOTE_M1_G, NOTE_DURATION_HALF},       // Sol
    
    {NOTE_M1_G, NOTE_DURATION_EIGHTH},     // Sol
    {NOTE_M1_A, NOTE_DURATION_EIGHTH},     // La
    {NOTE_M1_G, NOTE_DURATION_EIGHTH},     // Sol
    {NOTE_M1_F, NOTE_DURATION_EIGHTH},     // Fa
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    
    {NOTE_M1_G, NOTE_DURATION_EIGHTH},     // Sol
    {NOTE_M1_A, NOTE_DURATION_EIGHTH},     // La
    {NOTE_M1_G, NOTE_DURATION_EIGHTH},     // Sol
    {NOTE_M1_F, NOTE_DURATION_EIGHTH},     // Fa
    {NOTE_M1_E, NOTE_DURATION_QUARTER},    // Mi
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_L1_G, NOTE_DURATION_QUARTER},    // 低音Sol
    {NOTE_M1_C, NOTE_DURATION_HALF},       // Do
    
    {NOTE_M1_C, NOTE_DURATION_QUARTER},    // Do
    {NOTE_L1_G, NOTE_DURATION_QUARTER},    // 低音Sol
    {NOTE_M1_C, NOTE_DURATION_HALF}        // Do
};
const uint8_t TwoTigersLength = sizeof(TwoTigers) / sizeof(TwoTigers[0]);

// 通用音乐播放函数
void PlayMusic(const MusicNote_t* music, uint8_t length)
{
    for(uint8_t i = 0; i < length; i++) {
        if(music[i].note == NOTE_SILENT) {
            // 静音处理
            BEEP_Stop();
            Delay_Ms(music[i].duration);
        } else {
            BEEP_PlayNote(music[i].note, music[i].duration);
        }
        Delay_Ms(50); // 音符间隔
    }
    BEEP_Stop(); // 播放完毕后停止
}

// 播放生日快乐歌
void PlayHappyBirthday(void)
{
    PlayMusic(HappyBirthday, HappyBirthdayLength);
}

// 播放小星星
void PlayTwinkleStar(void)
{
    PlayMusic(TwinkleStar, TwinkleStarLength);
}

// 播放两只老虎
void PlayTwoTigers(void)
{
    PlayMusic(TwoTigers, TwoTigersLength);
}
