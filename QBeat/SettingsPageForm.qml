import QtQuick 2.8
import QtQuick.Controls 2.2

import uk.co.gfrancisdev.qbeat.settings 1.0



/**
 * TODO:
 * - Sort out the right hand margins on the text boxes - auto-fit the window if possible
 * - Add browse buttons for folder selection
 * - Add validation of settings - ensure bs directory is valid etc, check wine prefix is good
 * - On validation failure open actions page/such for sorting it out
 */

Page {
    id: pageSettings
    wheelEnabled: false

    title: qsTr("Settings")
    Settings {
        id: settings
    }

    Column {
        id: column
        anchors.fill: parent
        spacing: 25


        Row {
            id: row
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 25
            anchors.leftMargin: 25
            anchors.rightMargin: 25

            Text {
                id: labelBSInstall
                text: qsTr("Beat Saber Installation")
                font.pixelSize: 12
                width: 150
            }

            TextField {
                id: textBSInstall
                text: settings.bsInstall
                width: 450
                Binding {
                    target: settings
                    property: "bsInstall"
                    value: textBSInstall.text
                }
            }
        }

        Row {
            id: row1
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 25
            anchors.leftMargin: 25
            anchors.rightMargin: 25

            Text {
                id: labelBSInstallType
                text: qsTr("Installation Type")
                font.pixelSize: 12
                width: 150
            }

            ComboBox {
                id: comboBSType
                model: ListModel {
                    id: comboBSTypeItems
                    ListElement { text: "steam"; }
                    ListElement { text: "oculus"; }
                }
                onCurrentTextChanged: settings.gameType = currentText
            }
        }

        Row {
            id: row2
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 25
            anchors.leftMargin: 25
            anchors.rightMargin: 25

            Text {
                id: labelProtonInstall
                text: qsTr("Proton Installation")
                font.pixelSize: 12
                width: 150
            }

            TextField {
                id: textProtonInstall
                text: settings.bsProtonDir
                width: 450
                Binding {
                    target: settings
                    property: "bsProtonDir"
                    value: textProtonInstall.text
                }
            }
        }

        Row {
            id: row3
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 25
            anchors.leftMargin: 25
            anchors.rightMargin: 25

            Text {
                id: labelWinePrefix
                text: qsTr("Wine Prefix")
                font.pixelSize: 12
                width: 150
            }

            TextField {
                id: textWinePrefix
                text: settings.winePrefix
                width: 450
                Binding {
                    target: settings
                    property: "winePrefix"
                    value: textWinePrefix.text
                }
            }
        }

        Row {
            id: row4
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 25
            anchors.leftMargin: 25
            anchors.rightMargin: 25

            Text {
                id: labelBSVersion
                text: qsTr("Beat Saber Version")
                font.pixelSize: 12
                width: 150
            }

            ComboBox {
                id: comboBSVersion
                editable: true
                model: ListModel {
                    id: comboBSVersionItems
                    ListElement { text: "1.3.0"; }
                }
                onCurrentTextChanged: settings.gameVersion = currentText
                onEditTextChanged: settings.gameVersion = editText
            }
        }
    }
}




/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*
/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
