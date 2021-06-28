import 'package:flutter/material.dart';
import 'package:test_app/utils/constants.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/registerMap.dart';
import 'package:test_app/utils/utils.dart';

class SliderSent extends StatefulWidget {
  final String name;
  final double time;
  final int regVar;
  final int regTim;

  SliderSent(
      {required this.regVar,
      required this.regTim,
      required this.name,
      required this.time});

  SliderState createState() {
    return SliderState();
  }
}

class SliderState extends State<SliderSent> {
  double val = 0;

  bool writeReg(int reg, double val) {
    bool aux = GlobalWidget.of(context).droguiWrite(reg, val);
    logg((aux ? 'Sent $reg = ${val.toStringAsFixed(1)}!' : 'Not Connected :/'),
        context);
    return aux;
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Text(widget.name + ' (' + val.toStringAsFixed(1) + 'm)'),
        Expanded(
            child: Slider(
                min: 0,
                max: 5,
                onChanged: (newVal) => setState(() {
                      val = newVal;
                    }),
                value: val)),
        IconButton(
            constraints: BoxConstraints(maxHeight: 20, maxWidth: 20),
            splashRadius: 20,
            padding: new EdgeInsets.all(0.0),
            onPressed: () {
              writeReg(widget.regVar, val);
              writeReg(widget.regTim, widget.time);
            },
            iconSize: 20.0,
            icon: Icon(Icons.send))
      ],
    );
  }
}

class SlideSetpointPage extends StatefulWidget {
  SlideSetpointState createState() => SlideSetpointState();
}

class SlideSetpointState extends State<SlideSetpointPage>
    with AutomaticKeepAliveClientMixin {
  @override
  bool get wantKeepAlive => true;

  double time = 0;
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
                  Text('SetPoint',
                      style: TextStyle(
                        fontWeight: FontWeight.bold,
                        fontSize: 18,
                        color: primaryColor,
                      )),
                  SizedBox(height: 20),
                  SliderSent(
                      name: 'X', time: time, regVar: X_REF, regTim: X_REF_SIZE),
                  SliderSent(
                      name: 'Y', time: time, regVar: Y_REF, regTim: Y_REF_SIZE),
                  SliderSent(
                      name: 'Z', time: time, regVar: Z_REF, regTim: Z_REF_SIZE),
                  Row(children: [
                    Text('time'),
                    Expanded(
                        child: Slider(
                            value: time,
                            min: 0,
                            max: 5,
                            divisions: 5,
                            label: time.round().toString(),
                            onChanged: (value) => setState(() {
                                  time = value;
                                })))
                  ])
                ])))
      ],
    );
  }
}
