
if(solidRegions[i].name() == "WE")
{
  //if voltage is lower than 0V, toggle flag_voltage
  if(min(theta).value() < -1.0e-5) 
  {flag_SOC = -1;}

  if(max(theta).value() >= 1) 
  {flag_SOC = 1;}

}


  //if voltage is lower than 0V, toggle flag_voltage
  if(V < 0) 
  {flag_voltage = 1;}





