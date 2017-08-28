#ifndef OPTIONSMODEL_H
#define OPTIONSMODEL_H

#include <QAbstractListModel>

/** Interface from Qt to configuration data structure for Floripacoin client.
   To Qt, the options are presented as a list with the different options
   laid out vertically.
   This can be changed to a tree once the settings become sufficiently
   complex.
 */
class OptionsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit OptionsModel(QObject *parent = 0);

    enum OptionID {
        StartAtStartup,    // bool
        MinimizeToTray,    // bool
        MapPortUPnP,       // bool
        MinimizeOnClose,   // bool
        ProxyUse,          // bool
        ProxyIP,           // QString
        ProxyPort,         // int
        ProxySocksVersion, // int
        Fee,               // qint64
        DisplayUnit,       // FloripacoinUnits::Unit
        DisplayAddresses,  // bool
        StartMiningAtStartup, //bool
        Language,          // QString
        AllowSounds,        // bool
        CheckUpdatesAtStartup, // bool
        OptionIDRowCount
    };

    void Init();
    void Reset();

    /* Migrate settings from wallet.dat after app initialization */
    bool Upgrade(); /* returns true if settings upgraded */

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    /* Explicit getters */
    qint64 getTransactionFee();
    bool getMinimizeToTray() { return fMinimizeToTray; }
    bool getMinimizeOnClose() { return fMinimizeOnClose; }
    int getDisplayUnit() { return nDisplayUnit; }
    bool getDisplayAddresses() { return bDisplayAddresses; }
    bool getStartMiningAtStartup() { return bStartMiningAtStartup; }
    QString getLanguage() { return language; }
    bool getAllowSounds() { return bAllowSounds; }
    bool getCheckUpdatesAtStartup() { return bCheckUpdatesAtStartup; }

private:
    int nDisplayUnit;
    bool bDisplayAddresses;
    bool fMinimizeToTray;
    bool fMinimizeOnClose;
    bool bStartMiningAtStartup;
    QString language;
    bool bAllowSounds;
    bool bCheckUpdatesAtStartup;

signals:
    void displayUnitChanged(int unit);
};

#endif // OPTIONSMODEL_H
