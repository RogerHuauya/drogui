import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';
import '../widgets/form.dart';

class PIDPage extends StatelessWidget {
  final Socket? sock;
  final bool? connected;
  PIDPage({this.connected, this.sock});
  final TextEditingController pidKp = new TextEditingController();
  final TextEditingController pidKi = new TextEditingController();
  final TextEditingController pidKd = new TextEditingController();

  void send(String s) {
    sock!.add(utf8.encode(s));
  }

  @override
  Widget build(BuildContext context) {
    return MyVForm(
      connected: connected,
      childrens: [
        mFormElement(text: 'PID Kp', hint: '-'),
        mFormElement(text: 'PID Ki', hint: '-'),
        mFormElement(text: 'PID Kd', hint: '-'),
      ],
      sock: sock,
      title: 'PID',
    );
  }
}
