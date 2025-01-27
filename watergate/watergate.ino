#include <EtherCard.h>
#include "button.h"

static byte mymac[] = { 0xCA,0xFE,0x42,0x42,0x42,0x42 };

byte Ethernet::buffer[1024]; // tcp/ip send and receive buffer
static BufferFiller bfill;  // used as cursor while filling the buffer

#define PIN_SF 7 // sparkfun valve. set PIN_SF high for on, low for off

int sf_status = -1;
unsigned long sf_timeout = 0;
unsigned long time = 0;

const char okResponse[] PROGMEM =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n\r\n"
    "<html><head><style> td, a { font-size: 5em; } table, .btn { display: block; }"
    ".on { background-color: green; } .off { background-color: red; }"
    "</style></head><body><table><tr><td>hose $F</td></tr>"
    "<tr>"
        "<td><a class='btn on' href=/sf/on/10>on</a></td></tr>"
    "<tr>"
        "<td><a class='btn off' href=/sf/off>off</a></td></tr>"
    "<tr><td>$D</td></tr>" // times remaining for both waters
    "</table></body></html>"
;

const char on[] PROGMEM = "ON";
const char off[] PROGMEM = "OFF";
const char unknown[] PROGMEM = "?";

#define STATUS_STR(status_int) (status_int > 0 ? on : (status_int == 0 ? off : unknown))

const char redirectHeader[] PROGMEM =
    "HTTP/1.1 302\r\n"
    "Pragma: no-cache\r\n"
    "Location: /\r\n"
;

void setup() {
  pinMode(PIN_SF, OUTPUT);
  digitalWrite(PIN_SF, LOW);

  Serial.begin(57600);
  Serial.println("\n[backSoon]");

  if(ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0) {
    Serial.println("Failed to access Ethernet controller");
  }
  if(!ether.dhcpSetup()) {
    Serial.println("Failed to get DHCP address");
  }

  ether.printIp("IP: ", ether.myip);
  ether.printIp("GW: ", ether.gwip);
  ether.printIp("Mask: ", ether.netmask);  
  ether.printIp("DNS: ", ether.dnsip);  
}

void loop() {
  time = millis();
  if (sf_timeout && time >= sf_timeout) {
    sf_off();
    sf_timeout = 0;
  }
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  word data_len = len - pos;
  // check if valid tcp data is received
  if (pos) {
    bfill = ether.tcpOffset();
    char* data = (char *) Ethernet::buffer + pos;
    Serial.println(data);
    
    // receive buf hasn't been clobbered by reply yet
    if((data_len >= 10) && strncmp("GET /sf/on", data, 10) == 0) {
      
      sf_on();
      
      if(data_len > 11) {
        // if the URL was /sf/on/w
        if(strncmp(data+10, "/w", 2) == 0) {
          //if there's at least 30 seconds remaining of existing timeout
          if(sf_timeout >= time + 30 * 1000) {
            // if there's at least 4 minutes remaining of existing timeout
            if(sf_timeout >= time + 60 * 4 * 1000) {
            Serial.println("Turning on water for 20 minutes");
              sf_timeout = time + (unsigned long) 60 * 20 * 1000; // set timeout to 20 minutes
            } else { // more than 30 seconds but less than 4 minutes remianing
            Serial.println("Turning on water for 5 minutes");
              sf_timeout = time + (unsigned long) 60 * 5 * 1000; // set timeout to 5 minutes
            }
          } else { // less than 30 seconds remaining
            Serial.println("Turning on water for 1 minute");
            sf_timeout = time + (unsigned long) 60 * 1000; // set timeout to 1 minute
          }
          
        } else { // if the URL was /sf/on/<some_number>
          // turn on for <some_number> of minutes
          Serial.println("/<time>");
          sf_timeout = time + (unsigned long) atoi(data+11) * 60000;
        }
      }
      
    } else if((data_len >= 11) && strncmp("GET /sf/off", data, 11) == 0) {
      
      sf_off();
      
    } else {
      
      bfill.emit_p(okResponse, STATUS_STR(sf_status), sf_timeout - time);
      ether.httpServerReply(bfill.position()); // send web page data
      return;
    }

    bfill.emit_p(PSTR("$F\r\n"), redirectHeader);
    ether.httpServerReply(bfill.position()); // redirect to /
  }

}
void emit_status(int water_status, unsigned long timeout, BufferFiller& buf) {
  if (water_status == 0)
    buf.emit_p(PSTR("OFF "));
  else if (water_status == 1)
    buf.emit_p(PSTR("ON "));
  else
    buf.emit_p(PSTR("? "));

  if (timeout) {
    buf.emit_p(PSTR("$F"), timeout - time);
  }
}


void pulse(int pin) {
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
}

void sf_on() {
  Serial.println("SF on");
  digitalWrite(PIN_SF, HIGH);
  sf_status = 1;
}

void sf_off() {
  Serial.println("SF off");
  digitalWrite(PIN_SF, LOW);
  sf_status = 0;
  sf_timeout = 0;
}
