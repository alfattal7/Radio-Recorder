/*============================================================================
 Name        : stack.c
 Author      : MHD Bashar Al Fattal
 Version     : v1
 Copyright   : Your copyright notice
 Description : Radio Player and recorder
 Datum       : 29.08.2022
 ============================================================================*/

#ifndef RECORDER_H
#define RECORDER_H

#include <QMainWindow>
#include<QDesktopServices>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonDocument>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
#include<QDateTime>
#include<QAudioRecorder>
#include<QAudioInput>
#include<QAudio>
#include<QMultimedia>
#include<QAudioEncoderSettings>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QJsonArray>
#include<QJsonValue>


QT_BEGIN_NAMESPACE
namespace Ui { class Recorder; }
QT_END_NAMESPACE


struct RadioEntry {
    QString name;
    QUrl url;
    QString desrc;
};

class Recorder : public QMainWindow
{
    Q_OBJECT

public:
    Recorder(QWidget *parent = nullptr);
    ~Recorder();

// Memberklasse

private:
    Ui::Recorder *ui;
    QMediaPlayer* player = new QMediaPlayer(this);
    QAudioInput *audio = new QAudioInput();
    QAudioRecorder* audioRecorder = new QAudioRecorder(this);
    RadioEntry currentRadio;

//Die Methoden ,die ich brauche

private slots:
    void RecorderButtonPressed();
    void AddRadioButtonPressed();
    void RemoveRadioButtonPressed();
    void PlayButtonPressed();
    void ComboBoxFill();
    void ComboBoxIndexChanged(int index);
    void StopButtonPressed();
    void setVolume(int volume);
    void StopRecorderButtonPressed();
    void FirstCallSetting();
    void clearall();

};
#endif // RECORDER_H
