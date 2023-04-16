    /*
     * @file Voice Module.ino
     * @brief
     * @n [Get the module here]
     * @n This example Set the voice module volume and playback
     * @n [Connection and Diagram]()
     *
     * @copyright  [DFRobot](https://www.dfrobot.com), 2016
     * @copyright GNU Lesser General Public License
     *
     * @author [carl](lei.wu@dfrobot.com)
     * @version  V1.0
     * @date  2017-11-3
     */
    #include <SoftwareSerial.h>

    SoftwareSerial Serial1(10, 11);

    unsigned char order[4] = {0xAA,0x06,0x00,0xB0};


    void setup() {
    //Serial.begin(115200);
     Serial1.begin(9600);
     volume(0x1E);//音量设置0x00-0x1E
    }

    void loop() {
     play(0x01);//指定播放:0x01-文件0001
    // Serial1.write(order,4);//顺序播放
     delay(5000);
    }

    void play(unsigned char Track)
    {
     unsigned char play[6] = {0xAA,0x07,0x02,0x00,Track,Track+0xB3};//0xB3=0xAA+0x07+0x02+0x00,即最后一位为校验和
       Serial1.write(play,6);
    }
    void volume( unsigned char vol)
    {
      unsigned char volume[5] = {0xAA,0x13,0x01,vol,vol+0xBE};//0xBE=0xAA+0x13+0x01,即最后一位为校验和
         Serial1.write(volume,5);
     }
