import processing.net.*;
import processing.serial.*; 
//import oscP5.*;
//import netP5.*;

String HTTP_GET_REQUEST = "GET /";
String HTTP_HEADER = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
//String RECEIVER_IP = "128.122.98.14";
//int RECEIVER_PORT = 12000;

Server s;
Client c;
//OscP5 oscP5;
Serial port;
//NetAddress myRemoteLocation;

String input;
char fromFacebook;
boolean sendOpenFacebook = false;
boolean sendCloseFacebook = false;
boolean turnOffByHand = false;

void setup() {
  size(100, 100);
  frameRate(25);
  s = new Server(this, 8080); // start server on http-alt
  port = new Serial(this, Serial.list()[4], 9600);
  port.bufferUntil('\n');
//  /* start oscP5, listening for incoming messages at port 12000 */
//  oscP5 = new OscP5(this, RECEIVER_PORT);
//  myRemoteLocation = new NetAddress(RECEIVER_IP, RECEIVER_PORT);
}

void draw() {
  // Receive data from client
  c = s.available();
  if (c != null) {
    input = c.readString();
    if (input.indexOf("\n") >=0) {
      input = input.substring(0, input.indexOf("\n")); // Only up to the newline
      //println(input);
    }

    if (input.indexOf(HTTP_GET_REQUEST) == 0) // starts with ...
    {
      int pos=input.indexOf("on_view=");
      if (pos>=0) {
        fromFacebook = input.charAt(pos+8);
        println(fromFacebook);
        if (fromFacebook == 'Y' && sendOpenFacebook == false) {
          sendOpenFacebook = true;
          sendCloseFacebook = false;
          port.write(fromFacebook);
        }
        if (fromFacebook == 'N' && sendCloseFacebook == false) {
          sendCloseFacebook = true;
          sendOpenFacebook = false;
          port.write(fromFacebook);
        }
      }

      // turn off the Facebook by turning off the sand timer
      if (turnOffByHand) {
        // send msg to the other computer
//        /* in the following different ways of creating osc messages are shown by example */
//        OscMessage myMessage = new OscMessage("N");
//        /* send the message */
//        oscP5.send(myMessage, myRemoteLocation);

        // write to a HTML page
        c.write(HTTP_HEADER);  // answer that we're ok with the request and are gonna send html
        // some html
        c.write("<html><head><title>Processing talking</title></head><body><h3>facebook: ");
        println("turnOffByHand");
        c.write("close");
        turnOffByHand = false;

        c.write("</h3></body></html>");
      }
      // close connection to client, otherwise it's gonna wait forever
      c.stop();
    }
  }
}

void serialEvent(Serial port) { 
  // read the serial buffer:
  String myString = port.readStringUntil('\n');
  myString = trim(myString);
  if (myString.equals("B")) {
    turnOffByHand = true;
  }
}


//void mousePressed() {
//  /* in the following different ways of creating osc messages are shown by example */
//  OscMessage myMessage = new OscMessage("N");
//  /* send the message */
//  oscP5.send(myMessage, myRemoteLocation);
//}
//
//void oscEvent(OscMessage theOscMessage) {
//  /* print the address pattern and the typetag of the received OscMessage */
//  print("### received an osc message.");
////  print(" content: "+theOscMessage.addrPattern());
////  println(" typetag: "+theOscMessage.typetag());
////  char remoteInput = theOscMessage.addrPattern().charAt(0); 
////  println(remoteInput);
//}

