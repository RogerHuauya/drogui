import 'package:flutter/material.dart';

class TextFieldWithText extends StatelessWidget {
  final TextEditingController? control;
  final String? text;
  final String? hint;
	final bool? connected;
  TextFieldWithText({@required this.text, this.control, this.connected, this. hint});

  @override
  Widget build(BuildContext context) {
    return Flexible(
        child: Padding(
            padding: EdgeInsets.all(10),
            child: Column(
              children: [
                TextField(
									enabled: connected!,
                	textAlign: TextAlign.center,
                  decoration: InputDecoration(hintText: hint!),
                  controller: control,
                  keyboardType: TextInputType.number,
                ),
                Text(text!),
              ],
            )));
  }
}