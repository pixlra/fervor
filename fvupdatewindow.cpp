#include "fvupdatewindow.h"
#ifdef FERVOR_RELEASE_NOTES
#include "ui_fvupdatewindow.h"
#else
#include "ui_fvupdatewindow_no_release_notes.h"
#endif
#include "fvupdater.h"
#include "fvavailableupdate.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>


FvUpdateWindow::FvUpdateWindow(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::FvUpdateWindow)
{
	m_ui->setupUi(this);

	m_appIconScene = 0;
	setWindowModality(Qt::ApplicationModal);

	// Delete on close
	setAttribute(Qt::WA_DeleteOnClose, true);

	// Set the "new version is available" string
	QString newVersString = m_ui->newVersionIsAvailableLabel->text().arg(QString::fromUtf8(FV_APP_NAME));
	m_ui->newVersionIsAvailableLabel->setText(newVersString);

	// Connect buttons
	connect(m_ui->installUpdateButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(InstallUpdate()));
	connect(m_ui->skipThisVersionButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(SkipUpdate()));
	connect(m_ui->remindMeLaterButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(RemindMeLater()));
}

FvUpdateWindow::~FvUpdateWindow()
{
#ifdef FERVOR_RELEASE_NOTES
	m_ui->releaseNotesWebView->stop();
#endif
	delete m_ui;
}

bool FvUpdateWindow::UpdateWindowWithCurrentProposedUpdate()
{
	FvAvailableUpdate* proposedUpdate = FvUpdater::sharedUpdater()->GetProposedUpdate();
	if (! proposedUpdate) {
		return false;
	}

	QString downloadString = m_ui->wouldYouLikeToDownloadLabel->text()
			.arg(QString::fromUtf8(FV_APP_NAME), proposedUpdate->GetEnclosureVersion(), QString::fromUtf8(FV_APP_VERSION));
	m_ui->wouldYouLikeToDownloadLabel->setText(downloadString);
#ifdef FERVOR_RELEASE_NOTES
	m_ui->releaseNotesWebView->stop();
	m_ui->releaseNotesWebView->load(proposedUpdate->GetReleaseNotesLink());
#endif
	return true;
}

void FvUpdateWindow::closeEvent(QCloseEvent* event)
{
	FvUpdater::sharedUpdater()->updaterWindowWasClosed();
	event->accept();
}
