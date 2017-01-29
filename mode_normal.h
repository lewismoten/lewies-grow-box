void modeNormalStart() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
}

bool normalLoop() {


  // get current time
  tmElements_t tm;
  time_t t;
  if (RTC.read(tm)) {
    t = makeTime(tm);
  }

  String line1 = "";
  line1 = "";
  line1 += dayStr(weekday(t));

  // Line 2 - Date  
  String line2 = "";
  line2 += monthShortStr(tm.Month);
  line2 += " ";
  line2 += tm.Day;
  line2 += ", ";
  line2 += tmYearToCalendar(tm.Year);
  if(tm.Day < 10) {
    line2 += " ";
  }

  String line3 = "";
  line3 += hourFormat12(t);
  line3 += ":";
  if(tm.Minute < 10) line3 += "0";
  line3 += tm.Minute;
  line3 += ":";
  if(tm.Second < 10) line3 += "0";
  line3 += tm.Second;
  line3 += " ";
  line3 += isAM(t) ? "AM" : "PM";
  
  display.setCursor(0, 0);
  display.println(line1);
  display.println(line2);
  display.println(line3);
  display.display();

  return true;
}
