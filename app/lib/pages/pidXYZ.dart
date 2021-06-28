import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:test_app/utils/constants.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/registerMap.dart';
import 'package:test_app/utils/utils.dart';

class ThreeTextFields extends StatefulWidget {
  final String name;
  final List<int> pidReg;
  final double vari;
  ThreeTextFields(
      {required this.name, required this.pidReg, required this.vari});

  ThreeTextFieldsState createState() => ThreeTextFieldsState();
}

class _MyTextField extends StatelessWidget {
  final TextEditingController ed;
  _MyTextField({required this.ed});
  Widget build(BuildContext context) {
    return Flexible(
        child: Padding(
            padding: EdgeInsets.symmetric(horizontal: 20),
            child: TextField(
                textAlign: TextAlign.center,
                keyboardType: TextInputType.number,
                controller: ed)));
  }
}

class ThreeTextFieldsState extends State<ThreeTextFields>
    with AutomaticKeepAliveClientMixin {
  final TextEditingController kp = TextEditingController();
  final TextEditingController kd = TextEditingController();
  final TextEditingController ki = TextEditingController();

  @override
  bool get wantKeepAlive => true;

  String name = "";
  bool punto = false;
  @override
  void initState() {
    name = widget.name;
    punto = false;
    super.initState();
  }

  bool writeRegs() {
    double kpv = double.tryParse(kp.text) ?? 0;
    double kiv = double.tryParse(ki.text) ?? 0;
    double kdv = double.tryParse(kd.text) ?? 0;

    bool aux = GlobalWidget.of(context).droguiWrite(PID_INDEX, (punto ? 1 : 0));
    aux &= GlobalWidget.of(context).droguiWrite(PID_VAR, widget.vari);
    aux &= GlobalWidget.of(context).droguiWrite(widget.pidReg[0], kpv);
    aux &= GlobalWidget.of(context).droguiWrite(widget.pidReg[1], kiv);
    aux &= GlobalWidget.of(context).droguiWrite(widget.pidReg[2], kdv);
    logg((aux ? 'PID $name = $kpv $kiv $kdv!' : 'Not Connected :/'), context);
    return aux;
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return Row(children: [
      InkWell(
        onTap: () => setState(() {
          if (punto) {
            punto = false;
            name = name.substring(0, name.length - 1);
          } else {
            name += 'p';
            punto = true;
          }
        }),
        child: Text(name),
      ),
      Expanded(
          child: Row(children: [
        _MyTextField(ed: kp),
        _MyTextField(ed: ki),
        _MyTextField(ed: kd)
      ])),
      IconButton(
          constraints: BoxConstraints(maxHeight: 20, maxWidth: 20),
          splashRadius: 20,
          padding: new EdgeInsets.all(0.0),
          onPressed: writeRegs,
          iconSize: 20.0,
          icon: Icon(Icons.send))
    ]);
  }
}

class _MyTitleLetter extends StatelessWidget {
  final String letter;
  _MyTitleLetter({required this.letter});
  Widget build(BuildContext context) {
    return Expanded(
        child: Text(letter,
            textAlign: TextAlign.center,
            style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 18,
                color: primaryColor)));
  }
}

class PIDThreePage extends StatelessWidget {
  final TextEditingController pidKp = new TextEditingController();
  final TextEditingController pidKi = new TextEditingController();
  final TextEditingController pidKd = new TextEditingController();

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisSize: MainAxisSize.min,
      mainAxisAlignment: MainAxisAlignment.center,
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [
        Card(
            child: Padding(
                padding: EdgeInsets.fromLTRB(25, 30, 25, 30),
                child: Column(children: [
                  Row(children: [
                    _MyTitleLetter(letter: 'P'),
                    _MyTitleLetter(letter: 'I'),
                    _MyTitleLetter(letter: 'D'),
                  ]),
                  SizedBox(height: 20),
                  ThreeTextFields(
                      name: 'X', pidReg: [X_KP, X_KI, X_KD], vari: 0),
                  ThreeTextFields(
                      name: 'Y', pidReg: [Y_KP, Y_KI, Y_KD], vari: 1),
                  ThreeTextFields(
                      name: 'Z', pidReg: [Z_KP, Z_KI, Z_KD], vari: 2),
                ])))
      ],
    );
  }
}
