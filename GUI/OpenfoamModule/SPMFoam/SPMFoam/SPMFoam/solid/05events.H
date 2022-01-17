
if(solidRegions[i].name() == "solidPhase")
{
  //if voltage is lower than 0V, toggle flag_voltage
  if(min(X).value() <= 0) 
  {flag_SOC = -1;}

  if(max(X).value() >= 1) 
  {flag_SOC = 1;}

}


  //if voltage is lower than 0V, toggle flag_voltage
  if(V < 0) 
  {flag_voltage = 1;}





