import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 800
    height: 480
    title: qsTr("Watcher")

    color:"#68d1ea"

    function startUp(){
        var init_values = monitor.init();
        console.log(init_values.graphics_name);

        monitorTimer.start();
    }

    AnimatedImage { 
        id: animation; 
        source: "img/weiss.gif"; 
        height:440; 
        fillMode: Image.PreserveAspectFit;
        anchors.right: parent.right;
        anchors.rightMargin: 25;
        anchors.top: parent.top;
        anchors.topMargin: 20;
    }

    Timer {
        id: monitorTimer;
        interval: 1000; 
        running: false; 
        repeat: true;
        onTriggered: function(){
            var data = monitor.getData();
            console.log("Temperatura da GPU: " + data.graphics_temp + "°C; Uso da GPU: " + data.graphics_usage + "%");
        };
    }

    onClosing: {
        if(monitorTimer.running)
            monitorTimer.stop();
    }
}
