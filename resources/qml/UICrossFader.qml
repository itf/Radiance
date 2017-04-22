import QtQuick 2.3
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import radiance 1.0

FocusScope {
    property alias crossfader: crossfader;
    implicitWidth: 100;
    implicitHeight: 100;

    RadianceTile {
        anchors.fill: parent;
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_J)
            slider.value -= 0.1;
        else if (event.key == Qt.Key_K)
            slider.value += 0.1;
    }

    ColumnLayout {
        anchors.fill: parent;
        anchors.margins: 15;

        Item {
            Layout.preferredHeight: width;
            Layout.fillWidth: true;
            layer.enabled: true;

            CheckerboardBackground {
                anchors.fill: parent;
            }

            CrossFader {
                id: crossfader;
                anchors.fill: parent;
                parameter: slider.value;
            }
        }

        Slider {
            id: slider;
            Layout.fillWidth: true;
            minimumValue: 0;
            maximumValue: 1;
        }
    }
}
