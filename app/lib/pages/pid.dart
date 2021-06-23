import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';


class PIDPage extends StatelessWidget{
	final Socket? sock;
	PIDPage({@required this.sock});
	final TextEditingController pidKp = new TextEditingController();
	void send(String s){
		sock!.add(utf8.encode(s));
	}
	@override
	Widget build(BuildContext context){

		return Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
			children: <Widget>[
				Text(
					'PID'
				),
				TextField(
					decoration: InputDecoration(
						hintText: 'KP'
					),
					controller: pidKp,
					keyboardType : TextInputType.number,
				),
				IconButton(onPressed: () => send(pidKp.text), icon: Icon(Icons.send))

			],
		); 
	}
}