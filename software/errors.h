// errors
// ============================================================

void errors(String num="0") {
	
	

  
	// Errors 101 - 199 about accessing a file from the SD card
  // ==========================================================
	if (num == "101") {
  	SCREEN.setCursor(13,60);
  	SCREEN.print(F("File was not found on the SD card.\n")); 
	}

 
  // Errors 201 - 299 about processing an MSF tuning file
  // ==========================================================
  if (num >= "201" & num <= "209") {
    SCREEN.setCursor(13,60);
    SCREEN.print(F("Processing the MSF file failed.\n")); 
  }

  // evaluating the MSF file format
  if (num >= "211" & num <= "219") {
    SCREEN.setCursor(13,60);
    SCREEN.print(F("The MSF file seems to be incorrect!\n")); 
  }


  // Errors 901 - 999 about the sound server and sampler engine
  // ==========================================================
  if (num >= "981") {
    SCREEN.setCursor(13,60);
    SCREEN.print(F("please wait.\n")); 
  }
}
