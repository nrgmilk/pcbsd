/* PCDM Login Manager:
*  Written by Ken Moore (ken@pcbsd.org) 2012/2013
*  Copyright(c) 2013 by the PC-BSD Project
*  Available under the 3-clause BSD license
*/

/*
 Sub-classed widget for a login entry box
*/

#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <QWidget>
#include <QFile>
#include <QPushButton>
#include <QToolButton>
#include <QFormLayout>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QKeyEvent>
#include <QAction>
#include <QGroupBox>
#include <QListWidget>
#include <QAbstractItemView>

class LoginWidget : public QGroupBox
{
	Q_OBJECT

  public:
	LoginWidget(QWidget* parent = 0);
	~LoginWidget();

	//Get the currently selected items
	QString currentUsername();
	QString currentPassword();
	void setCurrentUser(QString);
	void setUsernames(QStringList);
        void displayHostName(QString);
	//Manually set the "back" (up/left) and "forward" (down/right) button icons
	void changeButtonIcon(QString button, QString iconFile, QSize iconSize);
	//Change the style sheet for all widget items (see QtStyle options for more)
	void changeStyleSheet(QString item, QString style);
        
        void keyPressEvent(QKeyEvent *e);
        void retranslateUi();
        void resetFocus(QString item="");
        void allowPasswordView(bool);
  
  private:
  	QComboBox* listUsers;
  	QListWidget* listUserBig;
  	QLineEdit* linePassword;
	QToolButton* pushLogin;
	QToolButton* pushViewPassword;
	QToolButton *pushUserIcon, *userIcon;
	

	QStringList idL;
        QString hostName;
	bool userSelected, pwVisible, allowPWVisible;

	void updateWidget();

  private slots:
	void slotUserActivated(QAction*);
	void slotUserClicked(QListWidgetItem*);
	void slotUserHighlighted(int);
	void slotUserSelected();
	void slotUserUnselected();
  	void slotTryLogin();
  	void slotChooseUser(int);
  	void slotChangePWView();
	
  signals:
	//Emits these signals whenever a login request is detected
	void loginRequested(QString user, QString password);
	void UserSelected(QString user);
	void UserChanged(QString user);
	void escapePressed();

};
#endif