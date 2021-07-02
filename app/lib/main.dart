import 'package:flutter/material.dart';
import 'package:test_app/pages/calibration.dart';
import 'package:test_app/pages/readWrite.dart';
import 'package:test_app/pages/sensors.dart';
import 'dart:io';
import 'utils/utils.dart';
import 'widgets/states.dart';
import 'pages/pidXYZ.dart';
import 'pages/slideSetpoint.dart';
import 'pages/options.dart';
import 'utils/constants.dart';
import 'global.dart';

Socket? socket;
bool connected = false;
void main() {
  runApp(MyApp());
}

class MyBehavior extends ScrollBehavior {
  @override
  Widget buildViewportChrome(
      BuildContext context, Widget child, AxisDirection axisDirection) {
    return child;
  }
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MasterWidget(
        child: MaterialApp(
      builder: (context, child) {
        return ScrollConfiguration(
          behavior: MyBehavior(),
          child: child!,
        );
      },
      title: 'Drogui',
      theme: ThemeData(
        primarySwatch: primaryColor,
      ),
      home: MyHomePage(title: 'Drogui'),
    ));
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final pgController = PageController(
    initialPage: 1,
  );

  bool connected = false;

  void connectSocket() {
    GlobalWidget.of(context).connectSocket();
    if (!connected) {
      logg('Cannot connect :(', context);
    }
    setState(() {});
  }

  void closeSocket() {
    GlobalWidget.of(context).closeSocket();
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
            title: Text(widget.title),
            backgroundColor: (GlobalWidget.of(context).connected
                ? Colors.green
                : Colors.red),
            actions: [
              IconButton(
                  onPressed: (GlobalWidget.of(context).connected
                      ? closeSocket
                      : () => connectSocket()),
                  icon: Icon((GlobalWidget.of(context).connected
                          ? Icons.power_off_sharp
                          : Icons.power_rounded) // add custom icons also
                      )),
              SizedBox(width: 10)
            ]),
        body: SingleChildScrollView(
            physics: ClampingScrollPhysics(),
            child: SizedBox(
              height: MediaQuery.of(context).size.height * 0.8,
              child: Stack(
                  fit: StackFit.expand,
                  alignment: Alignment.center,
                  children: <Widget>[
                    Padding(
                        padding: EdgeInsets.all(25),
                        child: PageView(
                          scrollDirection: Axis.vertical,
                          controller: pgController, children: [
                          OptionsPage(),
                          SlideSetpointPage(),
                          SensorsPage(),
                          ReadWritePage(),
                          PIDThreePage(),
                          CalibrationPage()
                        ])),
                    Positioned(
                        bottom: 20, left: 20, right: 20, child: StateBar())
                  ]),
            )));
  }
}
