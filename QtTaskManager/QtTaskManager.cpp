#include "QtTaskManager.h"

QtTaskManager::QtTaskManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.lwTaskBar, &QListWidget::doubleClicked, this, &QtTaskManager::handleNewTask);
    connect(ui.lwTaskBar, &QListWidget::itemChanged, this, &QtTaskManager::taskChanged);
    connect(ui.pbAppend, &QPushButton::clicked, this, &QtTaskManager::handleNewTask);
    connect(ui.pbDelete, &QPushButton::clicked, this, &QtTaskManager::deleteTask);
}

QtTaskManager::~QtTaskManager()
{}

//---------------------------------------------------------------------------
// Handler for ckecking if a new task fits
//---------------------------------------------------------------------------
void QtTaskManager::handleNewTask() {
  int selTask = ui.lwTaskBar->currentRow();
  if (selTask < ui.lwTaskBar->count() - 1)
    return;

  // create heap-allocated dialog to allow user enter the task text
  auto taskDialog{ std::make_unique<QDialog>(this) };
  taskDialog->setWindowTitle(tr("Enter your task text"));

  QFormLayout* layout{ new QFormLayout(taskDialog.get()) };
  QTextEdit* teTaskField{ new QTextEdit(taskDialog.get()) };
  // cleanup if failed
  connect(teTaskField, &QTextEdit::selectionChanged, [&] {
    if (teTaskField->textColor() == QColor("red")) {
      teTaskField->clear();
      teTaskField->setTextColor(QColor("black"));
    }
    });
  layout->addRow(teTaskField);

  // QDialogButtonBox
  QDialogButtonBox* btnBox{ new QDialogButtonBox(taskDialog.get()) };
  btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addRow(btnBox);

  connect(btnBox, &QDialogButtonBox::rejected, taskDialog.get(), &QDialog::reject);
  connect(btnBox, &QDialogButtonBox::accepted, taskDialog.get(), [&] {
    QString taskText = teTaskField->toPlainText();

    // something has been entered
    if (taskText.size()) {
      addTask(taskText);
      taskDialog->accept();
    }
    else {
      teTaskField->setTextColor(QColor("red"));
      teTaskField->setText("No task data entered!"); 
    }
  });
    
  taskDialog->exec();
};

//---------------------------------------------------------------------------
// Fill table with a new task
//---------------------------------------------------------------------------
void QtTaskManager::addTask(QString taskText) {
  QListWidgetItem* newItem = new QListWidgetItem(taskText);
  newItem->setCheckState(Qt::Unchecked);

  int taskCount = ui.lwTaskBar->count();
  ui.lwTaskBar->insertItem(taskCount-1, newItem);
};

//---------------------------------------------------------------------------
// Delete selected task
//---------------------------------------------------------------------------
void QtTaskManager::deleteTask() {
  int selTask = ui.lwTaskBar->currentRow();
  if (selTask < ui.lwTaskBar->count()-1)
    ui.lwTaskBar->takeItem(selTask);
}

//---------------------------------------------------------------------------
// Check if an item got checked state
//---------------------------------------------------------------------------
void QtTaskManager::taskChanged(QListWidgetItem* item) {
  Qt::CheckState itemState = item->checkState();
  QFont font{};

  if (itemState == Qt::Checked)   
    font.setStrikeOut(true);
  else if (itemState == Qt::Unchecked)
    font.setStrikeOut(false);

  item->setFont(font);
}