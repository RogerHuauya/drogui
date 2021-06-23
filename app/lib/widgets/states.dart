import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';

class ButtonWithText extends StatelessWidget{

    final IconData? icon;
    final GestureTapCallback? function;
    final String? text;
    
    ButtonWithText({@required this.icon, this.function, this.text});
    
    

    @override
    Widget build(BuildContext context){
        return Expanded(
            child:GestureDetector(
                onTap: function,
                child: Card(child:Column(
                    children: <Widget>[
                        Icon(icon),
                        Text(text!)
                    ],
                ),),
            )
        );
    }
}



class StateBar extends StatelessWidget{
	final Socket? sock;
	StateBar({@required this.sock});
	void arm(){
		sock!.add(utf8.encode('arm'));
	}
	void start(){
		sock!.add(utf8.encode('start'));
	}
    void descend(){
		sock!.add(utf8.encode('descend'));
	}
    stop() => sock!.add(utf8.encode('stop'));
    
	@override
	Widget build(BuildContext context){

		return Row(
			mainAxisAlignment: MainAxisAlignment.center,
			children: <Widget>[
                ButtonWithText(text: 'Descend', icon: Icons.paragliding, function: descend),
                ButtonWithText(text: 'Stop', icon: Icons.anchor, function: stop),
                ButtonWithText(text: 'Arm', icon: Icons.construction, function: arm),
                ButtonWithText(text: 'Start', icon: Icons.airplanemode_active, function: start),
			],
		); 
	}
}