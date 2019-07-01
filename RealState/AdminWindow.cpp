#include "AdminWindow.h"

AdminWindow::AdminWindow(ContractManager &conManager, int userId, QWidget *parent)
	: MainWindow(parent), conManager(conManager), userManager(conManager.get_userManager()),
	resManager(conManager.get_residenceManager()), userId(userId)
{
	setWindowTitle("Admin page");

	QMenu *editcrateButton = menuBar()->addMenu("Edit commission &rate");
	QMenu *changeinfoButton = menuBar()->addMenu("Edit user &information");

	tabs = new QTabWidget();

	usersTab = new UsersTab(userManager);
	tabs->addTab(usersTab, "&Users");

	ressTab = new ResidencesTab(conManager, userId);
	tabs->addTab(ressTab, "&Residences");

	consTab = new ContractsTab(conManager, userId);
	tabs->addTab(consTab, "&Contracts");
	
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(tabs);

	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	setCentralWidget(widget);

	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
	connect(editcrateButton, SIGNAL(triggered(QAction*)), this, SLOT(crateedit_clicked()));
	connect(changeinfoButton, SIGNAL(triggered(QAction*)), this, SLOT(changeinfo_clicked()));
}

AdminWindow::~AdminWindow()
{
}

void AdminWindow::crateedit_clicked(QAction *)
{
	(new CommissionRateWindow(conManager, this))->exec();
	consTab->populate();
}

void AdminWindow::changeinfo_clicked(QAction *)
{
	auto wnd = new UserInfoWindow(userManager, userId, this);
	wnd->setWindowModality(Qt::ApplicationModal);
	wnd->show();
	connect(wnd, SIGNAL(closed()), this, SLOT(changeinfo_closed()));
}

void AdminWindow::changeinfo_closed()
{
	usersTab->populate();
}

void AdminWindow::tab_changed(int index)
{
	switch (index)
	{
	case 0:
		usersTab->populate();
		break;
	case 1:
		ressTab->populate();
		break;
	case 2:
		consTab->populate();
		break;
	default:
		throw std::logic_error("");
		break;
	}
}
