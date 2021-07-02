import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:flutter/services.dart';
import 'package:test_app/utils/constants.dart';
import 'package:test_app/global.dart';
import 'package:test_app/utils/utils.dart';
import 'package:test_app/widgets/myicons.dart';

class ReadWritePage extends StatefulWidget {
  ReadWritePageState createState() => ReadWritePageState();
}


class ReadWritePageState extends State<ReadWritePage>
    with AutomaticKeepAliveClientMixin {
  @override
  bool get wantKeepAlive => true;
  final TextEditingController readRg = new TextEditingController();
  final TextEditingController writeRg = new TextEditingController();
  final TextEditingController writeVal = new TextEditingController();

  bool writeReg(int reg, double val) {
    bool aux = GlobalWidget.of(context).droguiWrite(reg, val);
    logg((aux ? 'Sent $reg = $val!' : 'Not Connected :/'), context);
    return aux;
  }

  void readReg(int reg) {
    bool aux = GlobalWidget.of(context).droguiRead(reg);
    logg((aux ? 'Read $reg' : 'Not Connected :/'), context);
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return Column(
      mainAxisSize: MainAxisSize.min,
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Card(
          child: Padding(
            padding: EdgeInsets.fromLTRB(25, 30, 25, 30),
            child: Column(
              children: [
                Text('Read & Write',
                    style: TextStyle(
                        fontWeight: FontWeight.bold,
                        fontSize: 18,
                        color: primaryColor)),
                SizedBox(height: 20),
                Row(children: [
                  Text('Read '),
                  Expanded(
                      child: Padding(
                          padding: EdgeInsets.symmetric(horizontal: 5),
                          child: TextField(
                              textAlign: TextAlign.center,
                              keyboardType: TextInputType.number,
                              inputFormatters: [
                                FilteringTextInputFormatter.digitsOnly
                              ],
                              controller: readRg))),
                  Expanded(
                      child: Text(
                          GlobalWidget.of(context)
                              .getReg(int.tryParse(readRg.text) ?? 0)
                              .toStringAsFixed(2),
                          textAlign: TextAlign.center)),
                  MyIconButton(
                      function: () => readReg(int.tryParse(readRg.text) ?? 0),
                      icon: Icons.read_more)
                ]),
                Row(children: [
                  Text('Write'),
                  Expanded(
                      child: Padding(
                          padding: EdgeInsets.symmetric(horizontal: 5),
                          child: TextField(
                              textAlign: TextAlign.center,
                              keyboardType: TextInputType.number,
                              inputFormatters: [
                                FilteringTextInputFormatter.digitsOnly
                              ],
                              controller: writeRg))),
                  Expanded(
                      child: Padding(
                          padding: EdgeInsets.symmetric(horizontal: 5),
                          child: TextField(
                              textAlign: TextAlign.center,
                              keyboardType: TextInputType.number,
                              controller: writeVal))),
                  MyIconButton(
                      function: () => writeReg(int.tryParse(writeRg.text) ?? 0,
                          double.tryParse(writeVal.text) ?? 0),
                      icon: Icons.send)
                ])
              ],
            ),
          ),
        )
      ],
    );
  }
}
