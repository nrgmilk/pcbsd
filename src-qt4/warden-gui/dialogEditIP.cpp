/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
// QT Includes
#include <QInputDialog>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "pcbsd-utils.h"

// Local Includes
#include "dialogEditIP.h"

void dialogEditIP::programInit(QString name)
{
  JailDir = pcbsd::Utils::getValFromPCConf("/usr/local/etc/warden.conf", "JDIR");
  jailName = name;
  QString tmp;

  // Lets start loading IP addresses
  QFile file( JailDir + "/." + jailName + ".meta/ipv4" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); tmp=""; tmp = stream.readLine();
     lineIP->setText(tmp);
     if ( ! tmp.isEmpty() )
        checkIPv4->setChecked(true);
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/bridge-ipv4" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); tmp=""; tmp = stream.readLine();
     lineIPBridge->setText(tmp);
     if ( ! tmp.isEmpty() )
        checkIPv4Bridge->setChecked(true);
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/defaultrouter-ipv4" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); tmp=""; tmp = stream.readLine();
     lineIPRouter->setText(tmp);
     if ( ! tmp.isEmpty() )
        checkIPv4Router->setChecked(true);
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/ipv6" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); tmp=""; tmp = stream.readLine();
     lineIP6->setText(tmp);
     if ( ! tmp.isEmpty() )
        checkIPv6->setChecked(true);
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/bridge-ipv6" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); tmp=""; tmp = stream.readLine();
     lineIP6Bridge->setText(tmp);
     if ( ! tmp.isEmpty() )
        checkIPv6Bridge->setChecked(true);
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/defaultrouter-ipv6" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); tmp=""; tmp = stream.readLine();
     lineIP6Router->setText(tmp);
     if ( ! tmp.isEmpty() )
        checkIPv6Router->setChecked(true);
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-ipv4" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); 
     while ( ! file.atEnd() )
       IPv4Alias << stream.readLine();
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-bridge-ipv4" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); 
     while ( ! file.atEnd() )
       IPv4AliasBridge << stream.readLine();
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-ipv6" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); 
     while ( ! file.atEnd() )
       IPv6Alias << stream.readLine();
     file.close();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-bridge-ipv6" );
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
     QTextStream stream( &file ); 
     while ( ! file.atEnd() )
       IPv6AliasBridge << stream.readLine();
     file.close();
  }

  // Our buttons / slots
  connect( checkIPv4, SIGNAL( clicked() ), this, SLOT( slotCheckChecks() ) );
  connect( checkIPv4Bridge, SIGNAL( clicked() ), this, SLOT( slotCheckChecks() ) );
  connect( checkIPv4Router, SIGNAL( clicked() ), this, SLOT( slotCheckChecks() ) );
  connect( checkIPv6, SIGNAL( clicked() ), this, SLOT( slotCheckChecks() ) );
  connect( checkIPv6Bridge, SIGNAL( clicked() ), this, SLOT( slotCheckChecks() ) );
  connect( checkIPv6Router, SIGNAL( clicked() ), this, SLOT( slotCheckChecks() ) );

  connect( comboIPType, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotComboIPChanged() ) );

  connect( pushSave, SIGNAL( clicked() ), this, SLOT( slotSaveClicked() ) );
  connect( pushCancel, SIGNAL( clicked() ), this, SLOT( slotCancelClicked() ) );
  connect( pushAdd, SIGNAL( clicked() ), this, SLOT( slotAddClicked() ) );
  connect( pushRemove, SIGNAL( clicked() ), this, SLOT( slotRemClicked() ) );

  slotCheckChecks();
  slotComboIPChanged();
}

void dialogEditIP::slotComboIPChanged()
{
   QStringList curList;

   // IPv4 Aliases
   if ( comboIPType->currentIndex() == 0 )
      curList = IPv4Alias;

   // IPv4 Bridge Aliases
   if ( comboIPType->currentIndex() == 1 )
      curList = IPv4AliasBridge;

   // IPv6 Aliases
   if ( comboIPType->currentIndex() == 2 )
      curList = IPv6Alias;

   // IPv6 Bridge Aliases
   if ( comboIPType->currentIndex() == 3 )
      curList = IPv6AliasBridge;

   listIP->clear();
   for (int i = 0; i < curList.size(); ++i)
          listIP->addItem(curList.at(i));

}

