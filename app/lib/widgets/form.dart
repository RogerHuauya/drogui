import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';
import '../widgets/textFieldWithText.dart';

// ignore: camel_case_types
class mFormElement {
  final String? text;
  final String? hint;

  mFormElement({required this.text, required this.hint});
}

class MyForm extends StatelessWidget {
  final Socket? sock;
  final String? title;
  final bool? connected;
  final List<mFormElement>? childrens;

  MyForm({this.sock, this.connected, this.childrens, this.title});

  late final List<TextEditingController?> edits;

  void send(String message, BuildContext context) {
    if (connected!) sock!.add(utf8.encode(message));
    ScaffoldMessenger.of(context).showSnackBar(new SnackBar(
        content: new Text("Sent " + message + " !!!"),
        duration: Duration(milliseconds: 300)));
  }

  @override
  Widget build(BuildContext context) {
    return Column(
        mainAxisSize: MainAxisSize.min,
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: <Widget>[
          Card(
              child: Padding(
                  padding: EdgeInsets.all(20),
                  child: Column(
                    children: [
                      Text(title!),
                      Row(children: getWidgets()),
                      IconButton(
                          onPressed: () => send('case', context),
                          icon: Icon(Icons.send))
                    ],
                  )))
        ]);
  }

  List<Widget> getWidgets() {
    List<Widget> widgs = []..length = 0;
    edits = []..length = childrens!.length;
    print(childrens!.length);

    for (int i = 0; i < childrens!.length; i++) {
      edits[i] = new TextEditingController();
      widgs.add(TextFieldWithText(
        text: childrens![i].text,
        hint: childrens![i].hint,
        control: edits[i],
        connected: connected,
      ));
    }

    return widgs;
  }
}

class MyVForm extends StatelessWidget {
  final Socket? sock;
  final String? title;
  final bool? connected;
  final List<mFormElement>? childrens;

  MyVForm({this.sock, this.connected, this.childrens, this.title});

  late final List<TextEditingController?> edits;

  void send(String message, BuildContext context) {
    if (connected!) sock!.add(utf8.encode(message));
    ScaffoldMessenger.of(context).showSnackBar(new SnackBar(
        content: new Text("Sent " + message + " !!!"),
        duration: Duration(milliseconds: 300)));
  }

  @override
  Widget build(BuildContext context) {
    return Column(
        mainAxisSize: MainAxisSize.min,
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: <Widget>[
          Card(
              child: Padding(
                  padding: EdgeInsets.all(20),
                  child: Column(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      Text(title!),
                      Column(
                          mainAxisSize: MainAxisSize.min,
                          children: getWidgets()),
                      IconButton(
                          onPressed: () => send(getMessage(), context),
                          icon: Icon(Icons.send))
                    ],
                  )))
        ]);
  }

  String getMessage() {
    String message = '';
    for (TextEditingController? i in edits) {
      message += i!.text + ' ';
    }
    return message;
  }

  List<Widget> getWidgets() {
    List<Widget> widgs = []..length = 0;
    edits = []..length = childrens!.length;
    print(childrens!.length);

    for (int i = 0; i < childrens!.length; i++) {
      edits[i] = new TextEditingController();
      widgs.add(TextField(
          decoration: InputDecoration(
              labelText: childrens![i].text, hintText: childrens![i].hint),
          controller: edits[i],
          enabled: connected,
          keyboardType: TextInputType.number
          //connected: connected,
          ));
    }

    return widgs;
  }
}
