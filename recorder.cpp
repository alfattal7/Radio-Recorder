/*============================================================================
 Name        : stack.c
 Author      : MHD Bashar Al Fattal
 Version     : v1
 Copyright   : Your copyright notice
 Description : Radio Player and recorder
 Datum       : 29.08.2022
 ============================================================================*/

#include "recorder.h"
#include "ui_recorder.h"

// Konstruktor
Recorder :: Recorder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui :: Recorder)
{
    ui->setupUi(this);
// Der Aufruf die Methoder...

    FirstCallSetting();

    ComboBoxFill();

    clearall();
// Die Verknüpfung der Buttons mit den Methoden..
    connect(ui->Recorders, SIGNAL(clicked()), this ,
            SLOT(RecorderButtonPressed()));
    connect(ui->AddRadioButton, SIGNAL(clicked()), this ,
            SLOT(AddRadioButtonPressed()));
    connect(ui->RemoveRadioButton, SIGNAL(clicked()), this ,
            SLOT(RemoveRadioButtonPressed()));
    connect(ui->PlayButton, SIGNAL(clicked()), this ,
            SLOT(PlayButtonPressed()));
    connect(ui->comboBoxTest, SIGNAL(currentIndexChanged(int)), this ,
            SLOT(ComboBoxIndexChanged(int)));
    connect(ui->StopButton, SIGNAL(clicked()), this ,
            SLOT(StopButtonPressed()));
    connect(ui->VolumeSlider, SIGNAL(valueChanged(int)), this,
            SIGNAL(volumeChanged(int)));
    connect(ui->VolumeSlider, SIGNAL(valueChanged(int)), player,
            SLOT(setVolume(int)));
    connect(ui->StopRecorderButton, SIGNAL(clicked()), this,
            SLOT(StopRecorderButtonPressed()));
}
// Default Konstruktor
Recorder :: ~Recorder()
{
    delete ui;
}

//==========================================================================================
//Recorder Methode, Recordet und speichert die Datei als mp3 ..
void Recorder :: RecorderButtonPressed()
{

    QAudioEncoderSettings settings;

    audioRecorder->setOutputLocation(QUrl(("//ibs-server/ibs-server/ibs/Mitarbeiter/Al Fattal Bashar/Qt-Projekte/RadioRecorder/audio/PCM")));
    settings.setSampleRate(8000);
    settings.setChannelCount(1);
    settings.setCodec("audio/PCM");
    audioRecorder->setEncodingSettings(settings);
    audioRecorder->setAudioInput(audioRecorder->defaultAudioInput());
    settings.setQuality(QMultimedia :: HighQuality);
    audioRecorder->setContainerFormat("mp3");

    qDebug()<<"audio recorder with settings";

    audioRecorder->record();
    ui->Recorders->setHidden(true);
    ui->StopRecorderButton->setHidden(false);

}

//===========================================================================================
//stoppt den Recorder ..
void Recorder :: StopRecorderButtonPressed()
{
    qDebug()<< " you stopped the Recorder !!";
    audioRecorder->stop();
    ui->Recorders->setHidden(false);
    ui->StopRecorderButton->setHidden(true);

}
//==========================================================================================
//um den konstruktor schön zu machen , packt man alle FirstCall Setting in einer Methode und ruft sie im konstruktor eifach auf..
void Recorder::FirstCallSetting()
{
    ui->VolumeSlider->setEnabled(false);
    ui->Recorders->setEnabled(false);
    ui->StopButton->setHidden(true);
    ui->StopRecorderButton->setHidden(true);
    ui->VolumeSlider->setValue(50);
    ui->RemoveRadioButton->setEnabled(true);
    ui->comboBoxTest->setCurrentIndex(0);
}
// macht die ComboBox leer ..
void Recorder::clearall()
{
    if (ui->comboBoxTest->count() > 0)
        ui->RadioName->clear();
}

//===========================================================================================
/*Add Radio nimmt die eingegebne Daten zu einem Json Datei,wo die nochmal eingelesen würden..
 *  alle Radio werden in einem Array gespeichert ,der Name "Radio" ist*/

