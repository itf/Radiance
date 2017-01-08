import QtQuick 2.3
import radiance 1.0
import QtQuick.Controls 1.4

RadianceTile {
    id: tile;
    property UIEffect uiEffect;
    implicitWidth: 200;
    implicitHeight: 300;
    borderWidth: 0;

    function place() {
        uiEffect.x = 0;
        uiEffect.y = 0;
    }

    function onChildKey(event) {
        if (event.key == Qt.Key_Delete) {
            unload();
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Colon) {
            loadfield.popup();
        }
    }

    function load(name) {
        var component = Qt.createComponent("UIEffect.qml")
        var e = component.createObject(this);
        e.effect.source = name;
        e.Keys.onPressed.connect(onChildKey);

        var prev = uiEffect;
        uiEffect = e;
        if(prev != null) e.destroy();
        place();
        loadfield.popdown();
    }

    function unload() {
        if(uiEffect != null) {
            var prev = uiEffect;
            uiEffect = null;
            prev.destroy();
        }
    }

    onActiveFocusChanged: {
        if(!activeFocus) {
            loadfield.popdown();
        }
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: {
            tile.forceActiveFocus();
        }
    }

    TextField {
        id: loadfield;
        x: 50;
        y: 50;
        visible: false;

        Keys.onPressed: {
            if (event.key == Qt.Key_Escape) {
                popdown();
            }
        }

        function popup() {
            visible = true;
            text = "";
            focus = true;
        }

        function popdown() {
            visible = false;
            focus = false;
        }

        onAccepted: {
            tile.load(text);
            popdown();
        }
    }
}
