import QtQuick 2.8
import QtQuick.Controls 2.2

import uk.co.gfrancisdev.qbeat.settings 1.0

Page {
    id: page
    width: 600
    height: 400
    wheelEnabled: false

    title: qsTr("Settings")
    Settings {
        id: settings
    }

    Text {
        id: labelProtonInstall
        x: 68
        y: 318
        text: qsTr("Proton Installation")
        font.pixelSize: 12
    }

    Text {
        id: labelWinePrefix
        x: 68
        y: 255
        text: qsTr("Wine Prefix")
        font.pixelSize: 12
    }

    ComboBox {
        id: comboBSType
        x: 322
        y: 176
        model: ListModel {
            id: comboBSTypeItems
            ListElement { text: "steam"; }
            ListElement { text: "oculus"; }
        }
        onCurrentTextChanged: settings.gameType = currentText
    }

    Text {
        id: labelBSInstallType
        x: 68
        y: 192
        text: qsTr("Installation Type")
        font.pixelSize: 12
    }

    Text {
        id: labelBSInstall
        x: 68
        y: 135
        text: qsTr("Beat Saber Installation")
        font.pixelSize: 12
    }

    Text {
        id: labelBSVersion
        x: 68
        y: 80
        text: qsTr("Beat Saber Version")
        font.pixelSize: 12
    }

    ComboBox {
        id: comboBSVersion
        editable: true
        x: 322
        y: 64
        model: ListModel {
            id: comboBSVersionItems
            ListElement { text: "1.3.0"; }
        }
        onCurrentTextChanged: settings.gameVersion = currentText
        onEditTextChanged: settings.gameVersion = editText
    }

    TextField {
        id: textBSInstall
        x: 322
        y: 120
        text: settings.bsInstall
        Binding {
            target: settings
            property: "bsInstall"
            value: textBSInstall.text
        }
    }

    TextField {
        id: textWinePrefix
        x: 322
        y: 240
        text: settings.winePrefix
        Binding {
            target: settings
            property: "winePrefix"
            value: textWinePrefix.text
        }
    }

    TextField {
        id: textProtonInstall
        x: 322
        y: 303
        text: settings.bsProtonDir
        Binding {
            target: settings
            property: "bsProtonDir"
            value: textProtonInstall.text
        }
    }
}
