#include <QProcess>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QTemporaryFile>
#include <QCloseEvent>
#include <pcbsd-netif.h>
#include <pcbsd-utils.h>

#include "backend.h"
#include "ui_firstboot.h"
#include "firstboot.h"
#include "helpText.h"

Installer::Installer(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    translator = new QTranslator();

    connect(backButton, SIGNAL(clicked()), this, SLOT(slotBack()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(slotNext()));

    connect(helpButton, SIGNAL(clicked()), this, SLOT(slotHelp()));
    connect(pushTouchKeyboard, SIGNAL(clicked()), this, SLOT(slotPushVirtKeyboard()));
    connect(pushChangeKeyLayout, SIGNAL(clicked()), this, SLOT(slotPushKeyLayout()));

    connect(lineRootPW, SIGNAL(textChanged ( const QString &)), this, SLOT(slotCheckRootPW()));
    connect(lineRootPW2, SIGNAL(textChanged ( const QString &)), this, SLOT(slotCheckRootPW()));

    connect(lineName,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(lineName,SIGNAL(editingFinished()),this,SLOT(slotSuggestUsername()));
    connect(lineUsername,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(linePW,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));
    connect(linePW2,SIGNAL(textChanged(const QString)),this,SLOT(slotCheckUser()));

    backButton->setText(tr("&Back"));
    nextButton->setText(tr("&Next"));


    // Load the keyboard info
    keyModels = Scripts::Backend::keyModels();
    keyLayouts = Scripts::Backend::keyLayouts();

    // Load the timezones
    comboBoxTimezone->clear();
    comboBoxTimezone->addItems(Scripts::Backend::timezones());
    // Set America/New_York to default
    int index = comboBoxTimezone->findText("America/New_York", Qt::MatchStartsWith);
    if (index != -1)
       comboBoxTimezone->setCurrentIndex(index);

    // Start on the first screen
    installStackWidget->setCurrentIndex(0);
    backButton->setVisible(false);
}

Installer::~Installer()
{
    //delete ui;
}

void Installer::slotSuggestUsername()
{
  if ( ! lineUsername->text().isEmpty() || lineName->text().isEmpty() )
    return;
  QString name;
  if ( lineName->text().indexOf(" ") != -1 ) {
    name = lineName->text().section(' ', 0, 0).toLower();
    name.truncate(1);
  }
  name = name + lineName->text().section(' ', -1, -1).toLower();
  lineUsername->setText(name);
}

void Installer::slotPushKeyLayout()
{
  wKey = new widgetKeyboard();
  wKey->programInit(keyModels, keyLayouts);
  wKey->setWindowModality(Qt::ApplicationModal);
  connect(wKey, SIGNAL(changedLayout(QString, QString, QString)), this, SLOT(slotKeyLayoutUpdated(QString, QString, QString)));
  wKey->show();
  wKey->raise();
}

// Start xvkbd
void Installer::slotPushVirtKeyboard()
{
   system("killall -9 xvkbd; xvkbd -compact &");
}

void Installer::initInstall()
{
    // load languages
    comboLanguage->clear();
    languages = Scripts::Backend::languages();
    for (int i=0; i < languages.count(); ++i) {
        QString languageStr = languages.at(i);
        QString language = languageStr.split("-").at(0);
        comboLanguage->addItem(language.trimmed());
    }
    connect(comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeLanguage()));
}

void Installer::proceed(bool forward)
{
    int count = installStackWidget->count() - 1;
    int index = installStackWidget->currentIndex();

    index = forward ?
            (index == count ? count : index + 1) :
            (index == 0 ? 0 : index - 1);

    installStackWidget->setCurrentIndex(index);
}

// Check root pw
void Installer::slotCheckRootPW()
{
  nextButton->setEnabled(false);

  if ( lineRootPW->text().isEmpty() )
     return;
  if ( lineRootPW2->text().isEmpty() )
     return;
  if ( lineRootPW->text() != lineRootPW2->text() )
     return;
  // if we get this far, all the fields are filled in
  nextButton->setEnabled(true);
}

void Installer::slotCheckUser()
{
  nextButton->setEnabled(false);
  if ( lineName->text().isEmpty() )
     return;
  if ( lineUsername->text().isEmpty() )
     return;
  if ( linePW->text().isEmpty() )
     return;
  if ( linePW2->text().isEmpty() )
     return;
  if ( linePW->text() != linePW2->text() )
     return;
  nextButton->setEnabled(true);
}

// Slot which is called when the Finish button is clicked
void Installer::slotFinished()
{
  qApp->quit();
}

void Installer::slotNext()
{
   QString tmp;
   backButton->setVisible(true);

   // Check rootPW match
   if ( installStackWidget->currentIndex() == 1)
     slotCheckRootPW();
   // Check user info
   if ( installStackWidget->currentIndex() == 2)
     slotCheckUser();

   // Check if we have a wireless device
   if ( installStackWidget->currentIndex() == 3) {
     if ( system("ifconfig wlan0") == 0 ) {
       haveWifi = true;
       QTimer::singleShot(50,this,SLOT(slotScanNetwork()));
       connect(pushButtonRescan, SIGNAL(clicked()), this, SLOT(slotScanNetwork()));
       connect(listWidgetWifi, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(slotAddNewWifi()));
     } else {
       haveWifi = false;
     }
   }

   // If not doing a wireless connection
   if ( installStackWidget->currentIndex() == 3 && ! haveWifi) {
      installStackWidget->setCurrentIndex(5);
      // Save the settings
      saveSettings();
      nextButton->setText(tr("&Finish"));
      backButton->setVisible(false);
      nextButton->disconnect();
      connect(nextButton, SIGNAL(clicked()), this, SLOT(slotFinished()));
      return;
   }

   // Finished screen
   if ( installStackWidget->currentIndex() == 4 ) {
      // Save the settings
      saveSettings();
      nextButton->setText(tr("&Finish"));
      backButton->setVisible(false);
      nextButton->disconnect();
      connect(nextButton, SIGNAL(clicked()), this, SLOT(slotFinished()));
   }

   proceed(true);
}

void Installer::slotBack()
{
   nextButton->setEnabled(true);

   if ( installStackWidget->currentIndex() == 1 )
     backButton->setVisible(false);
   else
     backButton->setVisible(true);

   proceed(false);
}

void Installer::slotChangeLanguage()
{
    if ( comboLanguage->currentIndex() == -1 )
      return;

    // Figure out the language code
    QString langCode = languages.at(comboLanguage->currentIndex());
    
    // Grab the language code
    langCode.truncate(langCode.lastIndexOf(")"));
    langCode.remove(0, langCode.lastIndexOf("(") + 1); 

    // Check what directory our app is in
    QString appDir;
    if ( QFile::exists("/usr/local/bin/pc-sysinstaller") )
      appDir = "/usr/local/share/pcbsd";
    else
      appDir = QCoreApplication::applicationDirPath();
    
    //QTranslator *translator = new QTranslator();
    qDebug() << "Remove the translator";
    if ( ! translator->isEmpty() )
      QCoreApplication::removeTranslator(translator);
    
    if (translator->load( QString("FirstBoot_") + langCode, appDir + "/i18n/" )) {
      qDebug() << "Load new Translator" << langCode;
      QCoreApplication::installTranslator(translator);
      this->retranslateUi(this);
    }

}

void Installer::changeLang(QString code)
{
   // Change the language in the combobox with the current running one
   comboLanguage->disconnect();

   for (int i=0; i < languages.count(); ++i) {
      if ( languages.at(i).indexOf("(" + code + ")" ) != -1 ) {
        comboLanguage->setCurrentIndex(i); 
      }
   }

   connect(comboLanguage, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeLanguage()));
}

