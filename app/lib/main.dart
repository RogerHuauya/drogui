import 'package:flutter/material.dart';
import 'dart:io';
import 'widgets/states.dart';
import 'pages/pid.dart';
import 'pages/setpoint.dart';

late Socket socket;
void main() async {
  socket = await Socket.connect('191.98.175.75', 1194);
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Drogui',
      theme: ThemeData(
        primarySwatch: Colors.green,
      ),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  bool connected = true;
  // send hello
  final pgController = PageController(
    initialPage: 1,
  );

  void close() {
    socket.close();
    connected = false;
    print('disconnected');
    setState(() {});
  }

  void conn() async {
    socket = await Socket.connect('191.98.175.75', 1194);
    connected = true;
    print('connected');
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
				
        leading: GestureDetector(
          onTap: (connected ? close : conn),
          child: Icon(
            (connected
                ? Icons.power_off_sharp 
                : Icons.power_rounded  ), // add custom icons also
          ),
        ),
      ),
      body: Stack(
          fit: StackFit.expand,
          alignment: Alignment.center,
          children: <Widget>[
            Padding(
                padding: EdgeInsets.all(50),
                child: PageView(
                  controller: pgController,
                  children: [PIDPage(sock: socket), SetPointPage(sock: socket)],
                )),
            Positioned(
                bottom: 20,
                left: 20,
                right: 20,
                child: StateBar(
                  sock: socket,
                ))
          ]),
    );
  }
}