void Recorder :: AddRadioButtonPressed()
{

    if (ui->comboBoxTest->count() > 0){
        ui->AddRadioButton->setEnabled(true);
        QString RadioName = ui->RadioName->text();
        QString URL = ui ->URL->text();
        QString Description = ui->Description->text();

        QFile file("//ibs-server/ibs-server/ibs/Mitarbeiter/Al Fattal Bashar/Qt-Projekte/RadioRecorder/DeutschlandRadios.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QJsonParseError JsonParseError;
        QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
        file.close();

        QJsonObject RootObject = JsonDocument.object();
        QJsonArray Jarray = RootObject.value("radio").toArray();

        QJsonObject ElementOneObject = Jarray.at(0).toObject();
        if(!RadioName.isEmpty() && !URL.isEmpty()){
            ElementOneObject.insert("RadioName", QJsonValue(RadioName));
            ElementOneObject.insert("URL", QJsonValue(URL));
            ElementOneObject.insert("Description", QJsonValue(Description));
            Jarray.insert(Jarray.count(),ElementOneObject);
            RootObject.insert("radio", Jarray);
            JsonDocument.setObject(RootObject);


            file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            file.write(JsonDocument.toJson());
            file.close();
            QMessageBox :: information(this, tr("Message"), tr(" you added a Radio to the list ! "));
        }
        else {
            qDebug()<< " there is a problem with the insert's information";
            QMessageBox :: warning(this,tr("Error"),tr("The Radio's name or URL box is empty !! "));
        }

        ui->comboBoxTest->clear();
        ui->RadioName->clear();
        ui->Description->clear();
        ui->URL->clear();

        ComboBoxFill();
    }
    else if (ui->RadioName->text().isEmpty()&&ui->URL->text().isEmpty()){
        ui->AddRadioButton->setHidden(true);
        ui->PlayButton->setHidden(true);
    }
}

//====================================================================================================================
//wo die Zeiger des ComboBox zeigt, wird der Radio gelöscht..
void Recorder :: RemoveRadioButtonPressed()
{

    QFile file("//ibs-server/ibs-server/ibs/Mitarbeiter/Al Fattal Bashar/Qt-Projekte/RadioRecorder/DeutschlandRadios.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();

    QJsonObject RootObject = JsonDocument.object();
    QJsonArray Jarray = RootObject.value("radio").toArray();

    Jarray.removeAt(ui->comboBoxTest->currentIndex());
    RootObject.insert("radio", Jarray);
    JsonDocument.setObject(RootObject);


    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(JsonDocument.toJson());
    file.close();

    ui->comboBoxTest->removeItem(ui->comboBoxTest->currentIndex());
    qDebug()<< " you removed the Radio , what is choosen !! ";
    QMessageBox :: information(this, tr("Message"), tr(" you removed a Radio form the list ! "));

    ui->comboBoxTest->clear();
    ui->RadioName->clear();
    ui->Description->clear();
    ui->URL->clear();

}

//==========================================================================================================
//spielt den ausgewählten Radio ..
void Recorder :: PlayButtonPressed()
{

    player->setMedia(QUrl(currentRadio.url));
    player->play();

    ui->VolumeSlider->setEnabled(true);
    ui->Recorders->setEnabled(true);
    ui->StopButton->setHidden(false);
    ui->PlayButton->setHidden(true);
    ui->RemoveRadioButton->setEnabled(false);


    qDebug()<< "Radio playing !! ";

}

//==========================================================================================================
//Die Ausfüllung des ComboBox mit daten vom Array Radio ,der in Json Datei geschrieben ist ..
void Recorder :: ComboBoxFill()
{
    QFile file("//ibs-server/ibs-server/ibs/Mitarbeiter/Al Fattal Bashar/Qt-Projekte/RadioRecorder/DeutschlandRadios.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument Jdocument = QJsonDocument :: fromJson(jsonData);
    QJsonObject Jobject = Jdocument .object();


    QJsonValue Jvalue = Jobject.value("radio");
    QJsonArray Jarray = Jvalue.toArray();

    foreach (const QJsonValue &v, Jarray){

        QString RadioName = v.toObject().value("RadioName").toString();
        QString URL = v.toObject().value("URL").toString();


        ui->comboBoxTest->addItem(RadioName, QVariant(URL));


    }

}

//==========================================================================================================

void Recorder::ComboBoxIndexChanged(int)
{
    currentRadio.name = ui->comboBoxTest->currentText();
    ui->RadioName->setText(currentRadio.name);

    currentRadio.url = QUrl(ui->comboBoxTest->currentData().toString());
    ui->URL->setText(currentRadio.url.toString());

    // currentRadio.desrc =
    //ui->Description->setText(currentRadio.desrc);

    ui->StopButton->setHidden(true);
    ui->PlayButton->setHidden(! currentRadio.url.isValid());


    qDebug() <<"you changed the Radio index from the List !!";
}

//==========================================================================================================

void Recorder::StopButtonPressed()
{
    player->stop();

    ui->VolumeSlider->setEnabled(false);
    ui->PlayButton->setHidden(false);
    ui->StopButton->setHidden(true);
    ui->Recorders->setEnabled(false);
    ui->RemoveRadioButton->setEnabled(true);

    qDebug()<< "you Stoped the Player !! ";
}

//==========================================================================================================
//Lautstärker änderen
void Recorder::setVolume(int volume)
{

    player->setVolume(volume);
    qDebug()<< "you change the volume of the sound !! ";
}

//==========================================================================================================
