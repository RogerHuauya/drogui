import 'dart:io';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter/cupertino.dart';
import 'package:flutter/services.dart';
import 'package:test_app/utils/registerMap.dart';

class MasterWidget extends StatefulWidget {
  final Widget child;

  MasterWidget({Key? key, required this.child}) : super(key: key);

  @override
  State<StatefulWidget> createState() => MasTerWidgetState();
}

class MasTerWidgetState extends State<MasterWidget> {
  late Socket socket;
  bool connected = false;
  ByteData regs = ByteData(300 * 4);

  bool sendSocket(String message) {
    if (connected) {
      socket.add(utf8.encode(message));
      return true;
    }
    setState(() {});
    return false;
  }

  bool droguiWrite(int reg, double val) {
    if (!connected) return false;
    ByteData msg = ByteData(8);

    msg.setUint8(0, 'd'.codeUnitAt(0));
    msg.setUint8(1, '\$'.codeUnitAt(0));
    msg.setUint8(2, 'w'.codeUnitAt(0));
    msg.setUint8(3, reg);
    msg.setFloat32(4, val, Endian.little);

    socket.add(msg.buffer.asUint8List().cast<int>());
    return true;
  }

  bool droguiRead(int reg) {
    if (!connected) return false;
    ByteData msg = ByteData(4);

    msg.setUint8(0, 'd'.codeUnitAt(0));
    msg.setUint8(1, '\$'.codeUnitAt(0));
    msg.setUint8(2, 'r'.codeUnitAt(0));
    msg.setUint8(3, reg);

    socket.add(msg.buffer.asUint8List().cast<int>());
    return true;
  }

  bool droguiCmd(String cmd) {
    if (!connected) return false;
    ByteData msg = ByteData(4);

    msg.setUint8(0, 'd'.codeUnitAt(0));
    msg.setUint8(1, '\$'.codeUnitAt(0));
    msg.setUint8(2, 'c'.codeUnitAt(0));
    msg.setUint8(3, cmd.codeUnitAt(0));

    socket.add(msg.buffer.asUint8List().cast<int>());
    return true;
  }

  double getReg(int reg) {
    return regs.getFloat32(reg * 4);
  }

  void serverClosed() {
    setState(() {
      print('disconnected');
      connected = false;
    });
  }

  void recieved(Uint8List message) {
    if (message[0] == 'd'.codeUnitAt(0) &&
        message[1] == '\$'.codeUnitAt(0) &&
        message[2] == 'w'.codeUnitAt(0)) {
      int reg = message[3];
      double dato = message.buffer.asByteData().getFloat32(4, Endian.little);
      regs.setFloat32(reg * 4, dato);
      print(message);
      print('reg:$reg');
      print('dato:$dato');

      setState(() {});
    }
  }

  void connectSocket() async {
    try {
      socket = await Socket.connect('191.98.175.75', 1194,
          timeout: Duration(seconds: 3));

      socket.listen(recieved, onDone: serverClosed);
      connected = true;
      print('connected');
    } on Exception catch (_) {
      connected = false;
      print('cannot connect');
    }
    setState(() {});
  }

  void closeSocket() {
    setState(() {
      if (connected) socket.close();
      connected = false;
      print('disconnected');
    });
  }

  @override
  Widget build(BuildContext context) {
    return GlobalWidget(child: widget.child, master: this);
  }
}

class GlobalWidget extends InheritedWidget {
  //final String texto;
  final MasTerWidgetState master;

  const GlobalWidget(
      {Key? key,
      required this.master,
      //required this.texto,
      required Widget child})
      : super(key: key, child: child);

  @override
  bool updateShouldNotify(GlobalWidget oldWidget) {
    return true;
  }

  static MasTerWidgetState of(BuildContext context) {
    return context.dependOnInheritedWidgetOfExactType<GlobalWidget>()!.master;
  }
}
