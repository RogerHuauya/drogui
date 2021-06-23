import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';

class TextFieldWithText extends StatelessWidget {
  final TextEditingController? control;
  final String? text;
  TextFieldWithText({@required this.text, this.control});

  @override
  Widget build(BuildContext context) {
    return Flexible(
        child: Padding(
            padding: EdgeInsets.all(10),
            child: Column(
              children: [
                TextField(
                  decoration: InputDecoration(hintText: text!),
                  controller: control,
                  keyboardType: TextInputType.number,
                ),
                Text(text!),
              ],
            )));
  }
}

class PIDPage extends StatelessWidget {
  final Socket? sock;
  PIDPage({@required this.sock});
  final TextEditingController pidKp = new TextEditingController();
  final TextEditingController pidKi = new TextEditingController();
  final TextEditingController pidKd = new TextEditingController();

  void send(String s) {
    sock!.add(utf8.encode(s));
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      crossAxisAlignment: CrossAxisAlignment.center,
      children: <Widget>[
        Text('PID'),
        Row(
          children: [
            TextFieldWithText(text: 'kp', control: pidKp),
            TextFieldWithText(text: 'ki', control: pidKi),
            TextFieldWithText(text: 'kd', control: pidKd)
          ],
        ),

        //TextFieldWithText(text: 'kp', control: pidKp),
        IconButton(
            onPressed: () =>
                send(pidKp.text + " " + pidKi.text + " " + pidKd.text),
            icon: Icon(Icons.send))
      ],
    );
  }
}
