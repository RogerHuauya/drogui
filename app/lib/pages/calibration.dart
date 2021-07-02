import 'package:flutter/material.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/constants.dart';
import 'package:test_app/utils/registerMap.dart';
import 'package:test_app/utils/utils.dart';

class _CalibrationProgress extends StatefulWidget {
  final String text;
  final IconData icon;
  final int rReg;
  final int wReg;
  _CalibrationProgress(
      {required this.text,
      required this.icon,
      required this.rReg,
      required this.wReg});
  _CalibrationProgressState createState() => _CalibrationProgressState();
}

class _CalibrationProgressState extends State<_CalibrationProgress> {
  bool writeReg(int reg, double val) {
    bool aux = GlobalWidget.of(context).droguiWrite(reg, val);
    logg((aux ? 'Sent $reg = $val!' : 'Not Connected :/'), context);
    return aux;
  }

  @override
  Widget build(BuildContext context) {
    return Row(children: [
      Expanded(
          child: InkWell(
              onTap: () => writeReg(widget.wReg, 1),
              child: Card(
                  shape: new RoundedRectangleBorder(
                      side: new BorderSide(color: primaryColor, width: 1.0),
                      borderRadius: BorderRadius.circular(4.0)),
                  child: Padding(
                      padding: EdgeInsets.all(10),
                      child: Row(children: <Widget>[
                        Icon(widget.icon),
                        Expanded(
                            child:
                                Text(widget.text, textAlign: TextAlign.center))
                      ]))))),
      SizedBox(
        width: 20,
      ),
      CircularProgressIndicator(
          value: GlobalWidget.of(context).getReg(widget.rReg),
          color: primaryColor)
    ]);
  }
}

class CalibrationPage extends StatelessWidget {
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
                    Text('Calibration',
                        style: TextStyle(
                            fontWeight: FontWeight.bold,
                            fontSize: 18,
                            color: primaryColor)),
                    SizedBox(height: 20),
                    _CalibrationProgress(
                        text: 'Gyroscope',
                        icon: Icons.rotate_left,
                        rReg: CAL_GYR,
                        wReg: CAL_GYR_TRG),
                    _CalibrationProgress(
                        text: 'Accelerometer',
                        icon: Icons.arrow_back,
                        rReg: CAL_ACC,
                        wReg: CAL_ACC_TRG),
                    _CalibrationProgress(
                        text: 'Magnetometer',
                        icon: Icons.east_rounded,
                        rReg: CAL_MAG,
                        wReg: CAL_MAG_TRG)
                  ])))
        ]);
  }
}
