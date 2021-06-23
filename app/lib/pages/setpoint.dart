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

class SetPointPage extends StatelessWidget {
  final Socket? sock;
  SetPointPage({@required this.sock});

  final TextEditingController x_ref = new TextEditingController();
  final TextEditingController y_ref = new TextEditingController();
  final TextEditingController z_ref = new TextEditingController();

  void send(String s) {
    sock!.add(utf8.encode(s));
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      crossAxisAlignment: CrossAxisAlignment.center,
      children: <Widget>[
        Text('Setpoint'),
        Row(
          children: [
            TextFieldWithText(text: 'X', control: x_ref),
            TextFieldWithText(text: 'Y', control: y_ref),
            TextFieldWithText(text: 'Z', control: z_ref)
          ],
        ),
        IconButton(
            onPressed: () =>
                send(x_ref.text + " " + y_ref.text + " " + z_ref.text),
            icon: Icon(Icons.send))
      ],
    );
  }
}
