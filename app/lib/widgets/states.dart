import 'package:flutter/material.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/registerMap.dart';
import 'package:test_app/utils/utils.dart';

class ButtonWithText extends StatelessWidget {
  final IconData icon;
  final GestureTapCallback function;
  final String text;

  ButtonWithText(
      {required this.icon, required this.function, required this.text});

  @override
  Widget build(BuildContext context) {
    return Expanded(
        child: InkWell(
            onTap: function,
            child: Card(
              child: Padding(
                padding: EdgeInsets.all(5),
                child: Column(
                  children: <Widget>[Icon(icon), Text(text)],
                ),
              ),
            )));
  }
}

class StateBar extends StatelessWidget {
  void writeReg(int reg, double val, BuildContext context) {
    bool aux = GlobalWidget.of(context).droguiWrite(reg, val);
    logg((aux ? 'Sent $reg = $val!' : 'Not Connected :/'), context);
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: <Widget>[
        ButtonWithText(
            text: 'Descend',
            icon: Icons.paragliding,
            function: () => writeReg(DESC, 1.0, context)),
        ButtonWithText(
            text: 'Stop',
            icon: Icons.anchor,
            function: () => writeReg(STOP, 1.0, context)),
        ButtonWithText(
            text: 'Arm',
            icon: Icons.construction,
            function: () => writeReg(ARM, 1.0, context)),
        ButtonWithText(
            text: 'Start',
            icon: Icons.airplanemode_active,
            function: () => writeReg(START, 1.0, context)),
      ],
    );
  }
}
