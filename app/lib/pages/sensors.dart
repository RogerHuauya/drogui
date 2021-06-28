import 'package:flutter/material.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/constants.dart';
import 'package:test_app/utils/registerMap.dart';
import 'package:test_app/utils/utils.dart';

class ButtonWithText extends StatelessWidget {
  final IconData icon;
  final String text;
  final bool selected;
  final GestureTapCallback function;

  ButtonWithText(
      {required this.icon,
      required this.text,
      required this.selected,
      required this.function});

  @override
  Widget build(BuildContext context) {
    return Expanded(
        child: InkWell(
            onTap: function,
            child: Card(
              shape: new RoundedRectangleBorder(
                  side: new BorderSide(
                      color: (selected ? selectedColor : primaryColor),
                      width: 1.0),
                  borderRadius: BorderRadius.circular(4.0)),
              child: Padding(
                  padding: EdgeInsets.all(10),
                  child: Column(children: <Widget>[Icon(icon), Text(text)])),
            )));
  }
}

class SensorsPage extends StatefulWidget {
  SensorsPageState createState() => SensorsPageState();
}

class SensorsPageState extends State<SensorsPage>
    with AutomaticKeepAliveClientMixin {
  @override
  bool get wantKeepAlive => true;

  int selected = -1;
  List<String> names1 = ['gx', 'ax', 'roll', 'gps x', 'x', 'ox'];
  List<String> names2 = ['gy', 'ay', 'pitch', 'gps y', 'y', 'oy'];
  List<String> names3 = ['gz', 'az', 'yaw', 'gps z', 'z', 'z'];

  List<int> var1 = [GYRO_X, ACC_X, ROLL_VAL, GPS_X, X_VAL, XP_VAL];
  List<int> var2 = [GYRO_Y, ACC_Y, PITCH_VAL, GPS_Y, Y_VAL, YP_VAL];
  List<int> var3 = [GYRO_Z, ACC_Z, YAW_VAL, Z_VAL, Z_VAL, Z_VAL];

  void readReg(int id) {
    bool aux = GlobalWidget.of(context).droguiRead(var1[id]);
    aux &= GlobalWidget.of(context).droguiRead(var2[id]);
    aux &= GlobalWidget.of(context).droguiRead(var3[id]);
    logg(
        (aux ? 'Read ${var1[id]} ${var2[id]} ${var3[id]}' : 'Not Connected :/'),
        context);
    if (!aux) return;

    selected = id;
    print(selected);
    setState(() {});
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
              child: Column(
                children: [
                  Text(
                    'Sensors',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontSize: 18,
                      color: primaryColor,
                    ),
                  ),
                  SizedBox(height: 20),
                  Row(
                    children: [
                      Expanded(
                          child: Text(
                              GlobalWidget.of(context)
                                  .getReg((selected >= 0 ? var1[selected] : 0))
                                  .toStringAsFixed(2),
                              textAlign: TextAlign.center)),
                      Expanded(
                          child: Text(
                              GlobalWidget.of(context)
                                  .getReg((selected >= 0 ? var2[selected] : 0))
                                  .toStringAsFixed(2),
                              textAlign: TextAlign.center)),
                      Expanded(
                          child: Text(
                              GlobalWidget.of(context)
                                  .getReg((selected >= 0 ? var3[selected] : 0))
                                  .toStringAsFixed(2),
                              textAlign: TextAlign.center))
                    ],
                  ),
                  Row(
                    children: [
                      Expanded(
                          child: Text((selected >= 0 ? names1[selected] : '-'),
                              textAlign: TextAlign.center)),
                      Expanded(
                          child: Text((selected >= 0 ? names2[selected] : '-'),
                              textAlign: TextAlign.center)),
                      Expanded(
                          child: Text((selected >= 0 ? names3[selected] : '-'),
                              textAlign: TextAlign.center)),
                    ],
                  ),
                  SizedBox(height: 20),
                  Row(children: [
                    ButtonWithText(
                      text: 'Gyro',
                      icon: Icons.ac_unit_outlined,
                      selected: selected == 0,
                      function: () => readReg(0),
                    ),
                    ButtonWithText(
                      text: 'Acc',
                      icon: Icons.face_retouching_natural,
                      selected: selected == 1,
                      function: () => readReg(1),
                    ),
                    ButtonWithText(
                      text: 'RPY',
                      icon: Icons.umbrella,
                      selected: selected == 2,
                      function: () => readReg(2),
                    )
                  ]),
                  Row(children: [
                    ButtonWithText(
                      text: 'GPS',
                      icon: Icons.zoom_in,
                      selected: selected == 3,
                      function: () => readReg(3),
                    ),
                    ButtonWithText(
                      text: 'XYZ',
                      icon: Icons.label_important_outline,
                      selected: selected == 4,
                      function: () => readReg(4),
                    ),
                    ButtonWithText(
                      text: 'Opt',
                      icon: Icons.dangerous,
                      selected: selected == 5,
                      function: () => readReg(5),
                    )
                  ])
                ],
              ),
            ),
          )
        ]);
  }
}
