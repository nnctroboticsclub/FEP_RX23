/**
 *  @file   FEP_RX23.h
 *  @brief  FEP高速受信用ライブラリ
 *  @author 梅沢晄平
 *  @date   23/10/21
 */
#ifndef FEP_RX23_H
#define FEP_RX23_H

#define TIMEOUT_COUNT 5

#include "mbed.h"

/**
 *  @class con
 *  @brief class for FEP
 *  @note  mbed os-6 で使用可能なFEP受信用ライブラリです。FEP_RX22_os6ライブラリに対して処理の効率化・通信の堅牢度向上を行いました。
 */
class FEP_RX23 : public mbed::UnbufferedSerial {
public :
    /** constructor
     *  @param tx      FEPと接続するSerialTX pin
     *  @param rx      FEPと接続するSerialRX pin
     *  @param addr    送信側のFEPのアドレス
     *  @param datalen 送信側のデータ数
     *  @param baud    通信速度(デフォルト115200)
     *  @note  今までのライブラリと違い送信側のアドレスやデータ数が異なると受信できません
     */
    FEP_RX23(PinName tx, PinName rx, uint8_t addr_, uint8_t datalen_, int baud=115200);

    /** Start receiving
     */
    void StartReceive();

    /** Check timeout
     *  @brief 0.1秒毎のループで受信のタイムアウトをチェック
     */
    void TimeoutLoop();

    /** Interrupt input
     */
    void ReceiveBytes();

    /** extract the message
     */
    void CheckData();

    /** Write the received message
     *  @param data 受信メッセージを格納する配列
     */
    bool getData(uint8_t *data);

    /** return recieved or not
     */
    bool getStatus();

private :
    Ticker timeoutTimer;
    uint8_t buffer[256];             //! 全受信データを格納するリングバッファ配列
    uint8_t* msgdata;                //! メッセージのみを格納する配列
    uint8_t bufindex;                //! buffer の添え字変数
    uint8_t timeout;                 //! 0.1秒間通信が成功していないと1増える
    uint8_t indexofR;                //! RBNの先頭文字の位置
    bool status;                     //! (TIMEOUT_COUNT * 0.1)秒間通信が成り立っていない場合、false
    uint8_t datalen;                 //! データバイトの長さ
    uint8_t header[9]={'R','B','N'}; //! 受信コマンド
};

#endif