void Installer::slotHelp()
{
	pcHelp = new dialogHelp();
	switch (installStackWidget->currentIndex()) {
	case 0:
		pcHelp->dialogInit(HELPTEXT0);
		break;
	case 1:
		pcHelp->dialogInit(HELPTEXT1);
		break;
	case 2:
		pcHelp->dialogInit(HELPTEXT2);
		break;
	case 3:
		pcHelp->dialogInit(HELPTEXT3);
		break;
	case 4:
		pcHelp->dialogInit(HELPTEXT4);
		break;
	case 5:
		pcHelp->dialogInit(HELPTEXT5);
		break;
	default:
		pcHelp->dialogInit("Missing help file...");
		break;
	}
	pcHelp->show();
}

void Installer::slotScanNetwork()
{
  QString strength, ssid, security, FileLoad;
  QStringList ifconfout, ifline;
  int foundItem = 0;

  // Clear the list box and disable the add button
  listWidgetWifi->clear();

  // Start the scan and get the output
  //ifconfout = pcbsd::Utils::runShellCommand("ifconfig -v wlan0 up list scan");
  ifconfout = pcbsd::Utils::runShellCommand("ifconfig -v wlan0 list scan");
  
  qDebug() << ifconfout;

  //display the info for each wifi access point
  for(int i=1; i<ifconfout.size(); i++){    //Skip the header line by starting at 1
    ifline = NetworkInterface::parseWifiScanLine(ifconfout[i],true); //get a single line
    //save the info for this wifi
    ssid = ifline[0];
    strength = ifline[4];
    //determine the icon based on if there is security encryption
    security = ifline[6]; //NetworkInterface::getWifiSecurity(ssid,DeviceName);
    if(security.contains("None")){
      FileLoad = ":/modules/images/object-unlocked.png";
    }else{
      FileLoad = ":/modules/images/object-locked.png";
    }
    //Add the wifi access point to the list
    listWidgetWifi->addItem(new QListWidgetItem(QIcon(FileLoad), ssid + " (signal: " +strength + ")") );
    foundItem = 1; //set the flag for wifi signals found
  }
   
  if ( foundItem == 1 )
    listWidgetWifi->setCurrentRow(-1);
}

