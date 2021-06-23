import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';


class SetPointPage extends StatelessWidget{
	final Socket? sock;
	SetPointPage({@required this.sock});
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
					'Setpoint'
				),
				TextField(
					decoration: InputDecoration(
						hintText: 'X'
					),
					controller: pidKp,
					keyboardType : TextInputType.number,
				),
				IconButton(onPressed: () => send(pidKp.text), icon: Icon(Icons.send))

			],
		); 
	}
}