#include "GroupConfigDialog.h"
#include "ui_GroupConfigDialog.h"

#include <QList>
#include <QCheckBox>
#include <QInputDialog>

#include "main.h"


static int i0 = qRegisterMetaTypeStreamOperators < Config >();
const char GroupConfigDialog::COMMAND_TAG[] = "GroupCommands";

QDataStream &operator<<(QDataStream &out, const Config &config)
{
    out << config.isGroup << config.value;
    return out;
}

QDataStream &operator>>(QDataStream &in, Config &config)
{
    in >> config.isGroup >> config.value;
    return in;
}


GroupConfigDialog::GroupConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupConfigDialog)
{
    ui->setupUi(this);
    setAttribute( Qt::WA_DeleteOnClose );

    connect(ui->addConfigButton, SIGNAL(clicked()), this, SLOT(onAddConfig()));
    connect(ui->removeConfigButton, SIGNAL(clicked()), this, SLOT(onRemoveConfig()));
    connect(ui->configCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(onConfigSelected(QString)));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(onAdd()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(onRemove()));
    connect(ui->groupRadio, SIGNAL(toggled(bool)), this, SLOT(onActionsChaned(bool)));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onControlButtonsClicked(QAbstractButton*)));

    QSettings settings;
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::ReadWrite);
    data = settings.value(COMMAND_TAG).toByteArray();
    if(data.size())
        dataStream >> mConfigs;
}

GroupConfigDialog::~GroupConfigDialog()
{
    delete ui;
}

void GroupConfigDialog::setServers(const QStringList &servers)
{
    QVBoxLayout* layout = new QVBoxLayout();
    foreach(const QString item, servers)
    {
        QRadioButton* radio = new QRadioButton(ui->serversGroup);
        radio->setText(item);
        radio->setObjectName(item);
        connect(radio, SIGNAL(pressed()), this, SLOT(onHostSelected()));
        layout->addWidget(radio);
    }
    if(servers.isEmpty())
    {
        QLabel* label = new QLabel(ui->serversGroup);
        label->setText(tr("<No servers>"));
        layout->addWidget(label);
    }

    ui->serversGroup->setLayout(layout);
}

void GroupConfigDialog::setAvailableOptions(const QStringList &options)
{
    ui->availableList->clear();
    ui->selectedList->clear();

    ui->availableList->addItems(options);

    if(options.contains(mConfigs.value(ui->configCombo->currentText()).value(mCurrentHost).value))
        ui->selectedList->addItem(mConfigs.value(ui->configCombo->currentText()).value(mCurrentHost).value);
}

void GroupConfigDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }

    QDialog::changeEvent(event);
}

void GroupConfigDialog::init()
{
    emit requestServers();
    ui->groupRadio->setChecked(true);

    if(mConfigs.size())
    {
        ui->configCombo->addItems(mConfigs.keys());
        ui->configCombo->removeItem(ui->configCombo->findText(""));
        mConfigs.remove("");
    }
}

void GroupConfigDialog::onAdd()
{
    if(!ui->availableList->currentItem() ||
            mCurrentHost.isEmpty())
        return;

    QString item = ui->availableList->currentItem()->text();
    ui->selectedList->clear();
    ui->selectedList->addItem(item);
    mConfigs[ui->configCombo->currentText()][mCurrentHost].value = item;
    mConfigs[ui->configCombo->currentText()][mCurrentHost].isGroup = ui->groupRadio->isChecked();
}

void GroupConfigDialog::onRemove()
{
    mConfigs[ui->configCombo->currentText()][mCurrentHost].value = "";
    delete ui->selectedList->takeItem(ui->selectedList->currentIndex().row());
}

void GroupConfigDialog::onHostSelected()
{
    QRadioButton* btn = dynamic_cast<QRadioButton*>(sender());
    if(!btn)
        return;

    mCurrentHost = btn->text();

    ui->groupRadio->setChecked(mConfigs.value(ui->configCombo->currentText())
                                        .value(mCurrentHost).isGroup);
    ui->scriptRadio->setChecked(!mConfigs.value(ui->configCombo->currentText())
                                        .value(mCurrentHost).isGroup);
    onActionsChaned(mConfigs.value(ui->configCombo->currentText())
                    .value(mCurrentHost).isGroup);
}

void GroupConfigDialog::onActionsChaned(bool isGroupsSelected)
{
    if(mCurrentHost.isEmpty())
        return;

    if(isGroupsSelected)
        emit requestGroups(mCurrentHost);
    else
        emit requestScripts(mCurrentHost);
}

void GroupConfigDialog::onControlButtonsClicked(QAbstractButton *pButton)
{
    switch(ui->buttonBox->buttonRole(pButton))
    {
    case QDialogButtonBox::ApplyRole:
    case QDialogButtonBox::AcceptRole:
    {
        QSettings settings;
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << mConfigs;
        settings.setValue(COMMAND_TAG, data);
        break;
    }
    case QDialogButtonBox::RejectRole:
    {
        close();
        break;
    }
    default:
        break;
    }
}

void GroupConfigDialog::onAddConfig()
{
    bool ok;
    QInputDialog input(this);

    input.setInputMode(QInputDialog::TextInput);
    input.setLabelText(tr("Config name:"));
    input.setWindowTitle(tr("Add Config"));
    input.setTextValue(QString());
    input.resize(400, input.height());

    ok = input.exec();
    QString configName = input.textValue();

    if (ok == QDialog::Accepted &&
            !configName.isEmpty() &&
            ui->configCombo->findText(configName) == -1)
    {
        foreach(QRadioButton* button, ui->serversGroup->findChildren<QRadioButton* >())
        {
            mConfigs[configName].insert(button->text(), Config());
        }
        ui->configCombo->addItem(configName);

        ui->configCombo->removeItem(ui->configCombo->findText(""));
        mConfigs.remove("");
    }
}

void GroupConfigDialog::onRemoveConfig()
{
    QString key = ui->configCombo->currentText();
    key.replace(' ', '_');

    ui->configCombo->removeItem(ui->configCombo->currentIndex());
    mConfigs.remove(key);
}

void GroupConfigDialog::onConfigSelected(QString configName)
{
    if(!mConfigs.contains(configName))
        return;

    if(!ui->serversGroup->findChild<QRadioButton* >())
        return;

    QString host = ui->serversGroup->findChild<QRadioButton* >()->text();
    Config config = mConfigs.value(configName).value(host);

    if(config.isGroup)
    {
        ui->groupRadio->setChecked(true);
    }
    else
    {
        ui->scriptRadio->setChecked(true);
    }

    ui->selectedList->clear();
    if(ui->serversGroup->findChild<QRadioButton* >(host))
    {
        ui->serversGroup->findChild<QRadioButton* >(host)->setChecked(true);
        mCurrentHost = host;
        onActionsChaned(config.isGroup);
    }
}


QStringList GroupConfigDialog::getConfigNames()
{
    QSettings settings;
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::ReadWrite);
    data = settings.value(COMMAND_TAG).toByteArray();
    GroupConfigDialog::TConfigMap configs;
    dataStream >> configs;
    return configs.keys();
}

QHash<QString, Config> GroupConfigDialog::getConfig(const QString &name)
{
    QSettings settings;
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::ReadWrite);
    data = settings.value(COMMAND_TAG).toByteArray();
    GroupConfigDialog::TConfigMap configs;
    dataStream >> configs;
    return configs.value(name);
}
