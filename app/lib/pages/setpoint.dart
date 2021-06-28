import 'package:flutter/material.dart';
import 'package:test_app/widgets/form.dart';
import 'dart:io';
import 'dart:convert';

class SetPointPage extends StatelessWidget {
  final Socket? sock;
  final bool? connected;
  SetPointPage({this.connected, this.sock});

  void send(String s) {
    if (connected!) sock!.add(utf8.encode(s));
  }

  @override
  Widget build(BuildContext context) {
    return MyVForm(
      connected: connected,
      childrens: [
        mFormElement(text: 'Setpoint X', hint: 'meters'),
        mFormElement(text: 'Setpoint Y', hint: 'meters'),
        mFormElement(text: 'Setpoint Z', hint: 'meters'),
      ],
      sock: sock,
      title: 'Setpoint',
    );
  }
}
