class ESP8266
{
private:
	int resetPin;
	int timeout;
	HardwareSerial &ser;
	Stream &dbg;
public:
	enum status_t { OK=0, BUSY, DNSFAIL, TIMEOUT, COMMS_ERROR };

	ESP8266(HardwareSerial& ser, Stream& dbg, int resetPin, int timeout) : 
		resetPin(resetPin), 
		timeout(timeout),
		ser(ser),
		dbg(dbg)
	{
	}

	void begin()
	{
		ser.begin(115200);
		ser.setTimeout(timeout);
		pinMode(resetPin, OUTPUT);
		digitalWrite(resetPin, HIGH);
		reset();
	}

	status_t reset()
	{
		dbg.println("Hardware reset");
		digitalWrite(resetPin, LOW);
		delay(100);
		digitalWrite(resetPin, HIGH);
		status_t status = waitFor("ready");
		if (status != OK) return status;
		return sendAndWait("AT+CWQAP", "OK"); // quit current AP
	}

	// Read characters from WiFi module and echo to serial until keyword occurs or timeout.
	status_t waitFor(String keyword)
	{
// TODO - improve this search. I think it will fail for some cases
		byte current_char   = 0;
		byte keyword_length = keyword.length();
		// Fail if the target string has not been sent by deadline.
		unsigned long deadline = millis() + timeout;
		while(millis() < deadline)
		{
			if (ser.available())
			{
				char ch = ser.read();
				dbg.write(ch);
				if (ch == keyword[current_char])
				{
					if (++current_char == keyword_length)
					{
						  dbg.println();
						  return OK;
					}
				}
			}
		}
		return TIMEOUT;
	}

	// Echo module output until 3 newlines encountered.
	// (Used when we're indifferent to "OK" vs. "no change" responses.)
	void skip()
	{
		waitFor("\n");        // Search for nl at end of command echo
		waitFor("\n");        // Search for 2nd nl at end of response.
		waitFor("\n");        // Search for 3rd nl at end of blank line.
	}

	// Send a command to the module and wait for acknowledgement string
	// (or flush module output if no ack specified).
	// Echoes all data received to the serial monitor.
	status_t sendAndWait(String cmd, String ack)
	{
		//ser.println(cmd);
/* attempt to slow down write to module - doesn't seem to help with 'busy'  */
		for (unsigned int i=0;i<cmd.length();i++)
		{
			ser.write(cmd[i]);
			ser.flush();
			delay(1);
		}
		ser.write("\r");
		ser.write("\n");
		// If no ack response specified, skip all available module output.
		if (ack == "")
		{
			skip();
		}
		else
		{
			// Otherwise wait for ack.
			return waitFor(ack);
		}
		return OK;
	}
	
	// Read and echo all available module output.
	// (Used when we're indifferent to "OK" vs. "no change" responses or to get around firmware bugs.)
	void flush()
	{
		while(ser.available()) dbg.write(ser.read());
	}

	status_t connectToAP(String ssid, String password)
	{
		String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
		return sendAndWait(cmd, "OK");
	}

	status_t connectToServer(byte cnum, String type, String host, int port)
	{
		// Allow multiple connections (we'll only use the first).
		// we set this each time, as it seem that the module can loose this setting?
		status_t status = sendAndWait("AT+CIPMUX=1", "OK");    
		if (status != OK) return status;
		String cmd = "AT+CIPSTART=" + String(cnum) + ",\"" + String(type) + "\",\"" + String(host) + "\"," + String(port);
		return sendAndWait(cmd, "Linked");
		/* 
		need to wait for 'OK' then  'Linked'
		other possible responses are 'DNS Fail'
		'Link typ ERROR' - can occur when MUX=0

		*/
	}
	status_t sendNetworkData(byte cnum, String data)
	{
		String cmd = "AT+CIPSEND=" + String(cnum) + "," + String(data.length());
		status_t status = sendAndWait(cmd, ">");
		if (status != OK) return status;
		dbg.print("Sending:[");
		dbg.print(data);
		dbg.println("]");
		return sendAndWait(data, "SEND OK");
		// possible other responses include 'link is not'
	}

	status_t closeConnection(byte cnum)
	{
		String cmd = "AT+CIPCLOSE=" + String(cnum);
		return sendAndWait(cmd, "OK");
		// possible other responses include 'link is not'
	}
};

