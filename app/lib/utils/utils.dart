import 'package:flutter/material.dart';
import 'constants.dart';

void logg(String message, BuildContext context) {
  ScaffoldMessenger.of(context).showSnackBar(SnackBar(
    content: Text(message),
    duration: sentDuration,
  ));
}