void Installer::slotAddNewWifi()
{
  if ((listWidgetWifi->currentRow()) ==-1 )
     return;

  QString line = listWidgetWifi->item(listWidgetWifi->currentRow())->text();
  QString ssidc = line.section(" (",0,0,QString::SectionSkipEmpty);
  addNetworkProfile(ssidc);
}

void Installer::addNetworkProfile(QString ssid)
{
  //get the full SSID string
  QString dat = pcbsd::Utils::runShellCommandSearch("ifconfig -v wlan0 list scan",ssid);
  QStringList wdat = NetworkInterface::parseWifiScanLine(dat,true);
  QString SSID = wdat[0];
 
  //Get the Security Type
  QString sectype = wdat[6];
 
  if(sectype == "None"){
    //run the Quick-Connect slot without a key
    slotQuickConnect("",SSID);
  }else{
    //Open the dialog to prompt for the Network Security Key
    dialogNetKey = new netKey();
    //Insert the SSID into the dialog
    dialogNetKey->setSSID(SSID);
    //connect the signal from the dialog to the quick-connect slot
    connect(dialogNetKey,SIGNAL( saved(QString,QString) ),this,SLOT( slotQuickConnect(QString,QString) ) );
    //Activate the dialog
    dialogNetKey->exec();
  }
}

void Installer::slotQuickConnect(QString key,QString SSID){
 
  // Run the wifiQuickConnect function
  NetworkInterface::wifiQuickConnect(SSID,key,"wlan0");
 
  // Restart the network 
  system("/etc/rc.d/netif restart &");
 
  // Move to finish screen
  installStackWidget->setCurrentIndex(5);

  // Save the settings
  saveSettings();
  nextButton->setText(tr("&Finish"));
  backButton->setVisible(false);
  nextButton->disconnect();
  connect(nextButton, SIGNAL(clicked()), this, SLOT(slotFinished()));
}

void Installer::saveSettings()
{
  // Check if we need to change the language
  if ( comboLanguage->currentIndex() != 0 ) {
    QString lang = languages.at(comboLanguage->currentIndex());
    // Grab the language code
    lang.truncate(lang.lastIndexOf(")"));
    lang.remove(0, lang.lastIndexOf("(") + 1);
    qDebug() << "Set Lang:" << lang;
    system("/usr/local/share/pcbsd/scripts/set-lang.sh " + lang.toLatin1());
  }

  // Set the timezone
  QString tz = comboBoxTimezone->currentText();
  if ( tz.indexOf(":") != -1 )
   tz = tz.section(":", 0, 0);
  system("cp /usr/share/zoneinfo/" + tz.toLatin1() + " /etc/localtime");

  // Set the root PW
  QTemporaryFile rfile("/tmp/.XXXXXXXX");
  if ( rfile.open() ) {
    QTextStream stream( &rfile );
      stream << lineRootPW->text();
    rfile.close();
  }
  system("cat " + rfile.fileName().toLatin1() + " | pw usermod root -h 0 ");
  rfile.remove();

  // Create the new home-directory
  system("/usr/local/share/pcbsd/scripts/mkzfsdir.sh /usr/home/" + lineUsername->text().toLatin1() );

  // Create the new username
  QTemporaryFile ufile("/tmp/.XXXXXXXX");
  if ( ufile.open() ) {
    QTextStream stream( &ufile );
      stream << linePW->text();
    ufile.close();
  }
  QString userCmd = " | pw useradd -n \"" + lineUsername->text() + "\" -c \"" + lineName->text() + "\" -h 0 -s \"/bin/csh\" -m -d \"/usr/home/" + lineUsername->text() + "\" -G \"wheel,operator\"";
  system("cat " + ufile.fileName().toLatin1() + userCmd.toLatin1());
  ufile.remove();

  // Enable Flash for the new user
  QString flashCmd = "su -m " + lineUsername->text() + " -c \"flashpluginctl on\"";
  system(flashCmd.toLatin1());
  
}

void Installer::slotKeyLayoutUpdated(QString mod, QString lay, QString var)
{
  kbMod = mod;
  kbLay = lay;
  kbVar = var;
}

void Installer::closeEvent(QCloseEvent *event)
{
   event->ignore();
}