void dialogEditIP::slotCheckChecks()
{
  lineIP->setEnabled(checkIPv4->isChecked());
  lineIPBridge->setEnabled(checkIPv4Bridge->isChecked());
  lineIPRouter->setEnabled(checkIPv4Router->isChecked());
  lineIP6->setEnabled(checkIPv6->isChecked());
  lineIP6Bridge->setEnabled(checkIPv6Bridge->isChecked());
  lineIP6Router->setEnabled(checkIPv6Router->isChecked());

}

void dialogEditIP::slotCancelClicked()
{
   close();
}

bool dialogEditIP::sanityCheckSettings()
{
  if ( checkIPv4->isChecked() && ! checkValidBlock(lineIP->text(), QString("IPv4")) ) {
        QMessageBox::critical(this, tr("Warden"), tr("Invalid IPv4 address!"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
  }
  if ( checkIPv4Bridge->isChecked() && ! checkValidBlock(lineIPBridge->text(), QString("IPv4")) ) {
        QMessageBox::critical(this, tr("Warden"), tr("Invalid IPv4 bridge address!"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
  }
  if ( checkIPv4Router->isChecked() && ! checkValidBlock(lineIPRouter->text(), QString("IPv4")) ) {
        QMessageBox::critical(this, tr("Warden"), tr("Invalid IPv4 router address!"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
  }
  if ( checkIPv6->isChecked() && ! checkValidBlock(lineIP6->text(), QString("IPv6")) ) {
        QMessageBox::critical(this, tr("Warden"), tr("Invalid IPv6 address!"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
  }
  if ( checkIPv6Bridge->isChecked() && ! checkValidBlock(lineIP6Bridge->text(), QString("IPv6")) ) {
        QMessageBox::critical(this, tr("Warden"), tr("Invalid IPv6 bridge address!"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
  }
  if ( checkIPv6Router->isChecked() && ! checkValidBlock(lineIP6Router->text(), QString("IPv6")) ) {
        QMessageBox::critical(this, tr("Warden"), tr("Invalid IPv6 router address!"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
  }

  return true;
}

void dialogEditIP::slotSaveClicked()
{
   if ( sanityCheckSettings() )
   {
     saveSettings();
     emit saved();
     close();
   }
}

void dialogEditIP::saveSettings()
{
  QString tmp;
  QFile file;

  // Start saving settings
  file.setFileName( JailDir + "/." + jailName + ".meta/alias-ipv4" );
  if ( ! IPv4Alias.isEmpty() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file );
       for (int i = 0; i < IPv4Alias.size(); ++i)
         stream << IPv4Alias.at(i);
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-bridge-ipv4" );
  if ( ! IPv4AliasBridge.isEmpty() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file );
       for (int i = 0; i < IPv4AliasBridge.size(); ++i)
         stream << IPv4AliasBridge.at(i);
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-ipv6" );
  if ( ! IPv6Alias.isEmpty() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file );
       for (int i = 0; i < IPv6Alias.size(); ++i)
         stream << IPv6Alias.at(i);
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/alias-bridge-ipv6" );
  if ( ! IPv6AliasBridge.isEmpty() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file );
       for (int i = 0; i < IPv6AliasBridge.size(); ++i)
         stream << IPv6AliasBridge.at(i);
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/ipv4" );
  if ( checkIPv4->isChecked() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file ); tmp = lineIP->text();
       if (tmp.indexOf("/") == -1)
         tmp = tmp + "/24"; 
       stream << tmp;
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/bridge-ipv4" );
  if ( checkIPv4Bridge->isChecked() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file ); tmp = lineIPBridge->text();
       if (tmp.indexOf("/") == -1)
         tmp = tmp + "/24"; 
       stream << tmp;
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/defaultrouter-ipv4" );
  if ( checkIPv4Router->isChecked() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file ); tmp = lineIPRouter->text();
       if (tmp.indexOf("/") == -1)
         tmp = tmp + "/24"; 
       stream << tmp;
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/ipv6" );
  if ( checkIPv6->isChecked() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file ); tmp = lineIP6->text();
       if (tmp.indexOf("/") == -1)
         tmp = tmp + "/64"; 
       stream << tmp;
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/bridge-ipv6" );
  if ( checkIPv6Bridge->isChecked() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file ); tmp = lineIP6Bridge->text();
       if (tmp.indexOf("/") == -1)
         tmp = tmp + "/64"; 
       stream << tmp;
       file.close();
    }
  } else {
    file.remove();
  }

  file.setFileName( JailDir + "/." + jailName + ".meta/defaultrouter-ipv6" );
  if ( checkIPv6Router->isChecked() ) {
    if ( file.open( QIODevice::WriteOnly ) ) {
       QTextStream stream( &file ); tmp = lineIP6Router->text();
       if (tmp.indexOf("/") == -1)
         tmp = tmp + "/64"; 
       stream << tmp;
       file.close();
    }
  } else {
    file.remove();
  }

        /*
	QStringList IPs;
	for ( int i=0; i<listIP->count() ; i++)
		IPs << listIP->item(i)->text();

        qDebug() << "Updating IPs...";
        QProcess ipcmd;
        if ( IPs.count() == 0 )
          ipcmd.start(QString("warden"), QStringList() << "set" << "ip" << wardenIP << "" );
        else
          ipcmd.start(QString("warden"), QStringList() << "set" << "ip" << wardenIP << IPs.join(",") );

	setEnabled(false);
        ipcmd.waitForFinished(1000);
        while ( ipcmd.state() != QProcess::NotRunning ) {
                ipcmd.waitForFinished(100);
                QCoreApplication::processEvents();
        }
        */
	
}

void dialogEditIP::slotAddClicked()
{
   bool ok;
   QString address = QInputDialog::getText(this, tr("Add IP"),
			tr("IP Address:"), QLineEdit::Normal,
			QString(), &ok);
   if ( ! ok )
      return;

   if ( ! checkValidBlock(address, "IPv4") || checkValidBlock(address, "IPv6") )
      QMessageBox::critical(this, tr("Warden"), \
                                tr("Please enter a valid IPV4 or IPV6 address!"), \
                                QMessageBox::Ok, \
                                QMessageBox::Ok);

   // IPv4 Aliases
   if ( comboIPType->currentIndex() == 0 ) {
      if ( address.indexOf("/") == -1 ) 
         address = address + "/24";
      IPv4Alias << address;
      listIP->addItem(address);
   }

   // IPv4 Bridge Aliases
   if ( comboIPType->currentIndex() == 1 ) {
      if ( address.indexOf("/") == -1 ) 
         address = address + "/24";
      IPv4AliasBridge << address;
      listIP->addItem(address);
   }

   // IPv6 Aliases
   if ( comboIPType->currentIndex() == 2 ) {
      if ( address.indexOf("/") == -1 ) 
         address = address + "/64";
      IPv6Alias << address;
      listIP->addItem(address);
   }

   // IPv6 Bridge Aliases
   if ( comboIPType->currentIndex() == 3 ) {
      if ( address.indexOf("/") == -1 ) 
         address = address + "/64";
      IPv6AliasBridge << address;
      listIP->addItem(address);
   }

}

bool dialogEditIP::checkValidBlock(QString block, QString type)
{
   QString url = block;

   // Strip off the /24 part
   if ( url.indexOf("/") != -1 )
      url.truncate(url.indexOf("/"));

   if ( type == "IPv4" ) {
      if ( ! pcbsd::Utils::validateIPV4(url) )
	return false;

   } else {
      if ( ! pcbsd::Utils::validateIPV6(url) )
	return false;

   }

   return true;
}

void dialogEditIP::slotRemClicked()
{
	if ( ! listIP->currentItem() )
		return;

	delete listIP->currentItem();
}