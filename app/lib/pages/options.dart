import 'package:flutter/material.dart';
import 'package:test_app/utils/constants.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/registerMap.dart';
import 'package:test_app/utils/utils.dart';

class ButtonWithText extends StatelessWidget {
  final IconData icon;
  final String text;
  final GestureTapCallback function;
  final bool start;

  ButtonWithText(
      {required this.icon,
      required this.text,
      required this.function,
      required this.start});

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
                child: Row(children: <Widget>[
                  Icon(icon),
                  Expanded(
                      child: Text((start ? "Stop " : "Start ") + text,
                          textAlign: TextAlign.center))
                ]))));
  }
}

class OptionsPage extends StatefulWidget {
  OptionsPageState createState() => OptionsPageState();
}

class OptionsPageState extends State<OptionsPage>
    with AutomaticKeepAliveClientMixin {
  @override
  bool get wantKeepAlive => true;

  List<bool> starts = [false, false, false];

  bool writeReg(int reg, double val) {
    bool aux = GlobalWidget.of(context).droguiWrite(reg, val);
    logg((aux ? 'Sent $reg = $val!' : 'Not Connected :/'), context);
    return aux;
  }

  bool sendCmd(String cmd) {
    bool aux = GlobalWidget.of(context).droguiCmd(cmd);
    logg((aux ? 'Sent cmd = $cmd!' : 'Not Connected :/'), context);
    return aux;
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return Column(
        mainAxisSize: MainAxisSize.min,
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          Card(
              child: Padding(
                  padding: EdgeInsets.fromLTRB(25, 30, 25, 30),
                  child: Column(children: [
                    Text(
                      'Options',
                      style: TextStyle(
                          fontWeight: FontWeight.bold,
                          fontSize: 18,
                          color: primaryColor),
                    ),
                    SizedBox(height: 20),
                    ButtonWithText(
                        icon: Icons.text_format,
                        text: "Logging",
                        function: () {
                          if (sendCmd(starts[0] ? 'n' : 'l'))
                            starts[0] = !starts[0];
                          setState(() {});
                        },
                        start: starts[0]),
                    ButtonWithText(
                        icon: Icons.auto_fix_high_sharp,
                        text: "XYC",
                        function: () {
                          if (writeReg(START_XYC, (starts[1] ? 0 : 1)))
                            starts[1] = !starts[1];
                          setState(() {});
                        },
                        start: starts[1]),
                    ButtonWithText(
                        icon: Icons.gps_fixed,
                        text: "GPS",
                        function: () {
                          if (writeReg(START_GPS, (starts[2] ? 0 : 1)))
                            starts[2] = !starts[2];
                          setState(() {});
                        },
                        start: starts[2])
                  ])))
        ]);
  }
}
