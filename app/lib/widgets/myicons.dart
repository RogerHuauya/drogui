import 'package:flutter/material.dart';
import 'package:test_app/utils/constants.dart';

class MyIconButton extends StatelessWidget {
  final GestureTapCallback function;
  final IconData icon;
  MyIconButton({required this.icon, required this.function});

  @override
  Widget build(BuildContext context) {
    return InkWell(
        onTap: function,
        child: Card(
            shape: new RoundedRectangleBorder(
                side: new BorderSide(color: primaryColor, width: 1.0),
                borderRadius: BorderRadius.circular(4.0)),
            child: Padding(
                padding: EdgeInsets.all(10),
                child: Icon(icon,
                size: 20,
                ))));
  }
}